#include "../mud.h"
#include "../save.h"
#include "../character.h"
#include "../storage.h"
#include "../auxiliary.h"
#include "../hooks.h"
#include "../set_val/set_val.h"
#include "stats.h"

// Define our data structures
typedef struct {
  int health;
  int fatigue;
  int current_health;
  int current_fatigue;
  int strength;
  int endurance;
  int agility;
  int speed;
  int wisdom;
  int focus;
  int willpower;
  int charisma;
} STAT_DATA;

typedef struct {
  int health;
  int fatigue;
  int current_health;
  int current_fatigue;
  int strength;
  int endurance;
  int agility;
  int speed;
  int wisdom;
  int focus;
  int willpower;
  int charisma;
} STAT_AUX_DATA;

int rand_range(int min_n, int max_n)
{
  return rand() % (max_n - min_n + 1) + min_n;
}

// Provide STAT_DATA functions
// Create a new STAT_DATA
STAT_DATA *newStatData() {
  STAT_DATA *new_stat_data = malloc(sizeof(STAT_DATA));
  return new_stat_data;
}

// Delete a STAT_DATA
void deleteStatData(STAT_DATA *data) {
  free(data);
}

// Copy a STAT_DATA
// Normally named statCopy(*STAT_DATA)
STAT_DATA *copyStatData(STAT_DATA *data) {
  STAT_DATA *new_data = malloc(sizeof(STAT_DATA));
  new_data->health = data->health;
  new_data->fatigue = data->fatigue;
  new_data->strength = data->strength;
  new_data->endurance = data->endurance;
  new_data->speed = data->speed;
  new_data->agility = data->agility;
  new_data->focus = data->focus;
  new_data->willpower = data->willpower;
  new_data->wisdom = data->wisdom;
  new_data->charisma = data->charisma;
  return new_data;
}

// Read a STAT_DATA from a STORAGE_SET
// Normally named statRead(*STORAGE_SET)
STAT_DATA *readStatData(STORAGE_SET *set) {
  STAT_DATA *new_data = malloc(sizeof(STAT_DATA));
  new_data->health = read_int(set, "health");
  new_data->fatigue = read_int(set, "fatigue");
  new_data->strength = read_int(set, "strength");
  new_data->endurance = read_int(set, "endurance");
  new_data->speed = read_int(set, "speed");
  new_data->agility = read_int(set, "agility");
  new_data->focus = read_int(set, "focus");
  new_data->willpower = read_int(set, "willpower");
  new_data->charisma = read_int(set, "charisma");
  new_data->wisdom = read_int(set, "wisdom");
  return new_data;
}

// Store a STAT_DATA to a STORAGE_SET
// Normally named statStore(*STAT_DATA)
STORAGE_SET *storeStatData(STAT_DATA *data) {
  STORAGE_SET *storage = new_storage_set();
  store_int(storage, "health", data->health);
  store_int(storage, "fatigue", data->fatigue);
  store_int(storage, "strength", data->strength);
  store_int(storage, "endurance", data->endurance);
  store_int(storage, "speed", data->speed);
  store_int(storage, "agility", data->agility);
  store_int(storage, "focus", data->focus);
  store_int(storage, "willpower", data->willpower);
  store_int(storage, "wisdom", data->wisdom);
  store_int(storage, "charisma", data->charisma);
  return storage;
}

// Provide STAT_AUX_DATA functions
// Create new STAT_AUX_DATA
STAT_AUX_DATA *newStatAuxData() {
  STAT_AUX_DATA *aux_data = malloc(sizeof(STAT_AUX_DATA));
  return aux_data;
}

// Delete a STAT_AUX_DATA
void deleteStatAuxData(STAT_AUX_DATA *data) {
  free(data);
}

