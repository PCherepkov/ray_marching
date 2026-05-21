#pragma once

#include "../../def.h"
#include "../shaders/shader.h"
#include "../camera.h"
#include "../ray/shape.h"
#include "../gui/gui.h"


class anim;

extern anim ani;

class anim {
	bool update;
	vector<shape> shapes;
public:
	GUIData gui;
	GLFWwindow* window;
	int w, h;
	double x, y, scroll;
	bool is_fullscreen, saved = false;
	camera cam;
	shader shd, postproc;
	uint framebuffer = 0, colorbuffer = 0;
	float scale = 1;
	uint filtering = GL_NEAREST;

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

	void createFrameBuffer(void);

	void processInput();
	static void keyCallback(GLFWwindow* wnd, int key, int scancode, int action, int mods);

	void addShape(shape& s) { shapes.push_back(s); update = true; saved = false; }
	void applyShapes(void);

	void clearShapes(void) { shapes.clear(); }

	void saveShapes(string fpath = "");
	void loadShapes(string fpath);

	void setBackgroundColor(vec3 color) { shd.SetUniform("bgColor", shader::VEC3, (void*)glm::value_ptr(color)); }
	void setColorCorrection(uint flag) { shd.SetUniform("colorCorrection", shader::BOOL, &flag); }
	void setDoShade(uint flag) { shd.SetUniform("doShade", shader::BOOL, &flag); }
	void setDoAA(uint flag) { postproc.SetUniform("doAA", shader::BOOL, &flag); }
	void setFogDensity(float coeff) { shd.SetUniform("fog_coeff", shader::FLT, &coeff); }

	void setTolerance(float coeff) { shd.SetUniform("tol", shader::FLT, &coeff); }
	void setInfinity(float coeff) { shd.SetUniform("inf", shader::FLT, &coeff); }
	void setMaxSteps(uint cnt) { shd.SetUniform("MAX_STEPS", shader::UINT, &cnt); }

	size_t numOfShapes(void) { return shapes.size(); }
	shape getShape(size_t ind = 0) { return shapes[ind]; }
	vector<shape>* getAllShapes(void) { return &shapes; }
};
