#include <Python.h>

#include "../mud.h"
#include "../save.h"
#include "../character.h"
#include "../storage.h"
#include "../auxiliary.h"
#include "../set_val/set_val.h"
#include "../hooks.h"
#include "../scripts/pychar.h"
#include "../scripts/scripts.h"
#include "../buffer.h"

#include "descriptions.h"

// Define our data structure
typedef struct {
  BUFFER *height;
  BUFFER *body_type;
  BUFFER *complexion;
  BUFFER *eye_color;
  BUFFER *hair_color;
  BUFFER *hair_length;
  BUFFER *hair_texture;
  BUFFER *face;
  BUFFER *nose;
  BUFFER *lips;
  BUFFER *chin;
} DESC_AUX_DATA;

// Provide DESC_AUX_DATA functions
// Create a new DESC_AUX_DATA
DESC_AUX_DATA *newDescAuxData() {
  DESC_AUX_DATA *data = malloc(sizeof(DESC_AUX_DATA));
  data->height = newBuffer(10);
  data->eye_color = newBuffer(1);
  data->complexion = newBuffer(1);
  data->body_type = newBuffer(1);
  data->hair_color = newBuffer(1);
  data->hair_length = newBuffer(1);
  data->hair_texture = newBuffer(1);
  data->face = newBuffer(1);
  data->nose = newBuffer(1);
  data->lips = newBuffer(1);
  data->chin = newBuffer(1);
  return data;
}

// Delete a DESC_AUX_DATA
void deleteDescAuxData(DESC_AUX_DATA *data) {
  // TODO Re-enable memory cleaning
  //if(data->height) free(data->height);
  // if(data->body_type) free(data->body_type);
  // if(data->complexion) free(data->complexion);
  // if(data->eye_color) free(data->eye_color);
  // if(data->hair_color) free(data->hair_color);
  // if(data->hair_length) free(data->hair_length);
  // if(data->hair_texture) free(data->hair_texture);
  // if(data->face) free(data->face);
  // if(data->nose) free(data->nose);
  // if(data->lips) free(data->lips);
  // if(data->chin) free(data->chin);
  free(data);
}

// Copy one DESC_AUX_DATA to another
void copyDescAuxDataTo(DESC_AUX_DATA *from, DESC_AUX_DATA *to) { // TODO Check that values exist
  to->height = from->height;
  to->body_type = from->body_type;
  to->complexion = from->complexion;
  to->eye_color = from->eye_color;
  to->hair_color = from->hair_color;
  to->hair_length = from->hair_length;
  to->hair_texture = from->hair_texture;
  to->face = from->face;
  to->nose = from->nose;
  to->lips = from->lips;
  to->chin = from->chin;
}

// Duplicate a DESC_AUX_DATA
DESC_AUX_DATA *copyDescAuxData(DESC_AUX_DATA *from) {
  DESC_AUX_DATA *to = newDescAuxData();
  copyDescAuxDataTo(from, to);
  return to;
}

// Store a DESC_AUX_DATA
STORAGE_SET *storeDescAuxData(DESC_AUX_DATA *data) {
  STORAGE_SET *storage = new_storage_set();
  if (data->height) store_string(storage, "height", bufferString(data->height));
  if (data->body_type) store_string(storage, "body_type", bufferString(data->body_type));
  if (data->complexion) store_string(storage, "complexion", bufferString(data->complexion));
  if (data->eye_color) store_string(storage, "eye_color", bufferString(data->eye_color));
  if (data->hair_color) store_string(storage, "hair_color", bufferString(data->hair_color));
  if (data->hair_length) store_string(storage, "hair_length", bufferString(data->hair_length));
  if (data->hair_texture) store_string(storage, "hair_texture", bufferString(data->hair_texture));
  if (data->face) store_string(storage, "face", bufferString(data->face));
  if (data->nose) store_string(storage, "nose", bufferString(data->nose));
  if (data->lips) store_string(storage, "lips", bufferString(data->lips));
  if (data->chin) store_string(storage, "chin", bufferString(data->chin));
  return storage;
}