// Copy one STAT_AUX_DATA to another
// Normally named statAuxDataCopyTo(*STAT_AUX_DATA, *STAT_AUX_DATA)
void copyStatAuxData(STAT_AUX_DATA *from, STAT_AUX_DATA *to) { // TODO Check that from values exist
    from->health = to->health;
	from->fatigue = to->fatigue;
	from->strength = to->strength;
	from->endurance = to->endurance;
	from->willpower = to->willpower;
	from->focus = to->focus;
	from->wisdom = to->wisdom;
	from->charisma = to->charisma;
	from->fatigue = to->speed;
	from->agility = to->agility;
}

// Duplicate a STAT_AUX_DATA
// Normally named statAuxDataCopy(*STAT_AUX_DATA)
STAT_AUX_DATA *duplicateStatAuxData(STAT_AUX_DATA *from) {
  STAT_AUX_DATA *to = malloc(sizeof(STAT_AUX_DATA));
  copyStatAuxData(from, to);
  return to;
}
 
// Store STAT_AUX_DATA in STORAGE_SET
// Normally named statAuxDataStore(*STAT_AUX_DATA)
STORAGE_SET *storeStatAuxData(STAT_AUX_DATA *data) {
  STORAGE_SET *storage = new_storage_set();
  store_int(storage, "health", data->health);
  store_int(storage, "fatigue", data->fatigue);
  store_int(storage, "strength", data->strength);
  store_int(storage, "endurance", data->endurance);
  store_int(storage, "speed", data->speed);
  store_int(storage, "agility", data->agility);
  store_int(storage, "focus", data->focus);
  store_int(storage, "willpower", data->willpower);
  store_int(storage, "wisdom", data->wisdom);
  store_int(storage, "charisma", data->charisma);
  return storage;
}

// Read STAT_AUX_DATA from STORAGE_SET
// Normally named statAuxDataRead(*STORAGE_SET)
STAT_AUX_DATA *readStatAuxData(STORAGE_SET *storage) {
  STAT_AUX_DATA *data = malloc(sizeof(STAT_AUX_DATA));
  data->health = read_int(storage, "health");
  data->fatigue = read_int(storage, "fatigue");
  data->strength = read_int(storage, "strength");
  data->endurance = read_int(storage, "endurance");
  data->speed = read_int(storage, "speed");
  data->agility = read_int(storage, "agility");
  data->focus = read_int(storage, "focus");
  data->willpower = read_int(storage, "willpower");
  data->charisma = read_int(storage, "charisma");
  data->wisdom = read_int(storage, "wisdom");
  return data;
}

// Change stat values into word values
const char *getWordFromValue(int val) {
  const char *word_level;
  switch (val) {
    case 1 ... 10:
	  word_level = "Bad";
	  break;
	case 11 ... 25:
	  word_level = "Poor";
	  break;
	case 26 ... 40:
	  word_level = "Below Average";
	  break;
	case 41 ... 60:
	  word_level = "Average";
	  break;
	case 61 ... 75:
	  word_level = "Above Average";
	  break;
	case 76 ... 90:
	  word_level = "Good";
	  break;
	case 91 ... 100:
	  word_level = "Excellent";
	  break;
	default:
	  word_level = "Godly";
  }
  return word_level;
}

// Getters and setters
// This is where basing it on a list would be handy

int charGetMaxHealth(const CHAR_DATA *character) {
  STAT_AUX_DATA *stats = charGetAuxiliaryData(character, "stat_aux_data");
  return stats->health;
}

int charGetHealth(const CHAR_DATA *ch) {
  STAT_AUX_DATA *stats = charGetAuxiliaryData(ch, "stat_aux_data");
  return stats->current_health;
}

int charGetMaxFatigue(const CHAR_DATA *ch) {
  STAT_AUX_DATA *stats = charGetAuxiliaryData(ch, "stat_aux_data");
  return stats->fatigue;
}

int charGetFatigue(const CHAR_DATA *ch) {
  STAT_AUX_DATA *stats = charGetAuxiliaryData(ch, "stat_aux_data");
  return stats->current_fatigue;
}

