#include "anim.h"
#include "../../def.h"


void anim::processInput() {
	if (!window)
		return;

	ImGuiIO& io = ImGui::GetIO();
	if (io.WantCaptureKeyboard || io.WantCaptureMouse) {
		double newMX, newMY;
		glfwGetCursorPos(window, &newMX, &newMY);
		x = newMX, y = newMY;
		return;
	}

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		exit = true; // glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_F11) == GLFW_PRESS) {
		; // SetWindow(1920, 1080, true);; // glfwSetWindowShouldClose(window, true);
	}

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

	bool fast = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS;
	cam.speed *= exp((float)scroll / 40);

	if (fast)	cam.speed *= 2;
	cam.Loc += ani.Delta * moveDir * cam.speed;
	if (fast)	cam.speed /= 2;

	shd.SetUniform("cameraPos", shader::VEC3, &cam.Loc);

	double newMX, newMY;
	glfwGetCursorPos(window, &newMX, &newMY);

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
		mat4 rotX = mat4(1), rotY = mat4(1);
		rotX = rotate(mat4(1), (float)radians((newMX - x) / ani.w * 180), vec3(0, 1, 0));
		cam.Dir = vec3(rotX * vec4(ani.cam.Dir, 1.0));
		cam.Right = normalize(cross(ani.cam.Dir, vec3(0, 1, 0)));
		rotY = rotate(mat4(1), (float)radians((newMY - y) / ani.w * 180), ani.cam.Right);
		cam.Dir = vec3(rotY * vec4(ani.cam.Dir, 1.0));
		cam.Up = cross(ani.cam.Right, ani.cam.Dir);
	}

	shd.SetUniform("cameraDir", shader::VEC3, &cam.Dir);
	shd.SetUniform("cameraRight", shader::VEC3, &cam.Right);
	shd.SetUniform("cameraUp", shader::VEC3, &cam.Up);

	x = newMX, y = newMY;
	scroll = 0;
}

void anim::applyShapes(void) {
	uint ssbo;
	size_t num = shapes.size();

	glGenBuffers(1, &ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(shape) * num, shapes.data(), GL_DYNAMIC_COPY);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	shd.SetUniform("shapes_num", shader::UINT, &num);
}


void anim::keyCallback(GLFWwindow* wnd, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
		shape s;
		s.color = vec3(0.3, 0.47, 0.8);
		s.type = SPHERE;
		s.data.center = ani.cam.Loc;
		s.data.r = 1.0;
		ani.addShape(s);
		ani.applyShapes();
	}
}


void anim::saveShapes(string fpath) {
	if (fpath == "") fpath = ani.path;
	if (fpath == "") ani.path = fpath = ("scenes/" + to_string(time(nullptr)) + ".sc");

	size_t format = fpath.rfind('.');
	if (format == -1 || fpath.substr(format, string::npos) != ".sc") fpath += ".sc";

	if (fpath != ani.path) ani.path = fpath;

	ofstream fout(fpath);
	if (!fout.is_open()) { std::cout << "error while trying to save." << '\n'; return; }

	for (size_t i = 0; i < shapes.size(); i++) {
		fout << shapes[i].type << ',';
		fout << shapes[i].color.r << ',' << shapes[i].color.g << ',' << shapes[i].color.b << ',';
		fout << descrToString(shapes[i].data) << '\n';
	}

	fout.close();
	ani.saved = true;
	std::cout << "Saved." << '\n';
}


void anim::loadShapes(string fpath) {
	if (fpath == "") return;
	ifstream fin(fpath);
	if (!fin.is_open()) { std::cout << "error while trying to open." << '\n'; return; }
	// size_t format = fpath.rfind('.');
	// if (fpath.substr(format, string::npos) != ".sc") { cout << "wrong file format." << '\n'; return; }

	ani.path = fpath;
	ani.shapes.clear();
	string buffer;

	while (getline(fin, buffer)) {
		shape S;
		size_t start = 0, end = buffer.find(",");
		S.type = stoi(buffer.substr(start, end - start)), start = end + 1, end = buffer.find(",", start);

		S.color.r = stof(buffer.substr(start, end - start)), start = end + 1, end = buffer.find(",", start);
		S.color.g = stof(buffer.substr(start, end - start)), start = end + 1, end = buffer.find(",", start);
		S.color.b = stof(buffer.substr(start, end - start)), start = end + 1, end = buffer.length();
		S.data = stringToDescr(buffer.substr(start, end - start));
		ani.addShape(S);
	}

	ani.applyShapes();
}
