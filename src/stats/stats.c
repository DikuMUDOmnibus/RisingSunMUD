#include <Python.h>

#include "../mud.h"
#include "../save.h"
#include "../character.h"
#include "../storage.h"
#include "../auxiliary.h"
#include "../hooks.h"
#include "../set_val/set_val.h"
#include "../scripts/pychar.h"

#include "stats.h"
#include "../descriptions/descriptions.h"

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
  log_string("new STAT_DATA");
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
  if (data->health) store_int(storage, "health", data->health);
  if (data->fatigue) store_int(storage, "fatigue", data->fatigue);
  if (data->strength) store_int(storage, "strength", data->strength);
  if (data->endurance) store_int(storage, "endurance", data->endurance);
  if (data->speed) store_int(storage, "speed", data->speed);
  if (data->agility) store_int(storage, "agility", data->agility);
  if (data->focus) store_int(storage, "focus", data->focus);
  if (data->willpower) store_int(storage, "willpower", data->willpower);
  if (data->wisdom) store_int(storage, "wisdom", data->wisdom);
  if (data->charisma) store_int(storage, "charisma", data->charisma);
  return storage;
}

// Read STAT_AUX_DATA from STORAGE_SET
// Normally named statAuxDataRead(*STORAGE_SET)
STAT_AUX_DATA *readStatAuxData(STORAGE_SET *storage) {
  STAT_AUX_DATA *data = malloc(sizeof(STAT_AUX_DATA));
  if (read_int(storage, "health")) data->health = read_int(storage, "health");
  if (read_int(storage, "fatigue")) data->fatigue = read_int(storage, "fatigue");
  if (read_int(storage, "strength")) data->strength = read_int(storage, "strength");
  if (read_int(storage, "endurance")) data->endurance = read_int(storage, "endurance");
  if (read_int(storage, "speed")) data->speed = read_int(storage, "speed");
  if (read_int(storage, "agility")) data->agility = read_int(storage, "agility");
  if (read_int(storage, "focus")) data->focus = read_int(storage, "focus");
  if (read_int(storage, "willpower")) data->willpower = read_int(storage, "willpower");
  if (read_int(storage, "charisma")) data->charisma = read_int(storage, "charisma");
  if (read_int(storage, "wisdom")) data->wisdom = read_int(storage, "wisdom");
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

// This should be defined in pychar.h, but we don't want to change their files
// Maybe we will if this keeps being needed
typedef struct {
  PyObject_HEAD
  int uid;
} PyChar;

// Python getter and setters
// Getters
PyObject *pyCharGetStrength(PyChar *self, void *closure) {
  CHAR_DATA *ch = PyChar_AsChar((PyObject *)self);
  PyObject *strength = NULL;
  if(ch != NULL) strength = Py_BuildValue("i", charGetStrength(ch));
  return strength;
}

PyObject *pyCharGetEndurance(PyChar *self, void *closure) {
  CHAR_DATA *ch = PyChar_AsChar((PyObject *)self);
  PyObject *endurance = NULL;
  if(ch != NULL) endurance = Py_BuildValue("i", charGetEndurance(ch));
  return endurance;
}

PyObject *pyCharGetSpeed(PyChar *self, void *closure) {
  CHAR_DATA *ch = PyChar_AsChar((PyObject *)self);
  PyObject *speed = NULL;
  if(ch != NULL) speed = Py_BuildValue("i", charGetSpeed(ch));
  return speed;
}

PyObject *pyCharGetAgility(PyChar *self, void *closure) {
  CHAR_DATA *ch = PyChar_AsChar((PyObject *)self);
  PyObject *agility = NULL;
  if(ch != NULL) agility = Py_BuildValue("i", charGetAgility(ch));
  return agility;
}

PyObject *pyCharGetWillpower(PyChar *self, void *closure) {
  CHAR_DATA *ch = PyChar_AsChar((PyObject *)self);
  PyObject *willpower = NULL;
  if(ch != NULL) willpower = Py_BuildValue("i", charGetWillpower(ch));
  return willpower;
}

PyObject *pyCharGetFocus(PyChar *self, void *closure) {
  CHAR_DATA *ch = PyChar_AsChar((PyObject *)self);
  PyObject *focus = NULL;
  if(ch != NULL) focus = Py_BuildValue("i", charGetFocus(ch));
  return focus;
}

PyObject *pyCharGetWisdom(PyChar *self, void *closure) {
  CHAR_DATA *ch = PyChar_AsChar((PyObject *)self);
  PyObject *wisdom = NULL;
  if(ch != NULL) wisdom = Py_BuildValue("i", charGetWisdom(ch));
  return wisdom;
}

PyObject *pyCharGetCharisma(PyChar *self, void *closure) {
  CHAR_DATA *ch = PyChar_AsChar((PyObject *)self);
  PyObject *charisma = NULL;
  if(ch != NULL) charisma = Py_BuildValue("i", charGetCharisma(ch));
  return charisma;
}

PyObject *pyCharGetHealth(PyChar *self, void *closure) {
  CHAR_DATA *ch = PyChar_AsChar((PyObject *)self);
  PyObject *health = NULL;
  if(ch != NULL) health = Py_BuildValue("i", charGetHealth(ch));
  return health;
}

PyObject *pyCharGetMaxHealth(PyChar *self, void *closure) {
  CHAR_DATA *ch = PyChar_AsChar((PyObject *)self);
  PyObject *max_health = NULL;
  if(ch != NULL) max_health = Py_BuildValue("i", charGetMaxHealth(ch));
  return max_health;
}

PyObject *pyCharGetFatigue(PyChar *self, void *closure) {
  CHAR_DATA *ch = PyChar_AsChar((PyObject *)self);
  PyObject *fatigue = NULL;
  if(ch != NULL) fatigue = Py_BuildValue("i", charGetFatigue(ch));
  return fatigue;
}

PyObject *pyCharGetMaxFatigue(PyChar *self, void *closure) {
  CHAR_DATA *ch = PyChar_AsChar((PyObject *)self);
  PyObject *max_fatigue = NULL;
  if(ch != NULL) max_fatigue = Py_BuildValue("i", charGetMaxFatigue(ch));
  return max_fatigue;
}

PyObject *pyCharGetStrengthWord(PyChar *self, void *closure) {
  CHAR_DATA *ch = PyChar_AsChar((PyObject *)self);
  PyObject *strength_word = NULL;
  if(ch != NULL) strength_word = Py_BuildValue("z", charGetStrengthWord(ch));
  return strength_word;
}

PyObject *pyCharGetEnduranceWord(PyChar *self, void *closure) {
  CHAR_DATA *ch = PyChar_AsChar((PyObject *)self);
  PyObject *endurance_word = NULL;
  if(ch != NULL) endurance_word = Py_BuildValue("z", charGetEnduranceWord(ch));
  return endurance_word;
}

PyObject *pyCharGetSpeedWord(PyChar *self, void *closure) {
  CHAR_DATA *ch = PyChar_AsChar((PyObject *)self);
  PyObject *speed_word = NULL;
  if(ch != NULL) speed_word = Py_BuildValue("z", charGetSpeedWord(ch));
  return speed_word;
}

PyObject *pyCharGetAgilityWord(PyChar *self, void *closure) {
  CHAR_DATA *ch = PyChar_AsChar((PyObject *)self);
  PyObject *agility_word = NULL;
  if(ch != NULL) agility_word = Py_BuildValue("z", charGetAgilityWord(ch));
  return agility_word;
}

PyObject *pyCharGetWillpowerWord(PyChar *self, void *closure) {
  CHAR_DATA *ch = PyChar_AsChar((PyObject *)self);
  PyObject *willpower_word = NULL;
  if(ch != NULL) willpower_word = Py_BuildValue("z", charGetWillpowerWord(ch));
  return willpower_word;
}

PyObject *pyCharGetFocusWord(PyChar *self, void *closure) {
  CHAR_DATA *ch = PyChar_AsChar((PyObject *)self);
  PyObject *focus_word = NULL;
  if(ch != NULL) focus_word = Py_BuildValue("z", charGetFocusWord(ch));
  return focus_word;
}

PyObject *pyCharGetCharismaWord(PyChar *self, void *closure) {
  CHAR_DATA *ch = PyChar_AsChar((PyObject *)self);
  PyObject *charisma_word = NULL;
  if(ch != NULL) charisma_word = Py_BuildValue("z", charGetCharismaWord(ch));
  return charisma_word;
}

PyObject *pyCharGetWisdomWord(PyChar *self, void *closure) {
  CHAR_DATA *ch = PyChar_AsChar((PyObject *)self);
  PyObject *wisdom_word = NULL;
  if(ch != NULL) wisdom_word = Py_BuildValue("z", charGetWisdomWord(ch));
  return wisdom_word;
}

// Setters
void *pyCharSetStrength(PyChar *self, PyObject *value, void *closure) {
  CHAR_DATA *ch = PyChar_AsChar((PyObject *)self);
  if (ch != NULL) {
    charSetStrength(ch, PyInt_AsLong(value));
  }
  return NULL;
}

void *pyCharSetEndurance(PyChar *self, PyObject *value, void *closure) {
  CHAR_DATA *ch = PyChar_AsChar((PyObject *)self);
  if (ch != NULL) {
    charSetEndurance(ch, PyInt_AsLong(value));
  }
  return NULL;
}

void *pyCharSetSpeed(PyChar *self, PyObject *value, void *closure) {
  CHAR_DATA *ch = PyChar_AsChar((PyObject *)self);
  if (ch != NULL) {
    charSetSpeed(ch, PyInt_AsLong(value));
  }
  return NULL;
}

void *pyCharSetAgility(PyChar *self, PyObject *value, void *closure) {
  CHAR_DATA *ch = PyChar_AsChar((PyObject *)self);
  if (ch != NULL) {
    charSetAgility(ch, PyInt_AsLong(value));
  }
  return NULL;
}

void *pyCharSetWillpower(PyChar *self, PyObject *value, void *closure) {
  CHAR_DATA *ch = PyChar_AsChar((PyObject *)self);
  if (ch != NULL) {
    charSetWillpower(ch, PyInt_AsLong(value));
  }
  return NULL;
}

void *pyCharSetFocus(PyChar *self, PyObject *value, void *closure) {
  CHAR_DATA *ch = PyChar_AsChar((PyObject *)self);
  if (ch != NULL) {
    charSetFocus(ch, PyInt_AsLong(value));
  }
  return NULL;
}

void *pyCharSetWisdom(PyChar *self, PyObject *value, void *closure) {
  CHAR_DATA *ch = PyChar_AsChar((PyObject *)self);
  if (ch != NULL) {
    charSetWisdom(ch, PyInt_AsLong(value));
  }
  return NULL;
}

void *pyCharSetCharisma(PyChar *self, PyObject *value, void *closure) {
  CHAR_DATA *ch = PyChar_AsChar((PyObject *)self);
  if (ch != NULL) {
    charSetCharisma(ch, PyInt_AsLong(value));
  }
  return NULL;
}

void *pyCharSetHealth(PyChar *self, PyObject *value, void *closure) {
  CHAR_DATA *ch = PyChar_AsChar((PyObject *)self);
  if (ch != NULL) {
    charSetHealth(ch, PyInt_AsLong(value));
  }
  return NULL;
}

void *pyCharSetMaxHealth(PyChar *self, PyObject *value, void *closure) {
  CHAR_DATA *ch = PyChar_AsChar((PyObject *)self);
  if (ch != NULL) {
    charSetMaxHealth(ch, PyInt_AsLong(value));
  }
  return NULL;
}

void *pyCharSetMaxFatigue(PyChar *self, PyObject *value, void *closure) {
  CHAR_DATA *ch = PyChar_AsChar((PyObject *)self);
  if (ch != NULL) {
    charSetMaxFatigue(ch, PyInt_AsLong(value));
  }
  return NULL;
}

void *pyCharSetFatigue(PyChar *self, PyObject *value, void *closure) {
  CHAR_DATA *ch = PyChar_AsChar((PyObject *)self);
  if (ch != NULL) {
    charSetFatigue(ch, PyInt_AsLong(value));
  }
  return NULL;
}

void rollStatsForChar(CHAR_DATA *ch) { // If we add rerolls to character generator, this will move to Python
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
  // TODO Fit these to rules
  
  // Set bonuses based on body type.
  const char *body_type = charGetBodyType(ch);
  // This many string comparisons may be slow, perhaps consider doing this in Python anyway
  if ( 0 == strcmp("athletic", body_type) ) {
    charSetSpeed(ch, charGetSpeed(ch) + 5);
	charSetAgility(ch, charGetAgility(ch) + 5);
	charSetCharisma(ch, charGetCharisma(ch) + 5);
  }
  if ( 0 == strcmp("average", body_type) ) {
    charSetFocus(ch, charGetFocus(ch) + 10);
  }
  if ( 0 == strcmp("bony", body_type) ) {
    charSetStrength(ch, charGetStrength(ch) - 10);
  }
  if ( 0 == strcmp("burly", body_type) ) {
    charSetEndurance(ch, charGetEndurance(ch) + 10);
  }
  if ( 0 == strcmp("chubby", body_type) ) {
    charSetSpeed(ch, charGetSpeed(ch) - 10);
	charSetAgility(ch, charGetAgility(ch) - 10);
  }
  if ( 0 == strcmp("curvaceous", body_type) ) {
    charSetCharisma(ch, charGetCharisma(ch) + 10);
  }
  if ( 0 == strcmp("heavy", body_type) ) {
    charSetSpeed(ch, charGetSpeed(ch) - 10);
  }
  if ( 0 == strcmp("large", body_type) ) {
    charSetSpeed(ch, charGetSpeed(ch) - 5);
	charSetStrength(ch, charGetStrength(ch) + 10);
  }
  if ( 0 == strcmp("lean", body_type) ) {
    charSetSpeed(ch, charGetSpeed(ch) + 5);
	charSetCharisma(ch, charGetCharisma(ch) + 5);
  }
  if ( 0 == strcmp("muscular", body_type) ) {
    charSetEndurance(ch, charGetEndurance(ch) + 5);
	charSetCharisma(ch, charGetCharisma(ch) + 5);
	charSetStrength(ch, charGetStrength(ch) + 10);
  }
  if ( 0 == strcmp("rugged", body_type) ) {
    charSetEndurance(ch, charGetEndurance(ch) + 10);
	charSetCharisma(ch, charGetCharisma(ch) - 5);
  }
  if ( 0 == strcmp("skinny", body_type) ) {
    charSetFocus(ch, charGetFocus(ch) + 5);
  }
  if ( 0 == strcmp("slender", body_type) ) {
    charSetAgility(ch, charGetAgility(ch) + 5);
	charSetCharisma(ch, charGetCharisma(ch) + 5);
  }
  
  // Set bonuses based on clan
  if ( 0 == strcmp("izu", charGetRace(ch)) ) {
    charSetSpeed(ch, charGetSpeed(ch) + 10);
  }
  if ( 0 == strcmp("kai", charGetRace(ch)) ) {
    charSetWisdom(ch, charGetWisdom(ch) + 10);
  }
  if ( 0 == strcmp("hida", charGetRace(ch)) ) {
    charSetEndurance(ch, charGetEndurance(ch) + 10);
  }
  if ( 0 == strcmp("kasuza", charGetRace(ch)) ) {
    charSetFocus(ch, charGetFocus(ch) + 10);
  }
  if ( 0 == strcmp("akita", charGetRace(ch)) ) {
    charSetAgility(ch, charGetAgility(ch) + 10);
  }
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
  
  // Add Python getters and setters to PyChar object
  PyChar_addGetSetter("strength", pyCharGetStrength, pyCharSetStrength, "Manages character's strength");
  PyChar_addGetSetter("endurance", pyCharGetEndurance, pyCharSetEndurance, "Manages character's endurance");
  PyChar_addGetSetter("speed", pyCharGetSpeed, pyCharSetSpeed, "Manages character's speed");
  PyChar_addGetSetter("agility", pyCharGetAgility, pyCharSetAgility, "Manages character's agility");
  PyChar_addGetSetter("willpower", pyCharGetWillpower, pyCharSetWillpower, "Manages character's willpower");
  PyChar_addGetSetter("focus", pyCharGetFocus, pyCharSetFocus, "Manages character's focus");
  PyChar_addGetSetter("charisma", pyCharGetCharisma, pyCharSetCharisma, "Manages character's charisma");
  PyChar_addGetSetter("wisdomw", pyCharGetWisdom, pyCharSetWisdom, "Manages character's wisdom");
  PyChar_addGetSetter("max_health", pyCharGetMaxHealth, pyCharSetMaxHealth, "Manages character's maximum health");
  PyChar_addGetSetter("health", pyCharGetHealth, pyCharSetHealth, "Manages character's health");
  PyChar_addGetSetter("max_fatigue", pyCharGetMaxFatigue, pyCharSetMaxFatigue, "Manages character's maximum fatigue");
  PyChar_addGetSetter("fatigue", pyCharGetFatigue, pyCharSetFatigue, "Manages character's fatigue");
  PyChar_addGetSetter("strength_word", pyCharGetStrengthWord, NULL, "Displays character's strength word");
  PyChar_addGetSetter("endurance_word", pyCharGetEnduranceWord, NULL, "Displays character's endurance word");
  PyChar_addGetSetter("speed_word", pyCharGetSpeedWord, NULL, "Displays character's speed word");
  PyChar_addGetSetter("agility_word", pyCharGetAgilityWord, NULL, "Displays character's agility word");
  PyChar_addGetSetter("willpower_word", pyCharGetWillpowerWord, NULL, "Displays character's willpower word");
  PyChar_addGetSetter("focus_word", pyCharGetFocusWord, NULL, "Displays character's focus word");
  PyChar_addGetSetter("charisma_word", pyCharGetCharismaWord, NULL, "Displays character's charisma word");
  PyChar_addGetSetter("wisdom_word", pyCharGetWisdomWord, NULL, "Displays character's wisdom word");
  
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