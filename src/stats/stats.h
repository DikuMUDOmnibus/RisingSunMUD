#ifndef STATS_H
#define STATS_H
// this function should be called when the MUD first boots up.
// calling it will initialize the mail module for use.
void init_stats(void);

// This function sets character's health
void charSetHealth(const CHAR_DATA *, int);

int charGetHealth(const CHAR_DATA *);
#endif // STATS_H