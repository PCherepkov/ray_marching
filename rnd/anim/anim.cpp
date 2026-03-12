#include "anim.h"

void anim::processInput() {
	if (!window)
		return;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		ani.exit = true; // glfwSetWindowShouldClose(window, true);
	double newMX, newMY;
	glfwGetCursorPos(window, &newMX, &newMY);
	x = newMX, y = newMY;
}