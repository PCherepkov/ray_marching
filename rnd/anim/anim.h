#pragma once

#include "../../def.h"
#include "../shaders/shader.h"
#include "../camera.h"

class anim;

extern anim ani;

class anim {
public:
	GLFWwindow* window;
	int w, h;
	double x, y;
	bool is_fullscreen, exit;
	camera cam;
	shader shd;
	float Delta;

	anim() {
		window = nullptr;
		cam.Dir = vec3(0, 0, -1);
		cam.Up = vec3(0, 1, 0);
		cam.Right = vec3(1, 0, 0);
		cam.Loc = vec3(0);
	}

	~anim() {
	}

	void processInput();
};
