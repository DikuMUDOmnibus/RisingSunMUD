#include "../mud.h"
#include "../character.h"
#include "../save.h"
#include "../stats/stats.h"
#include "../auxiliary.h"
#include "../action.h"
#include "../storage.h"
#include "../set_val/set_val.h"

#include "basic_combat.h"

typedef struct {
  int busy;
} COMBAT_AUX_DATA;

// Provide COMBAT_AUX_DATA functions
// Create new COMBAT_AUX_DATA
COMBAT_AUX_DATA *newCombatAuxData() {
  COMBAT_AUX_DATA *aux_data = malloc(sizeof(COMBAT_AUX_DATA));
  aux_data->busy = 0;
  return aux_data;
}

// Delete a COMBAT_AUX_DATA
void deleteCombatAuxData(COMBAT_AUX_DATA *data) {
}

// Copy one COMBAT_AUX_DATA to another
// Normally named CombatAuxDataCopyTo(*COMBAT_AUX_DATA, *COMBAT_AUX_DATA)
void copyCombatAuxData(COMBAT_AUX_DATA *from, COMBAT_AUX_DATA *to) { // TODO Check that from values exist
  to->busy = from->busy;
}

// Duplicate a COMBAT_AUX_DATA
// Normally named CombatAuxDataCopy(*COMBAT_AUX_DATA)
COMBAT_AUX_DATA *duplicateCombatAuxData(COMBAT_AUX_DATA *from) {
  COMBAT_AUX_DATA *to = malloc(sizeof(COMBAT_AUX_DATA));
  copyCombatAuxData(from, to);
  return to;
}
 
// Store COMBAT_AUX_DATA in STORAGE_SET
// Normally named CombatAuxDataStore(*COMBAT_AUX_DATA)
STORAGE_SET *storeCombatAuxData(COMBAT_AUX_DATA *data) {
  STORAGE_SET *storage = new_storage_set();
  store_int(storage, "busy", data->busy);
  return storage;
}

// Read COMBAT_AUX_DATA from STORAGE_SET
// Normally named CombatAuxDataRead(*STORAGE_SET)
COMBAT_AUX_DATA *readCombatAuxData(STORAGE_SET *storage) {
  COMBAT_AUX_DATA *data = malloc(sizeof(COMBAT_AUX_DATA));
  data->busy = read_int(storage, "busy");
  return data;
}

int charIsBusy(CHAR_DATA *ch) {
  COMBAT_AUX_DATA *data = charGetAuxiliaryData(ch, "combat_aux_data");
  return data->busy;
}

void actionSetBusyForChar(CHAR_DATA *ch, int *value, int unused) {
  log_string("Setting busy for %s to %d", charGetName(ch), *value);
  charSetBusy(ch, *value);
}

void charSetBusy(CHAR_DATA *ch, int value) {
  COMBAT_AUX_DATA *data = charGetAuxiliaryData(ch, "combat_aux_data");
  data->busy = value;
  log_string("%s now has a busy value of %d", charGetName(ch), value);
}

void actionInterupt(CHAR_DATA *owner, int data, const char *arg) {
  send_to_char(owner, "You are busy.\r\n");
}

int damageChar(CHAR_DATA *ch, int amount) {
  charSetHealth(ch, charGetHealth(ch) - 10);
  return 10;
}

COMMAND(cmd_attack) {
  CHAR_DATA *target = get_player(arg);
  log_string("%s is attempting to attack %s.", charGetName(ch), arg);
  if ( target && !charIsBusy(ch) ) {
    log_string("%s is attacking %s!", charGetName(ch), charGetName(target));
    damageChar(target, 10);
    send_to_char(ch, "You attack %s.\n\r", charGetName(target));
    send_to_char(target, "%s visciously attacks you!\r\n", charGetName(ch));
	int duration = 5;
	// TODO FINISH THIS
	charSetBusy(ch, 1);
	int notBusy = 0;
	start_action(ch, duration SECONDS, 1, actionSetBusyForChar, actionInterupt, &notBusy, NULL);
  } else if ( target && charIsBusy(ch) )  {
    send_to_char(ch, "You are busy.");
  } else {
    send_to_char(ch, "Who?");
  }
}

void init_basic_combat(void) {
  auxiliariesInstall("combat_aux_data",
    newAuxiliaryFuncs(AUXILIARY_TYPE_CHAR,
    newCombatAuxData, deleteCombatAuxData,
    copyCombatAuxData, duplicateCombatAuxData,
    storeCombatAuxData, readCombatAuxData));
	
  add_set("busy", SET_CHAR, SET_TYPE_INT, charSetBusy, NULL);
	
  add_cmd("attack", NULL, cmd_attack, "player", FALSE);
}