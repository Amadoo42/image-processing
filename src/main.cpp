#include <SDL.h>
#include <SDL_opengl.h>
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"
#include "core/ImageProcessor.h"
#include "filters/Filter.h"
#include "filters/GrayscaleFilter.h"
#include "filters/InvertFilter.h"
#include "filters/BlackAndWhiteFilter.h"
#include "filters/BlurFilter.h"
#include "filters/ContrastFilter.h"
#include "filters/CropFilter.h"
#include "filters/DarkenFilter.h"
#include "filters/FrameFilter.h"
#include "filters/HorizontalFlipFilter.h"
#include "filters/InfraredFilter.h"
#include "filters/LightenFilter.h"
#include "filters/MergeFilter.h"
#include "filters/OilPaintingFilter.h"
#include "filters/OutlineFilter.h"
#include "filters/PurpleFilter.h"
#include "filters/ResizeFilter.h"
#include "filters/RetroFilter.h"
#include "filters/RotateFilter.h"
#include "filters/SaturationFilter.h"
#include "filters/SkewFilter.h"
#include "filters/VerticalFlipFilter.h"
#include "filters/VigentteFilter.h"
#include "filters/WarmthFilter.h"
#include <iostream>
#include <string>
#include <array>
#include "gui/RenderGUI.h"
#include "gui/PresetManager.h"

int main(int argc, char* argv[]) {
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0) {
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

    ImGuiIO& fonts_io = ImGui::GetIO();
    fonts_io.Fonts->AddFontFromFileTTF("assets/fonts/Inter-Regular.ttf", 16.0f);
    // Try to load Font Awesome (optional). If not present, icons will fallback.
    {
        ImFontConfig cfg;
        cfg.MergeMode = true; // merge into previous font
        cfg.PixelSnapH = true;
        static const ImWchar ranges1[] = { 0xf000, 0xf8ff, 0 }; // FA private use area
        static const ImWchar ranges2[] = { 0xe000, 0xefff, 0 }; // newer private use
        fonts_io.Fonts->AddFontFromFileTTF("assets/fonts/fa-solid-900.ttf", 15.0f, &cfg, ranges1);
        fonts_io.Fonts->AddFontFromFileTTF("assets/fonts/fa-solid-900.ttf", 15.0f, &cfg, ranges2);
    }

    setModernStyle();
    // Load presets on startup
    gPresetManager.loadFromDisk();

    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);
    
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    ImageProcessor imageProcessor;

    bool done = false;
    while(!done) {
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if(event.type == SDL_QUIT) done = true;
            if(event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window)) done = true;
            if (event.type == SDL_KEYDOWN) {
                if (io.KeyCtrl) {
                    if (event.key.keysym.sym == SDLK_o) {
                        std::string selected =
#ifdef _WIN32
                            openFileDialog_Windows(false, false);
#else
                            openFileDialog_Linux();
#endif
                        if(!selected.empty()) {
                            imageProcessor.loadImage(selected);
                            textureNeedsUpdate = true;
                            statusBarMessage = "Image loaded successfully!";
                            guiSetCurrentImagePath(selected);
                            gPreviewCacheNeedsUpdate = true;
                            extern int gImageSessionId; gImageSessionId++;
                            extern FilterType gSelectedFilter; gSelectedFilter = FilterType::None;
                            gPresetManager.clearSession();
                        } else {
                            statusBarMessage = "Failed to load image.";
                        }
                    }
                    if (event.key.keysym.sym == SDLK_s) {
                        const Image &img = imageProcessor.getCurrentImage();
                        if (img.width <= 0 || img.height <= 0) {
                            statusBarMessage = "No image loaded.";
                        } else {
                            std::string selected =
#ifdef _WIN32
                                openFileDialog_Windows(true, false);
#else
                                saveFileDialog_Linux();
#endif
                            if (!selected.empty()) {
                                if (imageProcessor.saveImage(selected)) {
                                    statusBarMessage = "Image saved to " + selected;
                                    guiSetCurrentImagePath(selected);
                                } else {
                                    statusBarMessage = "Failed to save image.";
                                }
                            }
                        }
                    }
                    if (event.key.keysym.sym == SDLK_z) {
                        if(imageProcessor.undo()) {
                            textureNeedsUpdate = true;
                            gPreviewCacheNeedsUpdate = true;
                            statusBarMessage = "Undo successful.";
                        } else {
                            statusBarMessage = "Nothing to undo.";
                        }
                    }
                    if (event.key.keysym.sym == SDLK_y) {
                        if(imageProcessor.redo()) {
                            textureNeedsUpdate = true;
                            gPreviewCacheNeedsUpdate = true;
                            statusBarMessage = "Redo successful.";
                        } else {
                            statusBarMessage = "Nothing to redo.";
                        }
                    }
                }
            }
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