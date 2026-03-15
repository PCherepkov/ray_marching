#include "render.h"

void renderInit(GLFWwindow* window) {
	ani.shd = shader("rnd/shaders/DEFAULT/");
	uint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	float t = clock() / CLOCKS_PER_SEC;
	ani.shd.SetUniform("time", shader::FLT, &t);

	ivec2 resolution = ivec2(ani.w, ani.h);
	ani.shd.SetUniform("ScreenResolution", shader::IVEC2, &(resolution));
	return;
}


void render(GLFWwindow* window) {
	glClear(GL_COLOR_BUFFER_BIT);
	// glBindFramebuffer(GL_FRAMEBUFFER, 0);
	float t = (flt)glfwGetTime(); // (float)clock() / CLK_TCK;
	ani.shd.SetUniform("time", shader::FLT, &t);

	glUseProgram(ani.shd.prg);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glUseProgram(0);
	return;
}
