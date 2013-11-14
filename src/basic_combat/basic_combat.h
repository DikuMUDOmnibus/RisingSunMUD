#ifndef BASIC_COMBAT_H
#define BASIC_COMBAT_H
  // this function should be called when the MUD first boots up.
  // calling it will initialize the mail module for use.
  void init_basic_combat(void);
  
  void charSetBusy(CHAR_DATA *, int);
#endif // BASIC_COMBAT_H