#include "anim.h"
#include "../../def.h"


void anim::processInput() {
	if (!window)
		return;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		ani.exit = true; // glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_F11) == GLFW_PRESS)
		; // SetWindow(1920, 1080, true);; // glfwSetWindowShouldClose(window, true);

	vec3 moveDir = vec3(0);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		moveDir += vec3(0, 0, 1);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		moveDir += vec3(-1, 0, 0);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		moveDir += vec3(0, 0, -1);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		moveDir += vec3(1, 0, 0);
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		moveDir += vec3(0, 1, 0);
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		moveDir += vec3(0, -1, 0);

	ani.shd.SetUniform("cameraPos", shader::VEC3, &moveDir);

	double newMX, newMY;
	glfwGetCursorPos(window, &newMX, &newMY);
	x = newMX, y = newMY;
}