// Read a DESC_AUX_DATA from a STORAGE_SET
DESC_AUX_DATA *readDescAuxData(STORAGE_SET *storage) {
  DESC_AUX_DATA *data = newDescAuxData();
  if (read_string(storage, "height")) {
    bufferClear(data->height);
	bufferCat(data->height, read_string(storage, "height"));
  }
  if (read_string(storage, "body_type")) {
    bufferClear(data->body_type);
    bufferCat(data->body_type, read_string(storage, "body_type"));
  }
  if (read_string(storage, "complexion")) {
    bufferClear(data->complexion);
    bufferCat(data->complexion, read_string(storage, "complexion"));
  }
  if (read_string(storage, "eye_color")) {
    bufferClear(data->eye_color);
    bufferCat(data->eye_color, read_string(storage, "eye_color"));
  }
  if (read_string(storage, "hair_color")) {
    bufferClear(data->hair_color);
    bufferCat(data->hair_color, read_string(storage, "hair_color"));
  }
  if (read_string(storage, "hair_length")) {
    bufferClear(data->hair_length);
    bufferCat(data->hair_length, read_string(storage, "hair_length"));
  }
  if (read_string(storage, "hair_texture")) {
    bufferClear(data->hair_texture);
    bufferCat(data->hair_texture, read_string(storage, "hair_texture"));
  }
  if (read_string(storage, "face")) {
    bufferClear(data->face);
    bufferCat(data->face, read_string(storage, "face"));
  }
  if (read_string(storage, "nose")) {
    bufferClear(data->nose);
    bufferCat(data->nose, read_string(storage, "nose"));
  }
  if (read_string(storage, "lips")) {
    bufferClear(data->lips);
    bufferCat(data->lips, read_string(storage, "lips"));
  }
  if (read_string(storage, "chin")) {
    bufferClear(data->chin);
    bufferCat(data->chin, read_string(storage, "chin"));
  }
  return data;
}

// Getters and setters
const char *charGetEyeColor(CHAR_DATA *ch) {
  DESC_AUX_DATA *desc = charGetAuxiliaryData(ch, "desc_aux_data");
  return bufferString(desc->eye_color);
}

void charSetEyeColor(CHAR_DATA *ch, const char *color) {
  DESC_AUX_DATA *desc = charGetAuxiliaryData(ch, "desc_aux_data");
  bufferClear(desc->eye_color);
  bufferCat(desc->eye_color, color);
  save_player(ch);
}

const char *charGetHeight(CHAR_DATA *ch) {
  DESC_AUX_DATA *desc = charGetAuxiliaryData(ch, "desc_aux_data");
  return bufferString(desc->height);
}

void charSetHeight(CHAR_DATA *ch, const char *height) {
  DESC_AUX_DATA *desc = charGetAuxiliaryData(ch, "desc_aux_data");
  bufferClear(desc->height);
  bufferCat(desc->height, height);
  save_player(ch);
}

const char *charGetBodyType(CHAR_DATA *ch) {
  DESC_AUX_DATA *desc = charGetAuxiliaryData(ch, "desc_aux_data");
  return bufferString(desc->body_type);
}

void charSetBodyType(CHAR_DATA *ch, const char *type) {
  DESC_AUX_DATA *desc = charGetAuxiliaryData(ch, "desc_aux_data");
  bufferClear(desc->body_type);
  bufferCat(desc->body_type, type);
  save_player(ch);
}

const char *charGetComplexion(CHAR_DATA *ch) {
  DESC_AUX_DATA *desc = charGetAuxiliaryData(ch, "desc_aux_data");
  return bufferString(desc->complexion);
}

void charSetComplexion(CHAR_DATA *ch, const char *complexion) {
  DESC_AUX_DATA *desc = charGetAuxiliaryData(ch, "desc_aux_data");
  bufferClear(desc->complexion);
  bufferCat(desc->complexion, complexion);
  save_player(ch);
}

const char *charGetHairColor(CHAR_DATA *ch) {
  DESC_AUX_DATA *desc = charGetAuxiliaryData(ch, "desc_aux_data");
  return bufferString(desc->hair_color);
}

const char *charGetHairLength(CHAR_DATA *ch) {
  DESC_AUX_DATA *desc = charGetAuxiliaryData(ch, "desc_aux_data");
  return bufferString(desc->hair_length);
}

const char *charGetHairTexture(CHAR_DATA *ch) {
  DESC_AUX_DATA *desc = charGetAuxiliaryData(ch, "desc_aux_data");
  return bufferString(desc->hair_texture);
}

