#include "render.h"


void renderInit(GLFWwindow* window) {
	ani.shd = shader("rnd/shaders/default/");
	ani.postproc = shader("rnd/shaders/postproc/");

	uint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	float t = (float)clock() / CLOCKS_PER_SEC;

	ani.shd.SetUniform("time", shader::FLT, &t);

	ivec2 resolution = ivec2(ani.w, ani.h);
	ani.shd.SetUniform("ScreenResolution", shader::IVEC2, &(resolution));
	ani.postproc.SetUniform("ScreenResolution", shader::IVEC2, &(resolution));

	ani.setBackgroundColor(vec3(0.47, 0.64, 0.96));
	ani.setColorCorrection(true);
	ani.setDoShade(true);
	ani.setFogDensity(0.0125f);
	ani.setDoAA(true);

	ani.setTolerance(1e-3f);
	ani.setInfinity(300);
	ani.setMaxSteps(1000);

	ani.loadShapes("scenes/default_2.sc");

	ani.createFrameBuffer();

	UIInit();

	return;
}


void renderGUI(void) {
	mainTopBar();
	mainOverlay();
	if (ani.gui.open_help_flag) helpInfo();
	if (ani.gui.show_fps_flag) fpsCounter();
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
