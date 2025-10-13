#pragma once
#include "FiltersMenu.h"
#include "Icons.h"
#include "FilterDefs.h"
#include "LoadTexture.h"
#include "MemoryOperation.h"
#include "CompareView.h"
#include "FilterPreviewPanel.h"
#include <string>

static bool is_dark_theme = true;
static int preferences_theme = 1; // 0 Light, 1 Dark, 2 Classic
static float zoom_level = 1.0f;
static ImVec2 pan_offset = ImVec2(0, 0);
static bool compareMode = false;
static bool showPreferencesWindow = false;
static bool showAboutWindow = false;
static FilterType gSelectedFilter = FilterType::None;
static GLuint currentTextureID = 0;
bool textureNeedsUpdate = false;
static std::string statusBarMessage = "Welcome to Image Processor!";
static std::unique_ptr<FilterPreviewPanel> gFilterPreviewPanel;

// Function to handle filter selection from preview panel
void handleFilterSelection(FilterType selectedFilter, ImageProcessor& processor, bool& textureNeedsUpdate);

void setModernStyle() {
    ImGuiStyle& style = ImGui::GetStyle();

    style.WindowRounding = 8.0f;
    style.FrameRounding = 6.0f;
    style.PopupRounding = 6.0f;
    style.ScrollbarRounding = 9.0f;
    style.GrabRounding = 6.0f;
    style.TabRounding = 6.0f;

    style.Colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
    style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
    style.Colors[ImGuiCol_ChildBg] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
    style.Colors[ImGuiCol_PopupBg] = ImVec4(0.25f, 0.25f, 0.25f, 0.94f);
    style.Colors[ImGuiCol_Border] = ImVec4(0.40f, 0.40f, 0.40f, 0.50f);
    style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.28f, 0.28f, 0.28f, 1.00f);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.10f, 0.10f, 0.10f, 0.70f);
    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.18f, 0.18f, 0.18f, 0.53f);
    style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.55f, 0.55f, 0.55f, 1.00f);
    style.Colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_Button] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_Header] = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_Separator] = style.Colors[ImGuiCol_Border];
    style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.78f);
    style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
    style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
    style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
    style.Colors[ImGuiCol_Tab] = ImLerp(style.Colors[ImGuiCol_Header], style.Colors[ImGuiCol_TitleBgActive], 0.80f);
    style.Colors[ImGuiCol_TabHovered] = style.Colors[ImGuiCol_HeaderHovered];
    style.Colors[ImGuiCol_TabActive] = ImLerp(style.Colors[ImGuiCol_HeaderActive], style.Colors[ImGuiCol_TitleBgActive], 0.60f);
    style.Colors[ImGuiCol_TabUnfocused] = ImLerp(style.Colors[ImGuiCol_Tab], style.Colors[ImGuiCol_TitleBg], 0.80f);
    style.Colors[ImGuiCol_TabUnfocusedActive] = ImLerp(style.Colors[ImGuiCol_TabActive], style.Colors[ImGuiCol_TitleBg], 0.40f);
    style.Colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
    style.Colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    style.Colors[ImGuiCol_NavCursor] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
}

