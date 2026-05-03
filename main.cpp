#include <stdio.h>
#include <stdlib.h>
// #include <windows.h>

// #include "RND/frame.h"
#include "RND/render.h"
#include "main.h"

anim ani;


void SetWindow(int w, int h, bool is_full_screen) {
    if (ani.window != nullptr)
        glfwDestroyWindow(ani.window);

    if (is_full_screen)
        ani.window = glfwCreateWindow(w, h, "SDF", glfwGetPrimaryMonitor(), NULL);
    else
        ani.window = glfwCreateWindow(w, h, "SDF", NULL, NULL);

    if (ani.window == nullptr)
        return;

    glfwMakeContextCurrent(ani.window);
    glfwSetFramebufferSizeCallback(ani.window, Reshape);
    glfwSetScrollCallback(ani.window, [](GLFWwindow* window, double xoffset, double yoffset) { ani.scroll = yoffset; });
    glfwSetKeyCallback(ani.window, ani.keyCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        return;

    glViewport(0, 0, w, h);

    ani.w = w, ani.h = h;
    glfwGetCursorPos(ani.window, &ani.x, &ani.y);
    renderInit(ani.window);
}


void Reshape(GLFWwindow* window, int w, int h) {
    ani.w = w;
    ani.h = h;
    ivec2 resolution = ivec2(w, h);
    ani.shd.SetUniform("ScreenResolution", shader::IVEC2, &(resolution));
    ani.postproc.SetUniform("ScreenResolution", shader::IVEC2, &(resolution));
    ani.createFrameBuffer();
}


int main() {
    int k = glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_SAMPLES, 4);

    SetWindow(1920 / 2, 1080 / 2, false);

    glClearColor(0.3f, 0.47f, 0.8f, 1.0f);
    glEnable(GL_MULTISAMPLE);
    // glEnable(GL_FRAMEBUFFER_SRGB);
    // glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapInterval(0);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.Fonts->AddFontFromFileTTF("./fonts/Roboto-Medium.ttf");
    io.Fonts->AddFontFromFileTTF("./fonts/JetBrainsMono-Regular.ttf");

    ImGui_ImplGlfw_InitForOpenGL(ani.window, true);
    ImGui_ImplOpenGL3_Init();

    float time = 0;

 
    while (!glfwWindowShouldClose(ani.window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ani.processInput();
        render(ani.window);

        ImGui::ShowDemoWindow();
        
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(ani.window);

        ani.Delta = (float)glfwGetTime() - time;
        time += ani.Delta;
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}
