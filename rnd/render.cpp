#include "render.h"


void renderInit(GLFWwindow* window) {
	ani.shd = shader("rnd/shaders/DEFAULT/");
	uint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	float t = (float)clock() / CLOCKS_PER_SEC;
	ani.shd.SetUniform("time", shader::FLT, &t);
	ani.shd.SetUniform("bgColor", shader::VEC3, (void*)glm::value_ptr(vec3(0.47, 0.64, 0.96)));

	uint correct = 1;
	ani.shd.SetUniform("colorCorrection", shader::BOOL, &correct);
	float fog_coeff = 0.0125f;
	ani.shd.SetUniform("fog_coeff", shader::FLT, &fog_coeff);

	ivec2 resolution = ivec2(ani.w, ani.h);
	ani.shd.SetUniform("ScreenResolution", shader::IVEC2, &(resolution));

	ani.loadShapes("scenes/default_2.sc");
	return;
}


void renderGUI(void) {
	mainTopBar();
	mainOverlay();
	if (ani.gui.open_help_flag) helpInfo();
}


void render(GLFWwindow* window) {
	glClear(GL_COLOR_BUFFER_BIT);
	float t = (flt)glfwGetTime();
	ani.shd.SetUniform("time", shader::FLT, &t);

	renderGUI();

	glUseProgram(ani.shd.prg);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glUseProgram(0);
	return;
}