const char *charGetFace(CHAR_DATA *ch) {
  DESC_AUX_DATA *desc = charGetAuxiliaryData(ch, "desc_aux_data");
  return bufferString(desc->face);
}

const char *charGetNose(CHAR_DATA *ch) {
  DESC_AUX_DATA *desc = charGetAuxiliaryData(ch, "desc_aux_data");
  return bufferString(desc->nose);
}

const char *charGetLips(CHAR_DATA *ch) {
  DESC_AUX_DATA *desc = charGetAuxiliaryData(ch, "desc_aux_data");
  return bufferString(desc->lips);
}

const char *charGetChin(CHAR_DATA *ch) {
  DESC_AUX_DATA *desc = charGetAuxiliaryData(ch, "desc_aux_data");
  return bufferString(desc->chin);
}

void charSetHairColor(CHAR_DATA *ch, const char *color) {
  DESC_AUX_DATA *desc = charGetAuxiliaryData(ch, "desc_aux_data");
  bufferClear(desc->hair_color);
  bufferCat(desc->hair_color, color);
  save_player(ch);
}

void charSetHairLength(CHAR_DATA *ch, const char *length) {
  DESC_AUX_DATA *desc = charGetAuxiliaryData(ch, "desc_aux_data");
  bufferClear(desc->hair_length);
  bufferCat(desc->hair_length, length);
  save_player(ch);
}

void charSetHairTexture(CHAR_DATA *ch, const char *texture) {
  DESC_AUX_DATA *desc = charGetAuxiliaryData(ch, "desc_aux_data");
  bufferClear(desc->hair_texture);
  bufferCat(desc->hair_texture, texture);
  save_player(ch);
}

void charSetFace(CHAR_DATA *ch, const char *face) {
  DESC_AUX_DATA *desc = charGetAuxiliaryData(ch, "desc_aux_data");
  bufferClear(desc->face);
  bufferCat(desc->face, face);
  save_player(ch);
}

void charSetNose(CHAR_DATA *ch, const char *nose) {
  DESC_AUX_DATA *desc = charGetAuxiliaryData(ch, "desc_aux_data");
  bufferClear(desc->nose);
  bufferCat(desc->nose, nose);
  save_player(ch);
}

void charSetLips(CHAR_DATA *ch, const char *lips) {
  DESC_AUX_DATA *desc = charGetAuxiliaryData(ch, "desc_aux_data");
  bufferClear(desc->lips);
  bufferCat(desc->lips, lips);
  save_player(ch);
}

void charSetChin(CHAR_DATA *ch, const char *chin) {
  DESC_AUX_DATA *desc = charGetAuxiliaryData(ch, "desc_aux_data");
  bufferClear(desc->chin);
  bufferCat(desc->chin, chin);
  save_player(ch);
}

// This should be defined in pychar.h, but we don't want to change their files
// Maybe we will if this keeps being needed
typedef struct {
  PyObject_HEAD
  int uid;
} PyChar;

// Make Python getters and setters
// TODO Error checking and validation
PyObject *pyCharGetHeight(PyChar *self, void *closure) {
  CHAR_DATA *ch = PyChar_AsChar((PyObject *)self);
  PyObject *height = NULL;
  if(ch != NULL) height = Py_BuildValue("z", charGetHeight(ch));
  return height;
}

PyObject *pyCharGetBodyType(PyChar *self, void *closure) {
  CHAR_DATA *ch = PyChar_AsChar((PyObject *)self);
  PyObject *body_type = NULL;
  if (ch != NULL) body_type = Py_BuildValue("z", charGetBodyType(ch));
  return body_type;
}

PyObject *pyCharGetComplexion(PyChar *self, void *closure) {
  CHAR_DATA *ch = PyChar_AsChar((PyObject *)self);
  PyObject *complexion = NULL;
  if (ch != NULL) complexion = Py_BuildValue("z", charGetComplexion(ch));
  return complexion;
}

PyObject *pyCharGetEyeColor(PyChar *self, void *closure) {
  CHAR_DATA *ch = PyChar_AsChar((PyObject *)self);
  PyObject *eye_color = NULL;
  if (ch != NULL) eye_color = Py_BuildValue("z", charGetEyeColor(ch));
  return eye_color;
}

