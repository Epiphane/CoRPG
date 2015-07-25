#ifndef KEYS_H
#define KEYS_H 

#include <ncurses.h>


const int K_LEFT = 
#ifdef __APPLE__
	'D'
#else
	KEY_LEFT
#endif
;

const int K_DOWN = 
#ifdef __APPLE__
	'B'
#else
	KEY_DOWN
#endif
;

const int K_UP = 
#ifdef __APPLE__
	'A'
#else
	KEY_UP
#endif
;

const int K_RIGHT = 
#ifdef __APPLE__
	'C'
#else
	KEY_RIGHT
#endif
;

#endif /* KEYS_H */
