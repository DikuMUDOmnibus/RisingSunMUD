#include "../mud.h"
#include "../utils.h"
#include "../room.h"
#include "../hooks.h"
#include "../character.h"
#include "../exit.h"
#include "../buffer.h"
#include "../auxiliary.h"
#include "../storage.h"
#include "../zone.h"
#include "../world.h"
#include "../room_reset.h"
#include "../inform.h"

#include "buildwalk.h"

// Forward declare some functions stolen from redit.c
// TODO Remove reliance on redit.c by creating our on versions of these requirements
typedef struct {} ROOM_OLC;
ROOM_OLC *roomOLCFromProto(PROTO_DATA *);
RESET_LIST *roomOLCGetResets(ROOM_OLC *);
ROOM_DATA *roomOLCGetRoom(ROOM_OLC *);
void save_room_olc(ROOM_OLC *);
void roomOLCSetAbstract(ROOM_OLC *, bool);
void roomOLCSetKey(ROOM_OLC *, const char *);

// Auxiliary data in every module
typedef struct {
  BUFFER *prototype;
} BUILDWALK_AUX_DATA;

// New data set
BUILDWALK_AUX_DATA *newBuildwalkAuxData() {
  BUILDWALK_AUX_DATA *data = malloc(sizeof(BUILDWALK_AUX_DATA));
  data->prototype = newBuffer(1);
  return data;
}

// Delete a set
void deleteBuildwalkAuxData(BUILDWALK_AUX_DATA *data) {
  bufferClear(data->prototype);
  deleteBuffer(data->prototype);
  free(data);
}

// Copy a set
BUILDWALK_AUX_DATA *copyBuildwalkAuxData(BUILDWALK_AUX_DATA *from) {
  BUILDWALK_AUX_DATA *to = newBuildwalkAuxData();
  to->prototype = from->prototype;
  return to;
}

// Copy a set to another
void copyBuildwalkAuxDataTo(BUILDWALK_AUX_DATA *from, BUILDWALK_AUX_DATA *to) {
  to->prototype = from->prototype;
}

// Store a set
STORAGE_SET *storeBuildwalkAuxData(BUILDWALK_AUX_DATA *data) {
  STORAGE_SET *store = new_storage_set();
  if (data->prototype) store_string(store, "prototype", bufferString(data->prototype));
  return store;
}

// Read a set
BUILDWALK_AUX_DATA *readBuildwalkAuxData(STORAGE_SET *store) {
  BUILDWALK_AUX_DATA *data = newBuildwalkAuxData();
  if (read_string(store, "prototype")) {
    bufferClear(data->prototype);
	bufferCat(data->prototype, read_string(store, "prototype"));
  }
  return data;
}

