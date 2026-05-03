#include "render.h"


void renderInit(GLFWwindow* window) {
	ani.shd = shader("rnd/shaders/default/");
	ani.postproc = shader("rnd/shaders/postproc/");

	uint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	float t = (float)clock() / CLOCKS_PER_SEC;
	ani.shd.SetUniform("time", shader::FLT, &t);
	ani.shd.SetUniform("bgColor", shader::VEC3, (void*)glm::value_ptr(vec3(0.47, 0.64, 0.96)));

	uint correct = 1;
	ani.shd.SetUniform("colorCorrection", shader::BOOL, &correct);
	uint shade = 1;
	ani.shd.SetUniform("doShade", shader::BOOL, &shade);
	float fog_coeff = 0.0125f;
	ani.shd.SetUniform("fog_coeff", shader::FLT, &fog_coeff);

	ivec2 resolution = ivec2(ani.w, ani.h);
	ani.shd.SetUniform("ScreenResolution", shader::IVEC2, &(resolution));
	ani.postproc.SetUniform("ScreenResolution", shader::IVEC2, &(resolution));

	uint doAA = 1;
	ani.postproc.SetUniform("doAA", shader::BOOL, &doAA);

	ani.loadShapes("scenes/default_2.sc");

	ani.createFrameBuffer();

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

	glBindFramebuffer(GL_FRAMEBUFFER, ani.framebuffer);
	glUseProgram(ani.shd.prg);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	// post processing
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// glBlitFramebuffer(0, 0, ani.w / 2, ani.h / 2, 0, 0, ani.w, ani.h, GL_COLOR_BUFFER_BIT, GL_LINEAR);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(ani.postproc.prg);
	glBindTexture(GL_TEXTURE_2D, ani.colorbuffer);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glUseProgram(0);
	return;
}
