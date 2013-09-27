#pragma once;
#include "GL\freeglut.h"

struct MouseState {
	bool buttons[256];
	int x,y;
};

struct InputState {
	bool keyboard[256];
	bool special_keys[256];
	MouseState mouse;
};