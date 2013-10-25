#ifndef DESCRIPTIONS_H
#define DESCRIPTIONS_H
// this function should be called when the MUD first boots up.
// calling it will initialize the mail module for use.
void init_descriptions(void);
// TODO Prototype all the functions provided by descriptions.c

struct DESC_AUX_DATA;

const char *charGetBodyType(CHAR_DATA *);

#endif // DESCRIPTIONS_H