const char *charGetStrengthWord(const CHAR_DATA *ch) {
  STAT_AUX_DATA *stats = charGetAuxiliaryData(ch, "stat_aux_data");
  return getWordFromValue(stats->strength);
}

int charGetStrength(const CHAR_DATA *ch) {
  STAT_AUX_DATA *stats = charGetAuxiliaryData(ch, "stat_aux_data");
  return stats->strength;
}

const char *charGetEnduranceWord(const CHAR_DATA *ch) {
  STAT_AUX_DATA *stats = charGetAuxiliaryData(ch, "stat_aux_data");
  return getWordFromValue(stats->endurance);
}

int charGetEndurance(const CHAR_DATA *ch) {
  STAT_AUX_DATA *stats = charGetAuxiliaryData(ch, "stat_aux_data");
  return stats->endurance;
}

const char *charGetSpeedWord(const CHAR_DATA *ch) {
  STAT_AUX_DATA *stats = charGetAuxiliaryData(ch, "stat_aux_data");
  return getWordFromValue(stats->speed);
}

int charGetSpeed(const CHAR_DATA *ch) {
  STAT_AUX_DATA *stats = charGetAuxiliaryData(ch, "stat_aux_data");
  return stats->speed;
}

const char *charGetAgilityWord(const CHAR_DATA *ch) {
  STAT_AUX_DATA *stats = charGetAuxiliaryData(ch, "stat_aux_data");
  return getWordFromValue(stats->agility);
}

int charGetAgility(const CHAR_DATA *ch) {
  STAT_AUX_DATA *stats = charGetAuxiliaryData(ch, "stat_aux_data");
  return stats->agility;
}

const char *charGetFocusWord(const CHAR_DATA *ch) {
  STAT_AUX_DATA *stats = charGetAuxiliaryData(ch, "stat_aux_data");
  return getWordFromValue(stats->focus);
}

int charGetFocus(const CHAR_DATA *ch) {
  STAT_AUX_DATA *stats = charGetAuxiliaryData(ch, "stat_aux_data");
  return stats->focus;
}

const char *charGetWillpowerWord(const CHAR_DATA *ch) {
  STAT_AUX_DATA *stats = charGetAuxiliaryData(ch, "stat_aux_data");
  return getWordFromValue(stats->willpower);
}

int charGetWillpower(const CHAR_DATA *ch) {
  STAT_AUX_DATA *stats = charGetAuxiliaryData(ch, "stat_aux_data");
  return stats->willpower;
}

const char *charGetWisdomWord(const CHAR_DATA *ch) {
  STAT_AUX_DATA *stats = charGetAuxiliaryData(ch, "stat_aux_data");
  return getWordFromValue(stats->wisdom);
}

int charGetWisdom(const CHAR_DATA *ch) {
  STAT_AUX_DATA *stats = charGetAuxiliaryData(ch, "stat_aux_data");
  return stats->wisdom;
}

const char *charGetCharismaWord(const CHAR_DATA *ch) {
  STAT_AUX_DATA *stats = charGetAuxiliaryData(ch, "stat_aux_data");
  return getWordFromValue(stats->charisma);
}

int charGetCharisma(const CHAR_DATA *ch) {
  STAT_AUX_DATA *stats = charGetAuxiliaryData(ch, "stat_aux_data");
  return stats->charisma;
}

void charSetMaxHealth(CHAR_DATA *ch, int health) {
  STAT_AUX_DATA *stats = charGetAuxiliaryData(ch, "stat_aux_data");
  stats->health = health;
}

void charSetHealth(const CHAR_DATA *ch, int health) {
  STAT_AUX_DATA *stats = charGetAuxiliaryData(ch, "stat_aux_data");
  if (health > stats->health) {
    stats->current_health = stats->health;
  } else {
    stats->current_health = health;
  }
}

