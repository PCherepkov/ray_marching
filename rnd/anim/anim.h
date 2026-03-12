#pragma once

#include "../../def.h"
#include "../shaders/shader.h"

class anim;

extern anim ani;

class anim {
public:
	GLFWwindow* window;
	int w, h;
	double x, y;
	bool is_fullscreen, exit;
	shader shd;

	anim() {
		window = nullptr;
	}

	~anim() {
	}

	void processInput();
};
