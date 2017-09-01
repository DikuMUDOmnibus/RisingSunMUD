#include "../mud.h"
#include "../character.h"
#include "../save.h"
#include "../stats/stats.h"
#include "../auxiliary.h"
#include "../action.h"
#include "../storage.h"
#include "../set_val/set_val.h"
#include "../hooks.h"
#include "../event.h"

#include "basic_combat.h"

#define HEAL_PULSE_RATE 5

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
    //COMBAT_AUX_DATA *data = charGetAuxiliaryData(ch, "combat_aux_data");
    //return data->busy;
    return is_acting(ch, 1);
}

void charNoLongerBusy(CHAR_DATA *ch, int unused, int not_used) {
    send_to_char(ch, "{WYou are no longer busy.{n");
}

void actionInterupt(CHAR_DATA *owner, int data, const char *arg) {
    send_to_char(owner, "You are busy.\r\n");
}

int damageChar(CHAR_DATA *ch, int amount) {
    charSetHealth(ch, charGetHealth(ch) - amount);
    //save_player(ch);
    return amount;
}

int healChar(CHAR_DATA *ch, int amount) {
    if (charGetHealth(ch) != charGetMaxHealth(ch)) {
        if (charGetHealth(ch) + amount <= charGetMaxHealth(ch)) {
            charSetHealth(ch, charGetHealth(ch) + amount);
        } else {
            charSetHealth(ch, charGetMaxHealth(ch));
        }
    }
    //save_player(ch);
    return amount;
}

int fatigueChar(CHAR_DATA *ch, int amount) {
    charSetFatigue(ch, charGetFatigue(ch) - amount);
    //save_player(ch);
    return amount;
}

int restChar(CHAR_DATA *ch, int amount) {
    if (charGetFatigue(ch) != charGetMaxFatigue(ch)) {
        if (charGetFatigue(ch) + amount <= charGetMaxFatigue(ch)) {
            charSetFatigue(ch, charGetFatigue(ch) + amount);
        } else {
            charSetFatigue(ch, charGetMaxFatigue(ch));
        }
    }
    //save_player(ch);
    return amount;
}

void event_heal(CHAR_DATA *ch, void *data, const char *arg) {
    int max_health = charGetMaxHealth(ch);
    int pulse_health = floor(max_health * 0.01);
    healChar(ch, pulse_health);
    int max_fatigue = charGetMaxFatigue(ch);
    int pulse_fatigue = floor(max_fatigue * 0.01);
    restChar(ch, pulse_fatigue);
}

void regenStartHook(const char *hook_info) {
    CHAR_DATA *ch = NULL;
    hookParseInfo(hook_info, &ch);
    //log_string("Hooked character entry, starting regen for %s.", charGetName(ch));
    start_update(ch, HEAL_PULSE_RATE SECONDS, event_heal, NULL, NULL, NULL);
}

COMMAND(cmd_attack) {
    char arg_to_parse[SMALL_BUFFER];
    CHAR_DATA *target = NULL;
    strcpy(arg_to_parse, arg);
    if (parse_args(ch, TRUE, "attack", arg_to_parse, "ch.room", &target) && !charIsBusy(ch)) {
        log_string("%s is attacking %s!", charGetName(ch), charGetName(target));
        damageChar(target, 10);
        fatigueChar(ch, 10);
        send_to_char(ch, "You attack %s.\n\r", charGetName(target));
        send_to_char(target, "{r%s visciously attacks you!{n\r\n", charGetName(ch));
        int duration = 5;
        start_action(ch, duration SECONDS, 1, charNoLongerBusy, actionInterupt, NULL, NULL);
    } else if (charIsBusy(ch)) {
        send_to_char(ch, "You are busy.");
    }
}

void init_basic_combat(void) {
    hookAdd("char_to_game", regenStartHook);

    add_cmd("attack", NULL, cmd_attack, "player", FALSE);
}