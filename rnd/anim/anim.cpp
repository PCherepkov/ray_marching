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
		moveDir += cam.Dir;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		moveDir += -cam.Right;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		moveDir += -cam.Dir;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		moveDir += cam.Right;
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		moveDir += cam.Up;
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		moveDir += -cam.Up;

	ani.cam.Loc += ani.Delta * moveDir;
	ani.shd.SetUniform("cameraPos", shader::VEC3, &ani.cam.Loc);

	double newMX, newMY;
	glfwGetCursorPos(window, &newMX, &newMY);

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
		mat4 rotX = mat4(1), rotY = mat4(1);
		rotX = rotate(mat4(1), (float)radians((newMX - x) / ani.w * 180), vec3(0, 1, 0));
		ani.cam.Dir = vec3(rotX * vec4(ani.cam.Dir, 1.0));
		ani.cam.Right = normalize(cross(ani.cam.Dir, vec3(0, 1, 0)));
		rotY = rotate(mat4(1), (float)radians((newMY - y) / ani.w * 180), ani.cam.Right);
		ani.cam.Dir = vec3(rotY * vec4(ani.cam.Dir, 1.0));
		ani.cam.Up = cross(ani.cam.Right, ani.cam.Dir);
	}

	ani.shd.SetUniform("cameraDir", shader::VEC3, &ani.cam.Dir);
	ani.shd.SetUniform("cameraRight", shader::VEC3, &ani.cam.Right);
	ani.shd.SetUniform("cameraUp", shader::VEC3, &ani.cam.Up);

	x = newMX, y = newMY;
}