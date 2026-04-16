#include "gui.h"


char* getPath(void) {
    char* outPath = nullptr;
    nfdresult_t result = NFD_OpenDialog("sc", "", &outPath);

    if (result == NFD_OKAY)
        return outPath;
    cout << "Error occured";
    free(outPath);
    return nullptr;
}


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


void mainTopBar(void) {
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Open", "Ctrl+O")) { openScene(); }
            if (ImGui::MenuItem("Save", "Ctrl+S")) { saveScene(); }
            if (ImGui::MenuItem("Save As", "Ctrl+Shift+S")) { saveSceneAs(); }
            if (ImGui::MenuItem("Exit", nullptr)) { ani.exit = true; }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo", "Ctrl+Z")) {}
            if (ImGui::MenuItem("Redo", "Ctrl+Y", false, false)) {} // Disabled item
            ImGui::Separator();
            if (ImGui::MenuItem("Cut", "Ctrl+X")) {}
            if (ImGui::MenuItem("Copy", "Ctrl+C")) {}
            if (ImGui::MenuItem("Paste", "Ctrl+V")) {}
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
    return;
}

void mainOverlay(void) {
    return;
}