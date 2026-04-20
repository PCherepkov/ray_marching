#include "gui.h"
#include "../anim/anim.h"


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
    if (!ImGui::Begin("Help", &ani.gui.open_help_flag, ImGuiWindowFlags_AlwaysAutoResize))
        return;
    if (ImGui::BeginPopupContextItem()) {
        if (ImGui::MenuItem("Close help"))
            ani.gui.open_help_flag = false;
        ImGui::EndPopup();
    }

    ImGuiStyle& style = ImGui::GetStyle();
    ImGui::PushFont(NULL, style.FontSizeBase * 1.5);
    ImGui::Text("Camera position");
    ImGui::PopFont();
    ImGui::Separator();
    ImGui::Text("W - forward");
    ImGui::Text("A - left");
    ImGui::Text("S - backward");
    ImGui::Text("D - right");
    ImGui::Text("R - upward");
    ImGui::Text("D - downward");
    ImGui::Text("");
    ImGui::Text("scroll wheel down - camera speed down");
    ImGui::Text("scroll wheel up - camera speed up");
    ImGui::Text("");
    ImGui::Text("home - return to (0, 0, 0)");
    ImGui::Text("");
    ImGui::Text("Enter - place a unit sphere on top of the camera");
    ImGui::Separator();

    ImGui::End();
}


void mainTopBar(void) {
    static bool open_help_flag = false;

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
        if (ImGui::BeginMenu("Help"))
        {
            if (ImGui::MenuItem("Open help", "")) { ani.gui.open_help_flag = true; }
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
                                    ImGuiWindowFlags_NoNav;

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
    bool* p_open = 0;
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

        size_t to_del = -1;
        size_t to_up = -1;
        size_t to_down = -1;

        for (int i = 0; i < n; i++) {
            // ImGui::Text("%02.0f | %s: (%.2f,%.2f,%.2f)", (float)(i + 1), snames[shapes[i].type].c_str(), shapes[i].data.center.x, shapes[i].data.center.y, shapes[i].data.center.z);
            ImGui::PushID(i);
            if (ImGui::TreeNode("##", "%d %s", i + 1, snames[(*shapes)[i].type].c_str())) {
                if (ImGui::BeginTable("move items table", 3, ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_NoBordersInBody)) {
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    if (ImGui::Button("del")) to_del = i, apply = true;
                    ImGui::TableSetColumnIndex(2);
                    ImGuiStyle style = ImGui::GetStyle();

                    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, style.ItemSpacing.y));
                    float widthNeeded = style.FontSizeBase * style.FontScaleMain * 2 + style.FramePadding.x * 4.f;;
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - widthNeeded);
                    if (ImGui::ArrowButton("up", ImGuiDir_Up)) to_up = i, apply = true; ImGui::SameLine();
                    if (ImGui::ArrowButton("down", ImGuiDir_Down)) to_down = i, apply = true; ImGui::SameLine();
                    ImGui::PopStyleVar();

                    ImGui::EndTable();
                }

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

                ImGui::SeparatorText("Type of minimum");
                apply |= (ImGui::RadioButton("regular min", (int*)&((*shapes)[i].min_type), 0)); ImGui::SameLine();
                apply |= (ImGui::RadioButton("cubic soft", (int*)&((*shapes)[i].min_type), 1)); ImGui::SameLine();
                apply |= (ImGui::RadioButton("exponential soft", (int*)&((*shapes)[i].min_type), 2));

                ImGui::SeparatorText("Soft min coeffitient");
                apply |= (ImGui::DragFloat("k", &((*shapes)[i].min_coef), 0.01, 0, 1, "%.3f"));

                ImGui::SeparatorText("Shape parameters");

                if ((*shapes)[i].type != sindexes::PLANE)
                    apply |= (ImGui::DragFloat3("center", glm::value_ptr((*shapes)[i].data.center), 0.1, -1000, 1000, "%.2f", ImGuiSliderFlags_ColorMarkers));

                if ((*shapes)[i].type == sindexes::SPHERE || (*shapes)[i].type == sindexes::TORUS)
                    apply |= (ImGui::DragFloat("radius", &((*shapes)[i].data.r), 0.01, 0, 100, "%.3f"));

                if ((*shapes)[i].type == sindexes::PLANE) {
                    apply |= (ImGui::DragFloat3("normal", glm::value_ptr((*shapes)[i].data.N), 0.1, -1000, 1000, "%.2f", ImGuiSliderFlags_ColorMarkers));
                    apply |= (ImGui::DragFloat("D", &((*shapes)[i].data.D), 0.01, -1000, 1000, "%.3f"));
                }

                if ((*shapes)[i].type == sindexes::BOX || (*shapes)[i].type == sindexes::ELLIPSOID)
                    apply |= (ImGui::DragFloat3("sides", glm::value_ptr((*shapes)[i].data.sides), 0.1, 0, 100, "%.2f", ImGuiSliderFlags_ColorMarkers));

                if ((*shapes)[i].type == sindexes::TORUS)
                    apply |= (ImGui::DragFloat("minor radius", &((*shapes)[i].data.R), 0.01, 0, 100, "%.3f"));

                ImGui::SeparatorText("color");
                apply |= (ImGui::ColorEdit3("##", glm::value_ptr((*shapes)[i].color), 0));
                ImGui::TreePop();
            }
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
    return;
}