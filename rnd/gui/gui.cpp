#include "gui.h"
#include "../anim/anim.h"

bool transparencyOption;
ImGuiStyle dark_style;


void openScene(void) {
    cout << "Openning file...\n";
    char* outPath = nullptr;
    nfdresult_t result = NFD_OpenDialog("sc", "", &outPath);

    if (result != NFD_OKAY) {
        cout << "Error occured";
        free(outPath);
        return;
    }

    if (outPath) ani.loadShapes(outPath);
    free(outPath);
}


void saveSceneAs(void) {
    cout << "Saving scene...\n";
    char* outPath = nullptr;
    nfdresult_t result = NFD_SaveDialog("sc", "", &outPath);

    if (result != NFD_OKAY) {
        cout << "Error occured";
        free(outPath);
        return;
    }

    if (outPath) ani.saveShapes(outPath);
    free(outPath);
    cout << "Saved as \"" << ani.path << "\"\n";
}


void saveScene(void) {
    cout << "Saving...\n";
    if (ani.saved) return;
    if (ani.path == "") return saveSceneAs();
    ani.saveShapes();
    cout << "Saved!\n";
}


void newScene(void) {
    cout << "Creating new...\n";
    char* outPath = nullptr;
    nfdresult_t result = NFD_SaveDialog("sc", "", &outPath);

    if (result != NFD_OKAY) {
        cout << "Error occured";
        free(outPath);
        return;
    }

    if (outPath) {
        ani.clearShapes();
        ani.applyShapes();
        ani.saveShapes(outPath);
    }

    free(outPath);
    cout << "Created!\n";
}


