// main_with_file_dialog.cpp
#include <SDL.h>
#include <SDL_opengl.h>
#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"

#include "core/ImageProcessor.h"
#include "filters/Filter.h"
#include "filters/GrayscaleFilter.h"
#include "filters/InvertFilter.h"

#include <cstdio>
#include <memory>
#include <array>
#include <string>
#include <iostream>

// -------------------- Linux file dialog helpers (kdialog / zenity) --------------------
static std::string runCommandCapture(const char* cmd) {
    std::array<char, 4096> buf;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) return {};
    while (fgets(buf.data(), buf.size(), pipe.get()) != nullptr) result += buf.data();
    if (!result.empty() && result.back() == '\n') result.pop_back();
    return result;
}

static std::string openFileDialog_Linux() {
    // Try kdialog (KDE)
    std::string out = runCommandCapture("which kdialog >/dev/null 2>&1 && kdialog --getopenfilename 2>/dev/null || true");
    if (!out.empty()) return out;

    // Try zenity (GNOME) with an image filter
    out = runCommandCapture(
        "which zenity >/dev/null 2>&1 && "
        "zenity --file-selection --title=\"Select an image\" --file-filter='Images | *.png *.jpg *.jpeg *.bmp *.gif' 2>/dev/null || true"
    );
    if (!out.empty()) return out;

    // No dialog found or user cancelled -> return empty string
    return std::string();
}

// -------------------- GUI (uses ImGui) --------------------
void renderGUI(ImageProcessor &processor) {
    ImGui::Begin("Image Processor");

    // We'll keep temporary UI state here:
    static char manual_path_buf[1024] = "";
    static bool show_manual_popup = false;

    if (ImGui::Button("Load Image")) {
        // Try system dialog first (kdialog/zenity)
        std::string selected = openFileDialog_Linux();
        if (!selected.empty()) {
            // Successfully got a path from a system dialog
            std::cout << "Selected: " << selected << std::endl;
            processor.loadImage(selected.c_str());
            std::cout << "Image loaded: " << selected << std::endl;
        } else {
            // No system dialog available or user cancelled -> show manual entry popup
            show_manual_popup = true;
            ImGui::OpenPopup("Enter image path");
        }
    }

    // Manual path popup (fallback)
    if (ImGui::BeginPopupModal("Enter image path", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::TextWrapped("No system file dialog available or user cancelled.\nPaste or type the image path below and press Load.");
        ImGui::InputText("Path", manual_path_buf, sizeof(manual_path_buf));
        ImGui::Spacing();
        if (ImGui::Button("Load")) {
            std::string manualPath = std::string(manual_path_buf);
            if (!manualPath.empty()) {
                std::cout << "Manual path: " << manualPath << std::endl;
                processor.loadImage(manualPath.c_str());
                std::cout << "Image loaded: " << manualPath << std::endl;
            } else {
                std::cerr << "Manual path empty, not loading.\n";
            }
            ImGui::CloseCurrentPopup();
            show_manual_popup = false;
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
            ImGui::CloseCurrentPopup();
            show_manual_popup = false;
        }
        ImGui::EndPopup();
    }

    ImGui::SameLine();
    if (ImGui::Button("Save Image")) {
        if (processor.saveImage("output.jpg")) std::cout << "Image saved to output.jpg" << std::endl;
        else std::cerr << "Failed to save image." << std::endl;
    }

    ImGui::Separator();

    ImGui::Text("Filters:");
    if (ImGui::Button("Apply Grayscale Filter")) {
        GrayscaleFilter filter;
        processor.applyFilter(filter);
        std::cout << "Grayscale filter applied." << std::endl;
    }
    if (ImGui::Button("Apply Invert Filter")) {
        InvertFilter filter;
        processor.applyFilter(filter);
        std::cout << "Invert filter applied." << std::endl;
    }

    ImGui::Separator();

    ImGui::Text("History:");
    if (ImGui::Button("Undo")) {
        if (processor.undo()) std::cout << "Undo successful." << std::endl;
        else std::cout << "Nothing to undo." << std::endl;
    }

    ImGui::SameLine();
    if (ImGui::Button("Redo")) {
        if (processor.redo()) std::cout << "Redo successful." << std::endl;
        else std::cout << "Nothing to redo." << std::endl;
    }

    ImGui::End();
}

// -------------------- main --------------------
int main(int argc, char* argv[]) {
    (void)argc; (void)argv;
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0) {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }

    const char* glsl_version = "#version 130";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Window* window = SDL_CreateWindow("Image Processor GUI", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    ImageProcessor imageProcessor;

    bool done = false;
    while (!done) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT) done = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window)) done = true;
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        renderGUI(imageProcessor);

        ImGui::Render();
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