PyObject *pyCharGetHairColor(PyChar *self, void *closure) {
  CHAR_DATA *ch = PyChar_AsChar((PyObject *)self);
  PyObject *hair_color = NULL;
  if (ch != NULL) hair_color = Py_BuildValue("z", charGetHairColor(ch));
  return hair_color;
}

PyObject *pyCharGetHairTexture(PyChar *self, void *closure) {
  CHAR_DATA *ch = PyChar_AsChar((PyObject *)self);
  PyObject *hair_texture = NULL;
  if (ch != NULL) hair_texture = Py_BuildValue("z", charGetHairTexture(ch));
  return hair_texture;
}

PyObject *pyCharGetHairLength(PyChar *self, void *closure) {
  CHAR_DATA *ch = PyChar_AsChar((PyObject *)self);
  PyObject *hair_length = NULL;
  if (ch != NULL) hair_length = Py_BuildValue("z", charGetHairLength(ch));
  return hair_length;
}

PyObject *pyCharGetFace(PyChar *self, void *closure) {
  CHAR_DATA *ch = PyChar_AsChar((PyObject *)self);
  PyObject *face = NULL;
  if (ch != NULL) face = Py_BuildValue("z", charGetFace(ch));
  return face;
}

PyObject *pyCharGetNose(PyChar *self, void *closure) {
  CHAR_DATA *ch = PyChar_AsChar((PyObject *)self);
  PyObject *nose = NULL;
  if (ch != NULL) nose = Py_BuildValue("z", charGetNose(ch));
  return nose;
}

PyObject *pyCharGetLips(PyChar *self, void *closure) {
  CHAR_DATA *ch = PyChar_AsChar((PyObject *)self);
  PyObject *lips = NULL;
  if (ch != NULL) lips = Py_BuildValue("z", charGetLips(ch));
  return lips;
}

PyObject *pyCharGetChin(PyChar *self, void *closure) {
  CHAR_DATA *ch = PyChar_AsChar((PyObject *)self);
  PyObject *chin = NULL;
  if (ch != NULL) chin = Py_BuildValue("z", charGetChin(ch));
  return chin;
}

void *pyCharSetHeight(PyChar *self, PyObject *value, void *closure) {
  CHAR_DATA *ch = PyChar_AsChar((PyObject *)self);
  if (ch != NULL) {
    charSetHeight(ch, PyString_AsString(value));
  }
  return NULL;
}

void *pyCharSetBodyType(PyChar *self, PyObject *value, void *closure) {
  CHAR_DATA *ch = PyChar_AsChar((PyObject *)self);
  if (ch != NULL) {
    charSetBodyType(ch, PyString_AsString(value));
  }
  return NULL;
}

void *pyCharSetComplexion(PyChar *self, PyObject *value, void *closure) {
  CHAR_DATA *ch = PyChar_AsChar((PyObject *)self);
  if (ch != NULL) {
    charSetComplexion(ch, PyString_AsString(value));
  }
  return NULL;
}

void *pyCharSetHairColor(PyChar *self, PyObject *value, void *closure) {
  CHAR_DATA *ch = PyChar_AsChar((PyObject *)self);
  if (ch != NULL) {
    charSetHairColor(ch, PyString_AsString(value));
  }
  return NULL;
}

void *pyCharSetHairLength(PyChar *self, PyObject *value, void *closure) {
  CHAR_DATA *ch = PyChar_AsChar((PyObject *)self);
  if (ch != NULL) {
    charSetHairLength(ch, PyString_AsString(value));
  }
  return NULL;
}

void *pyCharSetHairTexture(PyChar *self, PyObject *value, void *closure) {
  CHAR_DATA *ch = PyChar_AsChar((PyObject *)self);
  if (ch != NULL) {
    charSetHairTexture(ch, PyString_AsString(value));
  }
  return NULL;
}

void *pyCharSetEyeColor(PyChar *self, PyObject *value, void *closure) {
  CHAR_DATA *ch = PyChar_AsChar((PyObject *)self);
  if (ch != NULL) {
    charSetEyeColor(ch, PyString_AsString(value));
  }
  return NULL;
}

void *pyCharSetFace(PyChar *self, PyObject *value, void *closure) {
  CHAR_DATA *ch = PyChar_AsChar((PyObject *)self);
  if (ch != NULL) {
    charSetFace(ch, PyString_AsString(value));
  }
  return NULL;
}