void helpInfo(void) {
    if (!ImGui::Begin("Help", &ani.gui.open_help_flag, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::End();
        return;
    }
    if (ImGui::BeginPopupContextItem()) {
        if (ImGui::MenuItem("Close help"))
            ani.gui.open_help_flag = false;
        ImGui::EndPopup();
    }

    ImGuiStyle& style = ImGui::GetStyle();
    ImGui::PushFont(NULL, style.FontSizeBase * 1.5f);
    ImGui::Text("Camera position");
    ImGui::PopFont();
    ImGui::Separator();
    if (ImGui::BeginTable("camera controls", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_SizingStretchSame))
    {
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("W");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("move forwards");

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("A");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("move left");

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("S");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("move backwards");

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("D");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("move right");

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("R");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("move up");

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("D");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("move down");

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("mouse scroll");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("change camera speed");

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("home");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("return to (0, 0, 0) coordinates");

        ImGui::EndTable();
    }

    // ImGui::Text("Enter - place a unit sphere on top of the camera");
    
    ImGui::Separator();

    ImGui::PushFont(NULL, style.FontSizeBase * 1.5f);
    ImGui::Text("Shapes in the scene");
    ImGui::PopFont();
    ImGui::Separator();

    if (ImGui::BeginTable("camera controls", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_SizingStretchSame)) {
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("Elements");
        ImGui::TableSetColumnIndex(1);

        ImGui::EndTable();
    }

    static int my_image_width = 0;
    static int my_image_height = 0;
    static GLuint my_image_texture = 0;

    if (!my_image_texture && false) {
        bool ret = LoadTextureFromFile("images/frong.png", &my_image_texture, &my_image_width, &my_image_height);
        IM_ASSERT(ret);
    }

    // ImGui::Text("pointer = %x", my_image_texture);
    // ImGui::Text("size = %d x %d", my_image_width, my_image_height);
    // ImGui::Image((ImTextureID)(intptr_t)my_image_texture, ImVec2(my_image_width, my_image_height));

    ImGui::Text("\"Add shape\" button adds a shape to the scene");
    ImGui::Text("Click on an element inside the list of shapes to expand the properties panel");
    ImGui::Text("X - deletes the shape from the scene");
    ImGui::Text("Arrows - deletes the shape from the scene");
    ImGui::Separator();

    ImGui::End();
}

void fpsCounter(void) {
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    float PAD = 4.0f;

    ImVec2 work_pos = viewport->WorkPos;
    ImVec2 work_size = viewport->WorkSize;
    ImVec2 window_pos, window_pos_pivot;
    window_pos.x = work_pos.x + work_size.x - PAD;
    window_pos.y = work_pos.y + work_size.y - PAD;
    window_pos_pivot.x = 1.0f;
    window_pos_pivot.y = 1.0f;

    ImGuiIO& io = ImGui::GetIO();
    ImGui::SetNextWindowBgAlpha(0.9f);
    ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
    if (ImGui::Begin("fps", 0, ImGuiWindowFlags_AlwaysAutoResize |
        ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoFocusOnAppearing |
        ImGuiWindowFlags_NoNav |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoDecoration)) {
        ImGui::Text("FPS: %0.0f", io.Framerate);
    }
    ImGui::End();
}


void mainTopBar(void) {
    static bool open_help_flag = false;
    static bool fpscnt = false;

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New", "Ctrl+N")) { newScene(); }
            if (ImGui::MenuItem("Open", "Ctrl+O")) { openScene(); }
            if (ImGui::MenuItem("Save", "Ctrl+S")) { saveScene(); }
            if (ImGui::MenuItem("Save As", "LCtrl+Shift+S")) { saveSceneAs(); }
            ImGui::Separator();
            if (ImGui::MenuItem("Exit", "LShift+Esc")) { glfwSetWindowShouldClose(ani.window, true); }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Options")) {
            ImGui::BeginTable("option table", 2, ImGuiTableFlags_NoBordersInBody | ImGuiTableFlags_SizingStretchSame);
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            static vec3 bgColor(0.47, 0.64, 0.96);
            ImGui::Text("Background color");
            ImGui::TableSetColumnIndex(1);
            ImGui::SetNextItemWidth(-1);
            if (ImGui::ColorEdit3("##0", glm::value_ptr(bgColor), ImGuiColorEditFlags_NoPicker & 0)) ani.setBackgroundColor(bgColor);

            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("Color correction");
            ImGui::TableSetColumnIndex(1);
            static bool colorCorrect = 1;
            if (ImGui::Checkbox("##1", &colorCorrect)) ani.setColorCorrection(colorCorrect);

            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("Fog density");
            ImGui::TableSetColumnIndex(1);
            static float fog_coeff = 0.0125f;
            ImGui::SetNextItemWidth(-1);
            if (ImGui::DragFloat("##2", &fog_coeff, 0.0001f, 0, 1, "%.4f", ImGuiSliderFlags_AlwaysClamp)) {
                // fog_coeff = (fog_coeff >= 0) ? fog_coeff : 0;
                ani.setFogDensity(fog_coeff);
            }

            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("Shade");
            ImGui::TableSetColumnIndex(1);
            static bool doShade = 1;
            if (ImGui::Checkbox("##3", &doShade)) ani.setDoShade(doShade);

            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("Anti-alisaing");
            ImGui::TableSetColumnIndex(1);
            static bool doAA = 1;
            if (ImGui::Checkbox("##4", &doAA)) ani.setDoAA(doAA);

            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("Down-scaling");
            ImGui::TableSetColumnIndex(1);
            ImGui::SetNextItemWidth(-1);
            if (ImGui::DragFloat("##5", &ani.scale, 0.5f, 0.5, 16, "%.1f", ImGuiSliderFlags_AlwaysClamp)) {
                // ani.scale = clamp(ani.scale, 0.5f, 16.f);
                ani.createFrameBuffer();
            }

            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("Filtering");
            ImGui::TableSetColumnIndex(1);
            const char* const list[2] = {"Nearest", "Linear"};
            static const char* cur = list[0];
            ImGui::SetNextItemWidth(-1);
            if (ImGui::BeginCombo("##6", cur)) {
                static bool nearest = (cur[0] == 'N'), linear = !nearest;
                if (ImGui::Selectable("Nearest", &nearest, 2)) {
                    cur = list[0];
                    ani.filtering = GL_NEAREST;
                    ani.createFrameBuffer();
                    linear = false;
                }
                if (ImGui::Selectable("Linear", &linear, 2)) {
                    cur = list[1];
                    ani.filtering = GL_LINEAR;
                    ani.createFrameBuffer();
                    nearest = false;
                }
                ImGui::EndCombo();
            }

            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Separator();
            ImGui::TableSetColumnIndex(1);
            ImGui::Separator();

            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("Hit margin");
            ImGui::TableSetColumnIndex(1);
            static float tol = 0.001f;
            ImGui::SetNextItemWidth(-1);
            if (ImGui::DragFloat("##7", &tol, 0.001f, 1e-6f, 0.1f, "%.4f", ImGuiSliderFlags_AlwaysClamp)) {
                ani.setTolerance(tol);
            }

            // uint MAX_STEPS = 1000;

            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("Render distance");
            ImGui::TableSetColumnIndex(1);
            static float infty = 300;
            ImGui::SetNextItemWidth(-1);
            if (ImGui::DragFloat("##8", &infty, 1, 1, 1000, "%.2f", ImGuiSliderFlags_AlwaysClamp)) {
                ani.setInfinity(infty);
            }

            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("Max step count");
            ImGui::TableSetColumnIndex(1);
            static int max_steps = 1000;
            ImGui::SetNextItemWidth(-1);
            if (ImGui::DragInt("##9", &max_steps, 1, 10, 100000, "%d", ImGuiSliderFlags_AlwaysClamp)) {
                ani.setMaxSteps(max_steps);
            }

            ImGui::EndTable();
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Editor"))
        {
            ImGui::BeginTable("alignment", 2, ImGuiTableFlags_NoBordersInBody | ImGuiTableFlags_SizingStretchSame);
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("Font size");
            ImGui::TableSetColumnIndex(1);
            ImGui::SetNextItemWidth(-1);
            static float fontsz = 20;
            if (ImGui::DragFloat("##0", &fontsz, 0.1f, 0, 1000, "%.0f", ImGuiSliderFlags_AlwaysClamp)) {
                ImGuiIO& io = ImGui::GetIO();
                // io.Fonts->AddFontFromFileTTF("./fonts/Roboto-Medium.ttf", fontsz);
                io.FontGlobalScale = (fontsz > 0) ? fontsz/20 : 1;
            }

            const char* const themes[2] = { "Light", "Dark" };
            static const char* theme = themes[1];
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("Theme");
            ImGui::TableSetColumnIndex(1);
            ImGui::SetNextItemWidth(-1);
            if (ImGui::BeginCombo("##1", theme)) {
                static bool light = (theme[0] == 'L'), dark = !light;
                if (ImGui::Selectable("Light", &light, 2)) {
                    theme = themes[0];
                    ImGuiStyle& cur_style = ImGui::GetStyle();
                    ImGui::StyleColorsLight(&cur_style);
                    dark = false;
                }
                if (ImGui::Selectable("Dark", &dark, 2)) {
                    theme = themes[1];
                    ImGuiStyle& cur_style = ImGui::GetStyle();
                    cur_style = dark_style;
                    light = false;
                }
                ImGui::EndCombo();
            }

            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("Show FPS");
            ImGui::TableSetColumnIndex(1);
            ImGui::SetNextItemWidth(-1);
            if (ImGui::Checkbox("##2", &fpscnt)) { ani.gui.show_fps_flag = fpscnt; }

            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("Transparency");
            ImGui::TableSetColumnIndex(1);
            ImGui::SetNextItemWidth(-1);
            ImGui::Checkbox("##3", &transparencyOption);

            ImGui::EndTable();

            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Help"))
        {
            if (ImGui::MenuItem("Show help", "")) { ani.gui.open_help_flag = true; }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    return;
}

void mainOverlay(void) {
    static int location = 0;
    ImGuiIO& io = ImGui::GetIO();
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_AlwaysAutoResize |
                                    ImGuiWindowFlags_NoSavedSettings |
                                    ImGuiWindowFlags_NoFocusOnAppearing |
                                    ImGuiWindowFlags_NoNav |
                                    ImGuiWindowFlags_NoBringToFrontOnFocus;
    ImGuiStyle style = ImGui::GetStyle();

    const float PAD = 0.0f;
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImVec2 work_pos = viewport->WorkPos;
    ImVec2 work_size = viewport->WorkSize;
    ImVec2 window_pos, window_pos_pivot;
    window_pos.x = (location & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
    window_pos.y = (location & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
    window_pos_pivot.x = (location & 1) ? 1.0f : 0.0f;
    window_pos_pivot.y = (location & 2) ? 1.0f : 0.0f;
    ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
    window_flags |= ImGuiWindowFlags_NoMove;

    ImGui::SetNextWindowBgAlpha(0.9f);

    ImGui::SetNextWindowSize(ImVec2(0, viewport->WorkSize.y));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);

    static bool* p_open = 0;

    if (ImGui::Begin("Elements", p_open, window_flags))
    {
        ImGui::Text("Scene elements\n" "(right-click to change position)");
        ImGui::Separator();

        if (ImGui::BeginPopupContextWindow())
        {
            if (ImGui::MenuItem("left", NULL, location == 0)) location = 0;
            if (ImGui::MenuItem("right", NULL, location == 1)) location = 1;
            ImGui::EndPopup();
        }

        vector<string> snames = {"", "Sphere", "Plane", "Box", "Torus", "Cone", "Cylinder", "Ellipsoid"};
        vector<shape>* shapes = ani.getAllShapes();
        size_t n = shapes->size();
        bool apply = false;

        bool dostretch = !ImGui::IsWindowCollapsed();

        size_t to_del = -1;
        size_t to_up = -1;
        size_t to_down = -1;

        for (int i = 0; i < n; i++) {
            ImGui::PushID(i);
            ImVec2 node_pos = ImGui::GetCursorScreenPos();
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0);

            if (ImGui::TreeNodeEx("##", ImGuiTreeNodeFlags_Framed, "%d %s", i + 1, snames[(*shapes)[i].type].c_str())) {
                ImGui::BeginTable("border", 1, ImGuiTableFlags_BordersOuter | ImGuiTableFlags_Resizable);
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::PushStyleVarY(ImGuiStyleVar_ItemSpacing, 0);
                if (ImGui::BeginTable("move items table", 3, ImGuiTableFlags_SizingStretchSame * dostretch | ImGuiTableFlags_NoBordersInBody)) {
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);

                    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0, 0.6f, 0.6f));
                    if (ImGui::Button("X")) to_del = i, apply = true;
                    ImGui::PopStyleColor();
                    ImGui::TableSetColumnIndex(2);

                    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, style.ItemSpacing.y));
                    float widthNeeded = style.FontSizeBase * style.FontScaleMain * 2 + style.FramePadding.x * 4.f;;
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - widthNeeded);
                    if (ImGui::ArrowButton("up", ImGuiDir_Up)) to_up = i, apply = true; ImGui::SameLine();
                    if (ImGui::ArrowButton("down", ImGuiDir_Down)) to_down = i, apply = true; ImGui::SameLine();
                    ImGui::PopStyleVar();

                    ImGui::EndTable();
                }
                ImGui::PopStyleVar();
                ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6);

                ImGui::SeparatorText("Shape");
                apply |= (ImGui::RadioButton("Sphere", (int*)&((*shapes)[i].type), 1)); ImGui::SameLine();
                apply |= (ImGui::RadioButton("Plane", (int*)&((*shapes)[i].type), 2)); ImGui::SameLine();
                apply |= (ImGui::RadioButton("Box", (int*)&((*shapes)[i].type), 3)); ImGui::SameLine();
                apply |= (ImGui::RadioButton("Torus", (int*)&((*shapes)[i].type), 4)); ImGui::SameLine();
                apply |= (ImGui::RadioButton("Ellipsoid", (int*)&((*shapes)[i].type), 7));

                ImGui::SeparatorText("Mode");
                apply |= (ImGui::RadioButton("addition", (int*)&((*shapes)[i].mode), 0)); ImGui::SameLine();
                apply |= (ImGui::RadioButton("subtraction", (int*)&((*shapes)[i].mode), 1)); ImGui::SameLine();
                apply |= (ImGui::RadioButton("intersection", (int*)&((*shapes)[i].mode), 2));

                ImGui::SeparatorText("Type of softening");
                apply |= (ImGui::RadioButton("No softening", (int*)&((*shapes)[i].min_type), 0)); ImGui::SameLine();
                apply |= (ImGui::RadioButton("cubic", (int*)&((*shapes)[i].min_type), 1)); ImGui::SameLine();
                apply |= (ImGui::RadioButton("exponential", (int*)&((*shapes)[i].min_type), 2));

                ImGui::SeparatorText("Soft min coeffitient");
                apply |= (ImGui::DragFloat("k", &((*shapes)[i].min_coef), 0.01f, 0, 1, "%.3f"));

                ImGui::SeparatorText("Shape parameters");

                if ((*shapes)[i].type != sindexes::PLANE)
                    apply |= (ImGui::DragFloat3("center", glm::value_ptr((*shapes)[i].data.center), 0.1f, -1000, 1000, "%.2f", ImGuiSliderFlags_ColorMarkers));

                if ((*shapes)[i].type != sindexes::PLANE && (*shapes)[i].type != sindexes::SPHERE) {
                    static vec4 rotation_buffer(0, 1, 0, 0);

                    ImGui::Text("");

                    apply |= (ImGui::DragFloat3("rotation axis", glm::value_ptr((*shapes)[i].rotation), 0.1f, -10, 10, "%.2f", ImGuiSliderFlags_ColorMarkers));
                    apply |= (ImGui::SliderAngle("angle", glm::value_ptr((*shapes)[i].rotation) + 3, -180, 180, "%0.1f deg")); // ImGuiSliderFlags_WrapAround

                    ImGui::Text("");
                }

                if ((*shapes)[i].type == sindexes::SPHERE || (*shapes)[i].type == sindexes::TORUS)
                    apply |= (ImGui::DragFloat("radius", &((*shapes)[i].data.r), 0.01f, 0, 100, "%.3f"));

                if ((*shapes)[i].type == sindexes::PLANE) {
                    apply |= (ImGui::DragFloat3("normal", glm::value_ptr((*shapes)[i].data.N), 0.1f, -1000, 1000, "%.2f", ImGuiSliderFlags_ColorMarkers));
                    apply |= (ImGui::DragFloat("D", &((*shapes)[i].data.D), 0.01f, -1000, 1000, "%.3f"));
                }

                if ((*shapes)[i].type == sindexes::BOX || (*shapes)[i].type == sindexes::ELLIPSOID)
                    apply |= (ImGui::DragFloat3("sides", glm::value_ptr((*shapes)[i].data.sides), 0.1f, 0, 100, "%.2f", ImGuiSliderFlags_ColorMarkers));

                if ((*shapes)[i].type == sindexes::TORUS)
                    apply |= (ImGui::DragFloat("minor radius", &((*shapes)[i].data.R), 0.01f, 0, 100, "%.3f"));

                ImGui::SeparatorText("Material");
                apply |= (ImGui::ColorEdit3("color", glm::value_ptr((*shapes)[i].color), 0));
                apply |= (ImGui::DragFloat("spread", glm::value_ptr((*shapes)[i].color) + 3, 0.01f, 0, 1, "%.2f"));
                apply |= (ImGui::DragFloat("specular", glm::value_ptr((*shapes)[i].spec), 1.0f, 0, 1000, "%.0f"));
                if (transparencyOption)
                    apply |= (ImGui::DragFloat("transparency", &((*shapes)[i].transparency), 0.05f, 0.f, 1.f, "%.3f", ImGuiSliderFlags_AlwaysClamp));
                ImGui::Spacing();
                ImGui::PopStyleVar();
                ImGui::EndTable();
                ImGui::TreePop();
            }
            ImGui::PopStyleVar();
            ImGui::PopID();
        }

        if (ImGui::BeginTable("add shape table", 3, ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_NoBordersInBody)) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(1);
            if (ImGui::Button("Add Shape")) {
                shape new_shape;
                ani.addShape(new_shape);
                apply = true;
            }
            ImGui::EndTable();
        }

        ImGui::Separator();
        ImGui::TextDisabled("Camera position: (%.2f, %.2f, %.2f)", ani.cam.Loc.x, ani.cam.Loc.y, ani.cam.Loc.z);

        if (to_del < n)
            shapes->erase(shapes->begin() + to_del);
        if (to_up >= 1 && to_up < n) {
            shape tmp = (*shapes)[to_up - 1];
            (*shapes)[to_up - 1] = (*shapes)[to_up];
            (*shapes)[to_up] = tmp;
        }
        if (to_down >= 0 && to_down < n - 1) {
            shape tmp = (*shapes)[to_down + 1];
            (*shapes)[to_down + 1] = (*shapes)[to_down];
            (*shapes)[to_down] = tmp;
        }

        if (apply)
            ani.applyShapes();
    }
    ImGui::End();
    ImGui::PopStyleVar();
    return;
}