void tryBuildwalk(const char *info) {
  CHAR_DATA *ch = NULL;
  char *dir;
  hookParseInfo(info, &ch, &dir); // Grab the character and direction used from the hook
  BUILDWALK_AUX_DATA *data = charGetAuxiliaryData(ch, "buildwalk_aux_data");
  if ( 0 != strcmp("", bufferString(data->prototype)) ) {
    // We need a key for the room
    const char *key_room = roomGetClass(charGetRoom(ch));
    char zone_name[SMALL_BUFFER];
    char room_name[SMALL_BUFFER];
    parse_worldkey_relative(ch, key_room, room_name, zone_name);
    ZONE_DATA *zone = worldGetZone(gameworld, zone_name);
	if (canEditZone(zone, ch)) {
	  // Start setting up the room
	  ROOM_OLC *olc = NULL;
	  PROTO_DATA *proto = NULL;
	  proto  = worldGetType(gameworld, "rproto", get_fullkey(bufferString(data->prototype), zone_name));
	  if ( NULL != proto ) {
		olc = roomOLCFromProto(proto);
		// Set the new key
		int room_found = 0;
		int room_number = 1;
		char new_room_name[SMALL_BUFFER];
		PROTO_DATA *test_room = NULL;
		// Strip a prefix from abstract rooms, if needed
		const char *prefix = "abstract_"; // TODO Set this via define, possibly in header
		char base_name[SMALL_BUFFER];
		if ( bufferLength(data->prototype) > strlen(prefix) ) {
		  char *possible_prefix = malloc(sizeof(char[strlen(prefix)]));
		  strncpy(possible_prefix, bufferString(data->prototype), strlen(prefix));
		  if ( 0 == strcmp(possible_prefix, prefix) ) {
		    strncpy(base_name, bufferString(data->prototype) + strlen(prefix), 
				bufferLength(data->prototype) - strlen(prefix));
		  } else {
		    strcpy(base_name, bufferString(data->prototype));
		  }
		} else {
		  strcpy(base_name, bufferString(data->prototype));
		}
		while ( !room_found && 50 >= room_number ) {
		  sprintf(new_room_name, "%s_%d", base_name, room_number);
		  test_room = worldGetType( gameworld, "rproto", get_fullkey(new_room_name, zone_name) );
		  if ( NULL == test_room ) {
		    room_found = 1;
	      }
		  room_number++;
		}
		// int name_length = strlen(new_room_name);
		// log_string("Length of name: %d, Prefix: %d", name_length, strlen(prefix));
		// if ( name_length > strlen(prefix) ) {
		  // log_string("Possible prefix...");
		  // char *cutout = malloc(sizeof(char[strlen(prefix)]));
		  // strncpy(cutout, new_room_name, strlen(prefix));
		  // log_string("Cut: %s - %d, Prefix: %s - %d", cutout, strlen(cutout), prefix, strlen(prefix));
		  // if ( 0 == strcmp(cutout, prefix) ) {
		    // log_string("Matched. Removing prefix.");
			// strncpy(new_room_name, new_room_name + strlen(prefix), 
				// strlen(new_room_name) - strlen(prefix));
			// new_room_name[strlen(new_room_name) - strlen(prefix)] = '\0';
		  // } else {
		    // log_string("No prefix");
		  // }
		  // free(cutout);
		// }
		roomOLCSetKey(olc, get_fullkey(new_room_name, zone_name));
		RESET_LIST *resets = NULL;
		resets = worldGetType(gameworld, "reset",  get_fullkey(bufferString(data->prototype), zone_name));
		if(resets != NULL) {
	      resetListCopyTo(resets, roomOLCGetResets(olc));
	    } else {
	      resetListSetKey(roomOLCGetResets(olc), get_fullkey(bufferString(data->prototype), zone_name));
		}
		roomOLCSetAbstract(olc, FALSE);
		// Saving room to world
		save_room_olc(olc);
		send_to_char(ch, "You create room %s.\r\n", get_fullkey(new_room_name, zone_name));
		char dig_action[SMALL_BUFFER];
		sprintf(dig_action, "dig %s %s", get_fullkey(new_room_name, zone_name), dir);
		do_cmd(ch, dig_action, TRUE);
		do_cmd(ch, dir, TRUE);
	  }
	} else {
	  log_string("%s is NOT allowed to edit this zone.", charGetName(ch));
	}
  }
}

COMMAND(cmd_buildwalk) {
  BUILDWALK_AUX_DATA *data = charGetAuxiliaryData(ch, "buildwalk_aux_data");
  if ( 0 != strcmp("", arg) ) {
    bufferClear(data->prototype);
	bufferCat(data->prototype, arg);
	send_to_char(ch, "Now buildwalking with prototype: %s", bufferString(data->prototype));
  } else {
    bufferClear(data->prototype);
	send_to_char(ch, "Buildwalk turned off.");
  }
}

// Initialize Buildwalk module
void init_buildwalk(void) {
  // Provide auxiliary data
  auxiliariesInstall("buildwalk_aux_data",
    newAuxiliaryFuncs(AUXILIARY_TYPE_CHAR,
    newBuildwalkAuxData, deleteBuildwalkAuxData,
    copyBuildwalkAuxDataTo, copyBuildwalkAuxData,
    storeBuildwalkAuxData, readBuildwalkAuxData));
	
  // Add the required command
  add_cmd("buildwalk", NULL, cmd_buildwalk, "builder", FALSE);
	
  // Hook into movement
  hookAdd("try_buildwalk", tryBuildwalk);
}