void charSetFatigue(const CHAR_DATA *ch, int fatigue) {
  STAT_AUX_DATA *stats = charGetAuxiliaryData(ch, "stat_aux_data");
  if (fatigue > stats->fatigue) {
    stats->current_fatigue = stats->fatigue;
  } else {
    stats->current_fatigue = fatigue;
  }
}

void charSetMaxFatigue(CHAR_DATA *ch, int fatigue) {
  STAT_AUX_DATA *stats = charGetAuxiliaryData(ch, "stat_aux_data");
  stats->fatigue = fatigue;
}

void charSetStrength(CHAR_DATA *ch, int strength) {
  STAT_AUX_DATA *stats = charGetAuxiliaryData(ch, "stat_aux_data");
  if (1<=strength && 100>=strength){  
	stats->strength = strength;
  } else if (1>strength) {
    stats->strength = 1;
  } else if (100<strength) {
    stats->strength = 100;
  }
}

void charSetEndurance(CHAR_DATA *ch, int endurance) {
  STAT_AUX_DATA *stats = charGetAuxiliaryData(ch, "stat_aux_data");
  if (1<=endurance && 100>=endurance){  
	stats->endurance = endurance;
  } else if (1>endurance) {
    stats->endurance = 1;
  } else if (100<endurance) {
    stats->endurance = 100;
  }
  stats->health = floor(2.5*stats->endurance+123);
  stats->fatigue = floor((100/99)*stats->endurance+49);
}

void charSetSpeed(CHAR_DATA *ch, int speed) {
  STAT_AUX_DATA *stats = charGetAuxiliaryData(ch, "stat_aux_data");
  if (1<=speed && 100>=speed){  
	stats->speed = speed;
  } else if (1>speed) {
    stats->speed = 1;
  } else if (100<speed) {
    stats->speed = 100;
  }
}

void charSetAgility(CHAR_DATA *ch, int agility) {
  STAT_AUX_DATA *stats = charGetAuxiliaryData(ch, "stat_aux_data");
  if (1<=agility && 100>=agility){  
	stats->agility = agility;
  } else if (1>agility) {
    stats->agility = 1;
  } else if (100<agility) {
    stats->agility = 100;
  }
}

void charSetWisdom(CHAR_DATA *ch, int wisdom) {
  STAT_AUX_DATA *stats = charGetAuxiliaryData(ch, "stat_aux_data");
  if (1<=wisdom && 100>=wisdom){  
	stats->wisdom = wisdom;
  } else if (1>wisdom) {
    stats->wisdom = 1;
  } else if (100<wisdom) {
    stats->wisdom = 100;
  }
}

void charSetFocus(CHAR_DATA *ch, int focus) {
  STAT_AUX_DATA *stats = charGetAuxiliaryData(ch, "stat_aux_data");
  if (1<=focus && 100>=focus){  
	stats->focus = focus;
  } else if (1>focus) {
    stats->focus = 1;
  } else if (100<focus) {
    stats->focus = 100;
  }
}

void charSetWillpower(CHAR_DATA *ch, int willpower) {
  STAT_AUX_DATA *stats = charGetAuxiliaryData(ch, "stat_aux_data");
  if (1<=willpower && 100>=willpower){  
	stats->willpower = willpower;
  } else if (1>willpower) {
    stats->willpower = 1;
  } else if (100<willpower) {
    stats->willpower = 100;
  }
}

void charSetCharisma(CHAR_DATA *ch, int charisma) {
  STAT_AUX_DATA *stats = charGetAuxiliaryData(ch, "stat_aux_data");
  if (1<=charisma && 100>=charisma){  
	stats->charisma = charisma;
  } else if (1>charisma) {
    stats->charisma = 1;
  } else if (100<charisma) {
    stats->charisma = 100;
  }
}