void UIInit(void) {
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImVec4* colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_Text] = ImVec4(0.95f, 0.93f, 0.93f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.22f, 0.22f, 0.22f, 0.54f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.87f, 0.84f, 0.84f, 0.40f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.77f, 0.77f, 0.77f, 1.00f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.78f, 0.77f, 0.74f, 0.25f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(1.00f, 0.99f, 0.95f, 0.50f);
    colors[ImGuiCol_Button] = ImVec4(0.59f, 0.59f, 0.56f, 0.40f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.50f, 0.50f, 0.49f, 0.75f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.60f, 0.60f, 0.58f, 1.00f);
    colors[ImGuiCol_Header] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.33f, 0.35f, 0.35f, 1.00f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.44f, 0.46f, 0.47f, 1.00f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.75f, 0.75f, 0.75f, 0.78f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.98f, 0.98f, 0.98f, 0.20f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.98f, 0.98f, 0.98f, 0.67f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.98f, 0.98f, 0.98f, 0.95f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.52f, 0.51f, 0.52f, 1.00f);
    colors[ImGuiCol_Tab] = ImVec4(0.20f, 0.20f, 0.20f, 0.86f);
    colors[ImGuiCol_TabSelected] = ImVec4(0.35f, 0.34f, 0.34f, 1.00f);
    colors[ImGuiCol_TabSelectedOverline] = ImVec4(0.66f, 0.75f, 0.86f, 1.00f);
    colors[ImGuiCol_TabDimmed] = ImVec4(0.15f, 0.15f, 0.15f, 0.97f);
    colors[ImGuiCol_TabDimmedSelected] = ImVec4(0.42f, 0.42f, 0.42f, 1.00f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.62f, 0.60f, 0.60f, 0.35f);
    colors[ImGuiCol_NavCursor] = ImVec4(0.87f, 0.92f, 0.98f, 1.00f);

    ImGuiStyle& style = ImGui::GetStyle();
    style.ColorButtonPosition = ImGuiDir_Left;

    // rounding
    style.FrameRounding = 6;
    style.PopupRounding = 6;
    style.GrabRounding = 12;
    style.ScrollbarRounding = 0;
    style.WindowTitleAlign.x = 0.5;
    style.WindowBorderSize = 0;
    style.IndentSpacing = 0;
    style.FrameBorderSize = 1;
    style.WindowRounding = 8;
    style.ColorMarkerSize = 4;
    dark_style = style;
    ImGuiIO& io = ImGui::GetIO();
}