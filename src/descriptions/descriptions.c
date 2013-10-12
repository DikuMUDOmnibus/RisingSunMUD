#include "../mud.h"
#include "../save.h"
#include "../character.h"
#include "../storage.h"
#include "../auxiliary.h"
#include "../set_val/set_val.h"
#include "../hooks.h"
#include "../buffer.h"

#include "descriptions.h"

// Define our data structure
typedef struct {
  const char *height;
  const char *body_type;
  const char *complexion;
  const char *eye_color;
  const char *hair_color;
  const char *hair_length;
  const char *hair_texture;
  const char *face;
  const char *nose;
  const char *lips;
  const char *chin;
} DESC_AUX_DATA;

// Provide DESC_AUX_DATA functions
// Create a new DESC_AUX_DATA
DESC_AUX_DATA *newDescAuxData() {
  DESC_AUX_DATA *data = malloc(sizeof(DESC_AUX_DATA));
  data->height = "average";
  data->eye_color = "brown";
  data->complexion = "";
  data->body_type = "average";
  data->hair_color = "black";
  data->hair_length = "short";
  data->hair_texture = "slick";
  data->face = "round";
  data->nose = "small";
  data->lips = "pouting";
  data->chin = "square";
  return data;
}

// Delete a DESC_AUX_DATA
void deleteDescAuxData(DESC_AUX_DATA *data) {
  // Does not performing this cause a memory leak?
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
  DESC_AUX_DATA *to = malloc(sizeof(DESC_AUX_DATA));
  copyDescAuxDataTo(from, to);
  return to;
}

// Store a DESC_AUX_DATA
STORAGE_SET *storeDescAuxData(DESC_AUX_DATA *data) {
  STORAGE_SET *storage = new_storage_set();
  if (data->height) store_string(storage, "height", data->height);
  if (data->body_type) store_string(storage, "body_type", data->body_type);
  if (data->complexion) store_string(storage, "complexion", data->complexion);
  if (data->eye_color) store_string(storage, "eye_color", data->eye_color);
  if (data->hair_color) store_string(storage, "hair_color", data->hair_color);
  if (data->hair_length) store_string(storage, "hair_length", data->hair_length);
  if (data->hair_texture) store_string(storage, "hair_texture", data->hair_texture);
  if (data->face) store_string(storage, "face", data->face);
  if (data->nose) store_string(storage, "nose", data->nose);
  if (data->lips) store_string(storage, "lips", data->lips);
  if (data->chin) store_string(storage, "chin", data->chin);
  return storage;
}

// Read a DESC_AUX_DATA from a STORAGE_SET
DESC_AUX_DATA *readDescAuxData(STORAGE_SET *storage) {
  DESC_AUX_DATA *data = malloc(sizeof(DESC_AUX_DATA));
  if (read_string(storage, "height")) data->height = read_string(storage, "height");
  if (read_string(storage, "body_type")) data->body_type = read_string(storage, "body_type");
  if (read_string(storage, "complexion")) data->complexion = read_string(storage, "complexion");
  if (read_string(storage, "eye_color")) data->eye_color = read_string(storage, "eye_color");
  if (read_string(storage, "hair_color")) data->hair_color = read_string(storage, "hair_color");
  if (read_string(storage, "hair_length")) data->hair_length = read_string(storage, "hair_length");
  if (read_string(storage, "hair_texture")) data->hair_texture = read_string(storage, "hair_texture");
  if (read_string(storage, "face")) data->face = read_string(storage, "face");
  if (read_string(storage, "nose")) data->nose = read_string(storage, "nose");
  if (read_string(storage, "lips")) data->lips = read_string(storage, "lips");
  if (read_string(storage, "chin")) data->chin = read_string(storage, "chin");
  return data;
}

// Getters and setters
const char *charGetEyeColor(CHAR_DATA *ch) {
  DESC_AUX_DATA *desc = charGetAuxiliaryData(ch, "desc_aux_data");
  return desc->eye_color;
}

void charSetEyeColor(CHAR_DATA *ch, const char *color) {
  DESC_AUX_DATA *desc = charGetAuxiliaryData(ch, "desc_aux_data");
  desc->eye_color = color;
  save_player(ch);
}

const char *charGetHeight(CHAR_DATA *ch) {
  DESC_AUX_DATA *desc = charGetAuxiliaryData(ch, "desc_aux_data");
  return desc->height;
}

void charSetHeight(CHAR_DATA *ch, const char *height) {
  DESC_AUX_DATA *desc = charGetAuxiliaryData(ch, "desc_aux_data");
  desc->height = height;
  save_player(ch);
}

const char *charGetBodyType(CHAR_DATA *ch) {
  DESC_AUX_DATA *desc = charGetAuxiliaryData(ch, "desc_aux_data");
  return desc->body_type;
}

void charSetBodyType(CHAR_DATA *ch, const char *type) {
  DESC_AUX_DATA *desc = charGetAuxiliaryData(ch, "desc_aux_data");
  desc->body_type = type;
  save_player(ch);
}

const char *charGetComplexion(CHAR_DATA *ch) {
  DESC_AUX_DATA *desc = charGetAuxiliaryData(ch, "desc_aux_data");
  return desc->complexion;
}

