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

	shape shapes[12];

	shape S1, S2, S3, P1;
	S1.data.r = 1;
	S1.data.center = vec3(0, 0, -2);
	S1.color = vec3(0, 1, 0);
	S1.type = SPHERE;
	ani.addShape(S1);

	S2.data.r = 0.5;
	S2.data.center = vec3(0, 1, -2);
	S2.color = vec3(0, 0, 1);
	S2.type = SPHERE;
	ani.addShape(S2);

	P1.data.N = normalize(vec3(0, 0.5, 0));
	P1.data.D = 1.5;
	P1.type = PLANE;
	P1.color = vec3(0.8, 0.1, 0.105);
	ani.addShape(P1);

	// shapes[0] = S1;
	// shapes[1] = S2;
	// shapes[2] = P1;

	S3.color = vec3(0.3, 0.04, 0.6);
	S3.type = 1;
	S3.data.center = vec3(3, 0, -2);
	S3.data.r = 1.5;
	ani.addShape(S3);

	// uint ssbo;
	
	// glGenBuffers(1, &ssbo);
	// glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
	// glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(shapes), shapes, GL_DYNAMIC_COPY);
	// glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, ssbo);
	// glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	ani.applyShapes();
	return;
}


void render(GLFWwindow* window) {
	glClear(GL_COLOR_BUFFER_BIT);
	// glBindFramebuffer(GL_FRAMEBUFFER, 0);
	float t = (flt)glfwGetTime(); // (float)clock() / CLK_TCK;
	ani.shd.SetUniform("time", shader::FLT, &t);

	// ani.applyShapes();

	glUseProgram(ani.shd.prg);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glUseProgram(0);
	return;
}
