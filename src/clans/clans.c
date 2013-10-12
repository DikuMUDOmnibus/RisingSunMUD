#include "../mud.h"
#include "../body.h"
#include "../races.h"

#include "clans.h"

// Initialize the clan module
void init_clans(void) {
  // Make the default human body
  BODY_DATA *body = newBody();
  bodySetSize(body, BODYSIZE_MEDIUM);
  bodyAddPosition(body, "right grip",              BODYPOS_HELD,          0);
  bodyAddPosition(body, "left grip",               BODYPOS_HELD,          0);
  bodyAddPosition(body, "right foot",              BODYPOS_RIGHT_FOOT,    2);
  bodyAddPosition(body, "left foot",               BODYPOS_LEFT_FOOT,     2);
  bodyAddPosition(body, "right leg",               BODYPOS_LEG,           9);
  bodyAddPosition(body, "left leg",                BODYPOS_LEG,           9);
  bodyAddPosition(body, "waist",                   BODYPOS_WAIST,         1);
  bodyAddPosition(body, "right finger",            BODYPOS_FINGER,        1);
  bodyAddPosition(body, "left finger",             BODYPOS_FINGER,        1);
  bodyAddPosition(body, "right hand",              BODYPOS_RIGHT_HAND,    2);
  bodyAddPosition(body, "left hand",               BODYPOS_LEFT_HAND,     2);
  bodyAddPosition(body, "right wrist",             BODYPOS_WRIST,         1);
  bodyAddPosition(body, "left wrist",              BODYPOS_WRIST,         1);
  bodyAddPosition(body, "right arm",               BODYPOS_ARM,           7);
  bodyAddPosition(body, "left arm",                BODYPOS_ARM,           7);
  bodyAddPosition(body, "about body",              BODYPOS_ABOUT,         0);
  bodyAddPosition(body, "torso",                   BODYPOS_TORSO,        50);
  bodyAddPosition(body, "neck",                    BODYPOS_NECK,          1);
  bodyAddPosition(body, "right ear",               BODYPOS_EAR,           0);
  bodyAddPosition(body, "left ear",                BODYPOS_EAR,           0);
  bodyAddPosition(body, "face",                    BODYPOS_FACE,          2);
  bodyAddPosition(body, "head",                    BODYPOS_HEAD,          2);
  bodyAddPosition(body, "floating about head",     BODYPOS_FLOAT,         0);
  //                                                                  ------
  //                                                                    100
  
  add_race("Izu", "izu", body, TRUE);
  add_race("Kai", "kai", body, TRUE);
  add_race("Hida", "hid", body, TRUE);
  add_race("Kazusa", "kaz", body, TRUE);
  add_race("Akita", "aki", body, TRUE);
  add_race("Immortal", "imm", body, FALSE);
}