void charSetComplexion(CHAR_DATA *ch, const char *complexion) {
  DESC_AUX_DATA *desc = charGetAuxiliaryData(ch, "desc_aux_data");
  desc->complexion = complexion;
  save_player(ch);
}

const char *charGetHairColor(CHAR_DATA *ch) {
  DESC_AUX_DATA *desc = charGetAuxiliaryData(ch, "desc_aux_data");
  return desc->hair_color;
}

const char *charGetHairLength(CHAR_DATA *ch) {
  DESC_AUX_DATA *desc = charGetAuxiliaryData(ch, "desc_aux_data");
  return desc->hair_length;
}

const char *charGetHairTexture(CHAR_DATA *ch) {
  DESC_AUX_DATA *desc = charGetAuxiliaryData(ch, "desc_aux_data");
  return desc->hair_texture;
}

const char *charGetFace(CHAR_DATA *ch) {
  DESC_AUX_DATA *desc = charGetAuxiliaryData(ch, "desc_aux_data");
  return desc->face;
}

const char *charGetNose(CHAR_DATA *ch) {
  DESC_AUX_DATA *desc = charGetAuxiliaryData(ch, "desc_aux_data");
  return desc->nose;
}

const char *charGetLips(CHAR_DATA *ch) {
  DESC_AUX_DATA *desc = charGetAuxiliaryData(ch, "desc_aux_data");
  return desc->lips;
}

const char *charGetChin(CHAR_DATA *ch) {
  DESC_AUX_DATA *desc = charGetAuxiliaryData(ch, "desc_aux_data");
  return desc->chin;
}

void charSetHairColor(CHAR_DATA *ch, const char *color) {
  DESC_AUX_DATA *desc = charGetAuxiliaryData(ch, "desc_aux_data");
  desc->hair_color = color;
  save_player(ch);
}

void charSetHairLength(CHAR_DATA *ch, const char *length) {
  DESC_AUX_DATA *desc = charGetAuxiliaryData(ch, "desc_aux_data");
  desc->hair_length = length;
  save_player(ch);
}

void charSetHairTexture(CHAR_DATA *ch, const char *texture) {
  DESC_AUX_DATA *desc = charGetAuxiliaryData(ch, "desc_aux_data");
  desc->hair_texture = texture;
  save_player(ch);
}

void charSetFace(CHAR_DATA *ch, const char *face) {
  DESC_AUX_DATA *desc = charGetAuxiliaryData(ch, "desc_aux_data");
  desc->face = face;
  save_player(ch);
}

void charSetNose(CHAR_DATA *ch, const char *nose) {
  DESC_AUX_DATA *desc = charGetAuxiliaryData(ch, "desc_aux_data");
  desc->nose = nose;
  save_player(ch);
}

void charSetLips(CHAR_DATA *ch, const char *lips) {
  DESC_AUX_DATA *desc = charGetAuxiliaryData(ch, "desc_aux_data");
  desc->lips = lips;
  save_player(ch);
}

void charSetChin(CHAR_DATA *ch, const char *chin) {
  DESC_AUX_DATA *desc = charGetAuxiliaryData(ch, "desc_aux_data");
  desc->chin = chin;
  save_player(ch);
}

// Hook into character initiation
void initDescHook(const char *info) {
  log_string("Initializing new character description");
  CHAR_DATA *ch = NULL;
  hookParseInfo(info, &ch);
  DESC_AUX_DATA *data = charGetAuxiliaryData(ch, "desc_aux_data");
  char *script_data = "I am";
  char *description = "[ch.name]";
  charSetDesc(ch, description);
}

// Initiate the module
void init_descriptions() {
    
  // Provide auxiliary data
  auxiliariesInstall("desc_aux_data",
    newAuxiliaryFuncs(AUXILIARY_TYPE_CHAR,
    newDescAuxData, deleteDescAuxData,
    copyDescAuxDataTo, copyDescAuxData,
    storeDescAuxData, readDescAuxData));
	
  // Add "set" fields for these attributes
  add_set("height", SET_CHAR, SET_TYPE_INT, charSetHeight, NULL);
  add_set("body_type", SET_CHAR, SET_TYPE_INT, charSetBodyType, NULL);
  add_set("complexion", SET_CHAR, SET_TYPE_INT, charSetComplexion, NULL);
  add_set("eye_color", SET_CHAR, SET_TYPE_INT, charSetEyeColor, NULL);
  add_set("hair_color", SET_CHAR, SET_TYPE_INT, charSetHairColor, NULL);
  add_set("hair_length", SET_CHAR, SET_TYPE_INT, charSetHairLength, NULL);
  add_set("hair_texture", SET_CHAR, SET_TYPE_INT, charSetHairTexture, NULL);
  add_set("face", SET_CHAR, SET_TYPE_INT, charSetFace, NULL);
  add_set("nose", SET_CHAR, SET_TYPE_INT, charSetNose, NULL);
  add_set("lips", SET_CHAR, SET_TYPE_INT, charSetLips, NULL);
  add_set("chin", SET_CHAR, SET_TYPE_INT, charSetChin, NULL);
  
  // Hook into the character initiation
  hookAdd("init_player", initDescHook);
}