void renderGUI(ImageProcessor &processor) {
    ImGuiIO& io = ImGui::GetIO();
    const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    
    // Initialize preview panel if not already done
    if (!gFilterPreviewPanel) {
        gFilterPreviewPanel = std::make_unique<FilterPreviewPanel>();
        gFilterPreviewPanel->setOnFilterSelected([&processor](FilterType filterType) {
            handleFilterSelection(filterType, processor, textureNeedsUpdate);
        });
    }
    
    // Update preview panel with current image
    const Image& currentImageForPreview = processor.getCurrentImage();
    if (currentImageForPreview.width > 0 && currentImageForPreview.height > 0) {
        gFilterPreviewPanel->updateImage(currentImageForPreview);
    }

    ImGui::SetNextWindowPos(main_viewport->WorkPos);
    ImGui::SetNextWindowSize(main_viewport->WorkSize);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    ImGui::Begin("Main Window", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus);
    ImGui::PopStyleVar(3);

    if(ImGui::BeginMainMenuBar()) {
        if(ImGui::BeginMenu("File")) {
            if(ImGui::MenuItem(iconLabel(ICON_FA_FOLDER_OPEN, "Open").c_str(), "Ctrl+O")) {
                std::string selected = openFileDialog_Linux();
                if(!selected.empty()) {
                    std::cout << "Image loaded successfully!\n";
                    processor.loadImage(selected);
                    textureNeedsUpdate = true;
                    statusBarMessage = "Image loaded successfully!";
                }
                else {
                    std::cerr << "Failed to load image." << std::endl;
                    statusBarMessage = "Failed to load image.";
                }
            }
            if(ImGui::MenuItem(iconLabel(ICON_FA_FLOPPY_DISK, "Save").c_str(), "Ctrl+S")) {
                std::string selected = saveFileDialog_Linux();
                if (!selected.empty()) {
                    if (processor.saveImage(selected)) {
                        std::cout << "Image saved to " << selected << std::endl;
                        statusBarMessage = "Image saved to " + selected;
                    }
                    else {
                        std::cerr << "Failed to save image." << std::endl;
                        statusBarMessage = "Failed to save image.";
                    }
                }
            }
            if(ImGui::MenuItem(iconLabel(ICON_FA_FLOPPY_DISK_CIRCLE_ARROW_RIGHT, "Save As").c_str())) {
                std::string selected = saveFileDialog_Linux();
                if (!selected.empty()) {
                    if (processor.saveImage(selected)) {
                        std::cout << "Image saved to " << selected << std::endl;
                        statusBarMessage = "Image saved to " + selected;
                    }
                    else {
                        std::cerr << "Failed to save image." << std::endl;
                        statusBarMessage = "Failed to save image.";
                    }
                }
            }
            if (ImGui::MenuItem(iconLabel(ICON_FA_RIGHT_FROM_BRACKET, "Exit").c_str())) {
                statusBarMessage = "Use window close button to exit.";
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit")) {
            if (ImGui::MenuItem(iconLabel(ICON_FA_ROTATE_LEFT, "Undo").c_str(), "Ctrl+Z")) {
                if(processor.undo()) { textureNeedsUpdate = true; statusBarMessage = "Undo successful."; }
                else { statusBarMessage = "Nothing to undo."; }
            }
            if (ImGui::MenuItem(iconLabel(ICON_FA_ROTATE_RIGHT, "Redo").c_str(), "Ctrl+Y")) {
                if(processor.redo()) { textureNeedsUpdate = true; statusBarMessage = "Redo successful."; }
                else { statusBarMessage = "Nothing to redo."; }
            }
            ImGui::Separator();
            if (ImGui::MenuItem(iconLabel(ICON_FA_GEAR, "Preferences").c_str())) {
                showPreferencesWindow = true;
            }
            if (ImGui::BeginMenu("Filters")) {
                if (ImGui::BeginMenu("Basic")) {
                    if (ImGui::MenuItem("Grayscale", nullptr, gSelectedFilter == FilterType::Grayscale)) gSelectedFilter = FilterType::Grayscale;
                    if (ImGui::MenuItem("Invert", nullptr, gSelectedFilter == FilterType::Invert)) gSelectedFilter = FilterType::Invert;
                    if (ImGui::MenuItem("Black & White", nullptr, gSelectedFilter == FilterType::BlackAndWhite)) gSelectedFilter = FilterType::BlackAndWhite;
                    if (ImGui::MenuItem("Brightness", nullptr, gSelectedFilter == FilterType::Brightness)) gSelectedFilter = FilterType::Brightness;
                    if (ImGui::MenuItem("Contrast", nullptr, gSelectedFilter == FilterType::Contrast)) gSelectedFilter = FilterType::Contrast;
                    if (ImGui::MenuItem("Saturation", nullptr, gSelectedFilter == FilterType::Saturation)) gSelectedFilter = FilterType::Saturation;
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Transform")) {
                    if (ImGui::MenuItem("Crop", nullptr, gSelectedFilter == FilterType::Crop)) gSelectedFilter = FilterType::Crop;
                    if (ImGui::MenuItem("Resize", nullptr, gSelectedFilter == FilterType::Resize)) gSelectedFilter = FilterType::Resize;
                    if (ImGui::MenuItem("Horizontal Flip", nullptr, gSelectedFilter == FilterType::HorizontalFlip)) gSelectedFilter = FilterType::HorizontalFlip;
                    if (ImGui::MenuItem("Vertical Flip", nullptr, gSelectedFilter == FilterType::VerticalFlip)) gSelectedFilter = FilterType::VerticalFlip;
                    if (ImGui::MenuItem("Rotate", nullptr, gSelectedFilter == FilterType::Rotate)) gSelectedFilter = FilterType::Rotate;
                    if (ImGui::MenuItem("Skew", nullptr, gSelectedFilter == FilterType::Skew)) gSelectedFilter = FilterType::Skew;
                    if (ImGui::MenuItem("Merge", nullptr, gSelectedFilter == FilterType::Merge)) gSelectedFilter = FilterType::Merge;
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Effects")) {
                    if (ImGui::MenuItem("Blur", nullptr, gSelectedFilter == FilterType::Blur)) gSelectedFilter = FilterType::Blur;
                    if (ImGui::MenuItem("Frame", nullptr, gSelectedFilter == FilterType::Frame)) gSelectedFilter = FilterType::Frame;
                    if (ImGui::MenuItem("Outline", nullptr, gSelectedFilter == FilterType::Outline)) gSelectedFilter = FilterType::Outline;
                    if (ImGui::MenuItem("Purple", nullptr, gSelectedFilter == FilterType::Purple)) gSelectedFilter = FilterType::Purple;
                    if (ImGui::MenuItem("Infrared", nullptr, gSelectedFilter == FilterType::Infrared)) gSelectedFilter = FilterType::Infrared;
                    if (ImGui::MenuItem("Wave", nullptr, gSelectedFilter == FilterType::Wave)) gSelectedFilter = FilterType::Wave;
                    if (ImGui::MenuItem("Oil Painting", nullptr, gSelectedFilter == FilterType::OilPainting)) gSelectedFilter = FilterType::OilPainting;
                    if (ImGui::MenuItem("Retro", nullptr, gSelectedFilter == FilterType::Retro)) gSelectedFilter = FilterType::Retro;
                    if (ImGui::MenuItem("Vignette", nullptr, gSelectedFilter == FilterType::Vignette)) gSelectedFilter = FilterType::Vignette;
                    if (ImGui::MenuItem("Warmth", nullptr, gSelectedFilter == FilterType::Warmth)) gSelectedFilter = FilterType::Warmth;
                    ImGui::EndMenu();
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View")) {
            if (ImGui::MenuItem(iconLabel(ICON_FA_SQUARE_SPLIT, "Compare Side-by-Side").c_str(), nullptr, compareMode)) {
                compareMode = !compareMode;
            }
            ImGui::EndMenu();
        }
        if(ImGui::BeginMenu("Help")) {
            if (ImGui::MenuItem(iconLabel(ICON_FA_QUESTION_CIRCLE, "About").c_str())) {
                showAboutWindow = true;
            }
            ImGui::MenuItem(iconLabel(ICON_FA_BOOK, "Documentation").c_str(), nullptr, false, false);
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    // Preferences window
    if (showPreferencesWindow) {
        if (ImGui::Begin("Preferences", &showPreferencesWindow, ImGuiWindowFlags_AlwaysAutoResize)) {
            const char* themes[] = {"Light", "Dark", "Classic"};
            ImGui::Text("Theme");
            ImGui::Separator();
            ImGui::Combo("Color Theme", &preferences_theme, themes, IM_ARRAYSIZE(themes));
            if (ImGui::Button("Apply Theme")) {
                if (preferences_theme == 0) { ImGui::StyleColorsLight(); is_dark_theme = false; }
                else if (preferences_theme == 1) { setModernStyle(); is_dark_theme = true; }
                else { ImGui::StyleColorsClassic(); is_dark_theme = false; }
            }
        }
        ImGui::End();
    }

    if (showAboutWindow) {
        if (ImGui::Begin("About", &showAboutWindow, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Text("Image Processing GUI");
            ImGui::Text("Using Dear ImGui");
        }
        ImGui::End();
    }
    
    ImGui::BeginChild("Tool Panel", ImVec2(250, 0), true);
    ImGui::Text("Tools");
    ImGui::Separator();
    if(ImGui::Button("Open")) {
        std::string selected = openFileDialog_Linux();
        if(!selected.empty()) {
            std::cout << "Image loaded successfully!\n";
            processor.loadImage(selected);
            textureNeedsUpdate = true;
            statusBarMessage = "Image loaded successfully!";
        }
        else {
            std::cerr << "Failed to load image." << std::endl;
            statusBarMessage = "Failed to load image.";
        }
    }
    if(ImGui::Button("Save")) {
        std::string selected = saveFileDialog_Linux();
        if (!selected.empty()) {
            if (processor.saveImage(selected)) {
                std::cout << "Image saved to " << selected << std::endl;
                statusBarMessage = "Image saved to " + selected;
            }
            else {
                std::cerr << "Failed to save image." << std::endl;
                statusBarMessage = "Failed to save image.";
            }
        }
    }
    if (ImGui::Button(compareMode ? "Compare: ON" : "Compare: OFF")) { compareMode = !compareMode; }
    ImGui::Separator();
    if(ImGui::Button("Reset Zoom & Pan", ImVec2(-1, 0))) {
        zoom_level = 1.0f;
        pan_offset = ImVec2(0, 0);
    }

    ImGui::Separator();
    ImGui::Text("History:");
    if(ImGui::Button("Undo", ImVec2(-1, 0))) {
        if(processor.undo()) {
            std::cout << "Undo successful." << std::endl;
            textureNeedsUpdate = true;
            statusBarMessage = "Undo successful.";
        }
        else {
            std::cout << "Nothing to undo." << std::endl;
            statusBarMessage = "Nothing to undo.";
        }
    }
    if(ImGui::Button("Redo", ImVec2(-1, 0))) {
        if(processor.redo()) {
            std::cout << "Redo successful." << std::endl;
            textureNeedsUpdate = true;
            statusBarMessage = "Redo successful.";
        }
        else {
            std::cout << "Nothing to redo." << std::endl;
            statusBarMessage = "Nothing to redo.";
        }
    }
    ImGui::EndChild();
    
    ImGui::SameLine();

    ImGui::BeginChild("Image View", ImVec2(-350, 0), true); // Negative width means "fill the rest of the space except for this amount"
    const Image& currentImage = processor.getCurrentImage();
    if(currentImage.width > 0 && currentImage.height > 0) {
        if(textureNeedsUpdate) {
            if(currentTextureID != 0) glDeleteTextures(1, &currentTextureID);
            currentTextureID = loadTexture(currentImage);
            textureNeedsUpdate = false;
        }
        if(!compareMode) {
            if(ImGui::IsWindowHovered() && io.MouseWheel != 0.0f) {
                zoom_level += io.MouseWheel * 0.1f;
                zoom_level = std::clamp(zoom_level, 0.1f, 10.0f);
            }
            if(ImGui::IsWindowHovered() && ImGui::IsMouseDragging(ImGuiMouseButton_Right)) {
                pan_offset.x += io.MouseDelta.x;
                pan_offset.y += io.MouseDelta.y;
            }
            ImVec2 zoomed_size = ImVec2(currentImage.width * zoom_level, currentImage.height * zoom_level);
            ImVec2 window_size = ImGui::GetContentRegionAvail();
            ImVec2 image_pos = ImVec2(
                (window_size.x - zoomed_size.x) * 0.5f,
                (window_size.y - zoomed_size.y) * 0.5f  
            );

            ImGui::SetCursorPos(image_pos);
            
            ImGui::Image((void*)(intptr_t)currentTextureID, zoomed_size);
        } else {
            renderCompareView(processor, zoom_level, pan_offset);
        }
    }
    ImGui::EndChild();
    
    ImGui::SameLine();

    ImGui::BeginChild("Filters Panel", ImVec2(0, 0), true);
    ImGui::Text("Filters Panel");
    ImGui::Separator();
    
    // Add tabs for different filter views
    if (ImGui::BeginTabBar("FilterTabs")) {
        if (ImGui::BeginTabItem("Filter List")) {
            filtersMenu(processor, textureNeedsUpdate, gSelectedFilter);
            ImGui::EndTabItem();
        }
        
        if (ImGui::BeginTabItem("Filter Previews")) {
            if (gFilterPreviewPanel) {
                gFilterPreviewPanel->render();
            }
            ImGui::EndTabItem();
        }
        
        ImGui::EndTabBar();
    }
    
    ImGui::EndChild();

    ImGui::SetCursorPosY(ImGui::GetWindowHeight() - ImGui::GetFrameHeightWithSpacing());
    ImGui::BeginChild("Status Bar", ImVec2(ImGui::GetWindowWidth(), 20), false);
    float text_width = ImGui::CalcTextSize(statusBarMessage.c_str()).x;
    ImGui::SetCursorPosX((ImGui::GetWindowWidth() - text_width) * 0.5f);
    ImGui::Text("%s", statusBarMessage.c_str());
    ImGui::EndChild();
    ImGui::End();

    
    // This has a lot of major issues, it is a mess with the current layout and it also doesn't work well with real-time preview.
    // {
    //     const int maxThumbs = 6;
    //     const ImVec2 thumbSize = ImVec2(120, 72);
    //     static std::vector<GLuint> historyTexCache;
    //     static std::vector<Image> historyImageCache;
    //     static bool cacheValid = false;

    //     static bool showVersionPreview = false;
    //     static GLuint previewTex = 0;
    //     static Image previewImage;

    //     if (textureNeedsUpdate || !cacheValid) {
    //         if (!historyTexCache.empty()) {
    //             for (GLuint t : historyTexCache) {
    //                 if (t != 0) glDeleteTextures(1, &t);
    //             }
    //             historyTexCache.clear();
    //         }
    //         historyImageCache.clear();

    //         for (int i = 1; i <= maxThumbs; ++i) {
               
    //             bool ok = true;
    //             ImageProcessor copyProc;
    //             try {
    //                 copyProc = processor; 
    //             } catch (...) {
    //                 ok = false;
    //             }
    //             if (!ok) break;

    //             for (int s = 0; s < i; ++s) {
    //                 if (!copyProc.undo()) { ok = false; break; }
    //             }
    //             if (!ok) break;

    //             const Image &histImg = copyProc.getCurrentImage();
    //             if (histImg.width > 0 && histImg.height > 0) {
    //                 historyImageCache.push_back(histImg);
    //                 GLuint tex = loadTexture(histImg); 
    //                 historyTexCache.push_back(tex);
    //             } else {
    //                 break;
    //             }
    //         }

    //         cacheValid = true;
    //         textureNeedsUpdate = true;
    //     }

    //     ImVec2 overlayPos = ImVec2(main_viewport->WorkPos.x + 10.0f,
    //                                main_viewport->WorkPos.y + main_viewport->WorkSize.y - (thumbSize.y + 20.0f));
    //     ImGui::SetNextWindowPos(overlayPos, ImGuiCond_Always);
    //     ImGui::SetNextWindowBgAlpha(0.0f);
    //     if (ImGui::Begin("History Thumbs Overlay", nullptr,
    //                      ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize |
    //                      ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing)) {

    //         ImGui::BeginGroup();
    //         ImGui::Text("History:");
    //         ImGui::SameLine();

    //         for (size_t i = 0; i < historyTexCache.size(); ++i) {
    //             GLuint tex = historyTexCache[i];
    //             char idbuf[32];
    //             std::snprintf(idbuf, sizeof(idbuf), "hist_thumb_%zu", i);

    //             if (ImGui::ImageButton(idbuf, (void*)(intptr_t)tex, thumbSize)) {
    //                 previewImage = historyImageCache[i];
    //                 previewTex = tex;
    //                 showVersionPreview = true;
    //                 ImGui::OpenPopup("Version Preview");
    //             }
    //             ImGui::SameLine();
    //         }
    //         ImGui::EndGroup();

    //         ImGui::End();
    //     }

    //     if (showVersionPreview) {
    //         if (ImGui::BeginPopupModal("Version Preview", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
    //             ImVec2 avail = ImGui::GetContentRegionAvail();
    //             float maxW = std::min(avail.x, 800.0f);
    //             float maxH = 600.0f;
    //             float iw = (float)previewImage.width;
    //             float ih = (float)previewImage.height;
    //             float scale = 1.0f;
    //             if (iw > 0 && ih > 0) scale = std::min(maxW / iw, maxH / ih);
    //             if (scale <= 0.0f) scale = 1.0f;
    //             ImVec2 previewSize = ImVec2(iw * scale, ih * scale);

    //             if (previewTex != 0 && previewImage.width > 0 && previewImage.height > 0) {
    //                 ImGui::Image((void*)(intptr_t)previewTex, previewSize);
    //             } else {
    //                 ImGui::Text("Unable to display preview.");
    //             }

    //             if (ImGui::Button("Close")) {
    //                 ImGui::CloseCurrentPopup();
    //                 showVersionPreview = false;
    //             }
    //             ImGui::EndPopup();
    //         }
    //     }
    // }
}

// Implementation of filter selection handler
void handleFilterSelection(FilterType selectedFilter, ImageProcessor& processor, bool& textureNeedsUpdate) {
    if (selectedFilter == FilterType::None) {
        return;
    }
    
    // Update the global selected filter
    gSelectedFilter = selectedFilter;
    
    // Apply the filter to the current image
    // This is a simplified version - in a real implementation, you'd want to
    // create the appropriate filter instance and apply it
    statusBarMessage = "Filter selected: " + std::string(filterTypeName(selectedFilter));
    textureNeedsUpdate = true;
}