void *pyCharSetNose(PyChar *self, PyObject *value, void *closure) {
  CHAR_DATA *ch = PyChar_AsChar((PyObject *)self);
  if (ch != NULL) {
    charSetNose(ch, PyString_AsString(value));
  }
  return NULL;
}

void *pyCharSetLips(PyChar *self, PyObject *value, void *closure) {
  CHAR_DATA *ch = PyChar_AsChar((PyObject *)self);
  if (ch != NULL) {
    charSetLips(ch, PyString_AsString(value));
  }
  return NULL;
}

void *pyCharSetChin(PyChar *self, PyObject *value, void *closure) {
  CHAR_DATA *ch = PyChar_AsChar((PyObject *)self);
  if (ch != NULL) {
    charSetChin(ch, PyString_AsString(value));
  }
  return NULL;
}

// Hook into character initiation
void initDescHook(const char *info) {
  CHAR_DATA *ch = NULL;
  hookParseInfo(info, &ch);
  //DESC_AUX_DATA *data = charGetAuxiliaryData(ch, "desc_aux_data");
  //char *script_data = "I am";
  // char *description = 
    // "[desc_data = me.getAuxiliary(\"desc_aux_data\")][if desc_data.height==\"average\"]	You see a[else]You see a [desc_data.height][/if] ";
  // charSetDesc(ch, description);
}

// Initiate the module
void init_descriptions() {
    
  // Provide auxiliary data
  auxiliariesInstall("desc_aux_data",
    newAuxiliaryFuncs(AUXILIARY_TYPE_CHAR,
    newDescAuxData, deleteDescAuxData,
    copyDescAuxDataTo, copyDescAuxData,
    storeDescAuxData, readDescAuxData));
	
  // Add the getters and setters to pychar.c
  PyChar_addGetSetter("height", pyCharGetHeight, pyCharSetHeight, "Manages character's height");
  PyChar_addGetSetter("body_type", pyCharGetBodyType, pyCharSetBodyType, "Manages character's body type");
  PyChar_addGetSetter("complexion", pyCharGetComplexion, pyCharSetComplexion, "Manages character's complexion");
  PyChar_addGetSetter("eye_color", pyCharGetEyeColor, pyCharSetEyeColor, "Manages character's eye color");
  PyChar_addGetSetter("hair_color", pyCharGetHairColor, pyCharSetHairColor, "Manages character's hair color");
  PyChar_addGetSetter("hair_length", pyCharGetHairLength, pyCharSetHairLength, "Manages character's hair length");
  PyChar_addGetSetter("hair_texture", pyCharGetHairTexture, pyCharSetHairTexture, "Manages character's hair texture");
  PyChar_addGetSetter("face", pyCharGetFace, pyCharSetFace, "Manages character's face");
  PyChar_addGetSetter("nose", pyCharGetNose, pyCharSetNose, "Manages character's nose");
  PyChar_addGetSetter("lips", pyCharGetLips, pyCharSetLips, "Manages character's lips");
  PyChar_addGetSetter("chin", pyCharGetChin, pyCharSetChin, "Manages character's chin");
	
  // Add "set" fields for these attributes
  add_set("height", SET_CHAR, SET_TYPE_STRING, charSetHeight, NULL);
  add_set("body_type", SET_CHAR, SET_TYPE_STRING, charSetBodyType, NULL);
  add_set("complexion", SET_CHAR, SET_TYPE_STRING, charSetComplexion, NULL);
  add_set("eye_color", SET_CHAR, SET_TYPE_STRING, charSetEyeColor, NULL);
  add_set("hair_color", SET_CHAR, SET_TYPE_STRING, charSetHairColor, NULL);
  add_set("hair_length", SET_CHAR, SET_TYPE_STRING, charSetHairLength, NULL);
  add_set("hair_texture", SET_CHAR, SET_TYPE_STRING, charSetHairTexture, NULL);
  add_set("face", SET_CHAR, SET_TYPE_STRING, charSetFace, NULL);
  add_set("nose", SET_CHAR, SET_TYPE_STRING, charSetNose, NULL);
  add_set("lips", SET_CHAR, SET_TYPE_STRING, charSetLips, NULL);
  add_set("chin", SET_CHAR, SET_TYPE_STRING, charSetChin, NULL);
  
  // Hook into the character initiation
  //hookAdd("init_player", initDescHook);
}