void rollStatsForChar(CHAR_DATA *ch) {
  charSetEndurance(ch, rand_range(1, 100));
  charSetHealth(ch, charGetMaxHealth(ch));
  charSetFatigue(ch, charGetMaxFatigue(ch));
  charSetStrength(ch, rand_range(1, 100));
  charSetSpeed(ch, rand_range(1, 100));
  charSetAgility(ch, rand_range(1, 100));
  charSetWisdom(ch, rand_range(1, 100));
  charSetFocus(ch, rand_range(1, 100));
  charSetWillpower(ch, rand_range(1, 100));
  charSetCharisma(ch, rand_range(1, 100));
}

// Define the command to view stats
COMMAND(cmd_stats) {
  //STAT_AUX_DATA *stats = charGetAuxiliaryData(ch, "stat_aux_data");
  //send_to_char(ch, "Stats for %s. \r\n", charGetName(ch));
  send_to_char(ch, "-------------------------------------------------------\r\n");
  send_to_char(ch, "%s of the %s clan\r\n", charGetName(ch), charGetRace(ch));
  send_to_char(ch, "%s at age %s\r\n\r\n", "[marital_status]", "[age]");
  send_to_char(ch, "Health: %d/%d    Fatigue: %d/%d\r\n\r\n", 
    charGetHealth(ch), charGetMaxHealth(ch), charGetFatigue(ch), charGetMaxFatigue(ch));
  send_to_char(ch, "Strength: %s            Endurance:     %s\r\n", 
    charGetStrengthWord(ch), charGetEnduranceWord(ch));
  send_to_char(ch, "Speed:    %s            Agility:       %s\r\n", 
    charGetSpeedWord(ch), charGetAgilityWord(ch));
  send_to_char(ch, "Focus:    %s            Willpower:     %s\r\n", 
    charGetFocusWord(ch), charGetWillpowerWord(ch));
  send_to_char(ch, "Wisdom:   %s            Charisma:      %s\r\n", 
    charGetWisdomWord(ch), charGetCharismaWord(ch));
  send_to_char(ch, "-------------------------------------------------------\r\n");
}

// Set up hooks
void initCharHook(const char *info) {
  CHAR_DATA *ch = NULL;
  hookParseInfo(info, &ch);
  //log_string("Initializing %s", charGetName(ch));
  rollStatsForChar(ch);
}

// Load module
void init_stats(void) {
  log_string("Initializing stats module");
  
  // Provide auxiliary data
  auxiliariesInstall("stat_aux_data",
    newAuxiliaryFuncs(AUXILIARY_TYPE_CHAR,
    newStatAuxData, deleteStatAuxData,
    copyStatAuxData, duplicateStatAuxData,
    storeStatAuxData, readStatAuxData));
	
  // Hook into the character initiation
  hookAdd("init_player", initCharHook);
	
  // Add the commands defined in this module to the game
  add_cmd("stats", NULL, cmd_stats, "player", FALSE);
  
  // Add "set" fields for these attributes
  add_set("max_health", SET_CHAR, SET_TYPE_INT, charSetMaxHealth, NULL);
  add_set("max_fatigue", SET_CHAR, SET_TYPE_INT, charSetMaxFatigue, NULL);
  add_set("strength", SET_CHAR, SET_TYPE_INT, charSetStrength, NULL);
  add_set("endurance", SET_CHAR, SET_TYPE_INT, charSetEndurance, NULL);
  add_set("speed", SET_CHAR, SET_TYPE_INT, charSetSpeed, NULL);
  add_set("agility", SET_CHAR, SET_TYPE_INT, charSetAgility, NULL);
  add_set("willpower", SET_CHAR, SET_TYPE_INT, charSetWillpower, NULL);
  add_set("focus", SET_CHAR, SET_TYPE_INT, charSetFocus, NULL);
  add_set("wisdom", SET_CHAR, SET_TYPE_INT, charSetWisdom, NULL);
  add_set("charisma", SET_CHAR, SET_TYPE_INT, charSetCharisma, NULL);
  add_set("fatigue", SET_CHAR, SET_TYPE_INT, charSetFatigue, NULL);
  add_set("health", SET_CHAR, SET_TYPE_INT, charSetHealth, NULL);
}