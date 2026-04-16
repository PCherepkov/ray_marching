#pragma once

#include "../../def.h"
#include "../shaders/shader.h"
#include "../camera.h"
#include "../ray/shape.h"


class anim;

extern anim ani;


class anim {
	bool update;
	vector<shape> shapes;
public:
	GLFWwindow* window;
	int w, h;
	double x, y, scroll;
	bool is_fullscreen, exit, saved = false;
	camera cam;
	shader shd;
	float Delta;
	string path = "";

	anim() {
		window = nullptr;
		cam.Dir = vec3(0, 0, -1);
		cam.Up = vec3(0, 1, 0);
		cam.Right = vec3(1, 0, 0);
		cam.Loc = vec3(0);
		cam.speed = 4.0;
	}

	~anim() {
	}

	void processInput();
	static void keyCallback(GLFWwindow* wnd, int key, int scancode, int action, int mods);

	void addShape(shape& s) { shapes.push_back(s); update = true; saved = false; }
	void applyShapes(void);

	void saveShapes(string fpath = "");
	void loadShapes(string fpath);
};
