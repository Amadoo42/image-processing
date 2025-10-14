#pragma once
#include "FiltersMenu.h"
#include "Icons.h"
#include "FilterDefs.h"
#include "LoadTexture.h"
#include "MemoryOperation.h"
#include "CompareView.h"
#include "FilterPreview.h"
#include "FilterParamsPanel.h"
#include <string>
#include <algorithm>
#include <cmath>

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

// New UI state for refactored layout
enum class LeftTab { Enhancement = 0, Tools, Captions };
static LeftTab gActiveLeftTab = LeftTab::Enhancement;
static float gBlendSlider = 0.5f;                 // Original <-> Effect slider value
static char gSearchBuffer[128] = {0};             // Top-right quick action search
static ImVec2 gLastCanvasAvail = ImVec2(0, 0);    // For Fit-to-screen calculations
static float kLeftPanelPct = 0.14f;               // ≈14% left
static float kRightPanelPct = 0.26f;              // ≈26% right
static std::string gCurrentImagePath;             // last opened/saved path for display

inline void guiSetCurrentImagePath(const std::string &path) { gCurrentImagePath = path; }

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

// --- Top Navigation Bar ----------------------------------------------------
static void drawTopNavBar(ImageProcessor &processor) {
    ImGuiIO& io = ImGui::GetIO();
    if(ImGui::BeginMainMenuBar()) {
        if(ImGui::BeginMenu("File")) {
            if(ImGui::MenuItem(iconLabel(ICON_FA_FOLDER_OPEN, "Open").c_str(), "Ctrl+O")) {
                std::string selected = openFileDialog_Linux();
                if(!selected.empty()) {
                    std::cout << "Image loaded successfully!\n";
                    processor.loadImage(selected);
                    guiSetCurrentImagePath(selected);
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
                        guiSetCurrentImagePath(selected);
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
                        guiSetCurrentImagePath(selected);
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
        if (ImGui::BeginMenu("Tools")) {
            if (ImGui::MenuItem("Reset Zoom & Pan")) { zoom_level = 1.0f; pan_offset = ImVec2(0, 0); }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Retouch")) {
            ImGui::MenuItem("Blemish Remover", nullptr, false, false);
            ImGui::MenuItem("Heal/Clone", nullptr, false, false);
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Effects")) {
            // Quick access to some effects
            if (ImGui::MenuItem("Retro", nullptr, gSelectedFilter == FilterType::Retro)) gSelectedFilter = FilterType::Retro;
            if (ImGui::MenuItem("Oil Painting", nullptr, gSelectedFilter == FilterType::OilPainting)) gSelectedFilter = FilterType::OilPainting;
            if (ImGui::MenuItem("Vignette", nullptr, gSelectedFilter == FilterType::Vignette)) gSelectedFilter = FilterType::Vignette;
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

        // Right-aligned search field
        float rightSpace = ImGui::GetWindowWidth();
        ImGui::SameLine(rightSpace - 320.0f);
        ImGui::SetNextItemWidth(300.0f);
        ImGui::InputTextWithHint("##top_search", "What do you want to do?", gSearchBuffer, IM_ARRAYSIZE(gSearchBuffer));
        ImGui::EndMainMenuBar();
    }
}

// --- Left Side Tabs --------------------------------------------------------
static void drawLeftSideTabs(float width) {
    ImGui::BeginChild("LeftSideTabs", ImVec2(width, 0), true);
    ImGui::TextUnformatted(" "); // small top padding
    const char* labels[] = {"Enhancement", "Tools", "Captions"};
    for (int i = 0; i < 3; ++i) {
        bool selected = static_cast<int>(gActiveLeftTab) == i;
        if (ImGui::Selectable(labels[i], selected, 0, ImVec2(-1, 36))) {
            gActiveLeftTab = static_cast<LeftTab>(i);
        }
    }
    ImGui::EndChild();
}

// --- Histogram Placeholder --------------------------------------------------
static void drawHistogramPlaceholder(const Image &img) {
    ImGui::TextUnformatted("Histogram");
    ImGui::BeginChild("HistogramBox", ImVec2(0, 120), true);
    // Simple grayscale histogram placeholder; if image is empty, draw noise
    const int N = 64;
    static float values[N] = {0};
    static float t = 0.0f;
    t += 0.02f;
    for (int i = 0; i < N; ++i) values[i] = 0.5f + 0.5f * sinf(t + i * 0.15f);
    ImGui::PlotLines("##hist", values, N, 0, nullptr, 0.0f, 1.0f, ImVec2(0, 100));
    ImGui::EndChild();
}

// --- Right Panel ------------------------------------------------------------
static void drawRightPanel(ImageProcessor &processor, float width) {
    ImGui::BeginChild("RightPanel", ImVec2(width, 0), true);

    // Action buttons aligned to the right
    float full = ImGui::GetContentRegionAvail().x;
    float btnW = 80.0f;
    ImGui::SetCursorPosX(std::max(0.0f, full - (btnW + 0.0f)));
    if (ImGui::Button("Save", ImVec2(btnW, 0))) {
        std::string selected = saveFileDialog_Linux();
        if (!selected.empty()) {
            if (processor.saveImage(selected)) { statusBarMessage = std::string("Image saved to ") + selected; }
            else { statusBarMessage = "Failed to save image."; }
        }
    }

    ImGui::Separator();
    ImGui::TextUnformatted("Filters");

    // Category dropdown then content
    static int categoryIndex = 0; // 0 Basic, 1 Transform, 2 Effects
    const char* categories[] = {"Basic Adjustments", "Transformations", "Effects"};
    ImGui::SetNextItemWidth(-1);
    if (ImGui::BeginCombo("##filterCategory", categories[categoryIndex])) {
        for (int i = 0; i < 3; ++i) {
            bool selected = categoryIndex == i;
            if (ImGui::Selectable(categories[i], selected)) {
                categoryIndex = i; // combo will close automatically
            }
        }
        ImGui::EndCombo();
    }

    static FilterPreviewCache previewCache;
    bool invalidate = textureNeedsUpdate;
    ImGui::BeginChild("FiltersContent", ImVec2(0, -220), false, ImGuiWindowFlags_AlwaysVerticalScrollbar);
    if (categoryIndex == 0) {
        // Basic adjustments with previews
        std::vector<FilterType> basics = {
            FilterType::Grayscale,
            FilterType::Invert,
            FilterType::BlackAndWhite,
            FilterType::Brightness,
            FilterType::Contrast,
            FilterType::Saturation
        };
        renderFilterPreviewGrid(previewCache, processor, basics, gSelectedFilter, invalidate, "basic_sidebar", 2, ImVec2(120, 90));
    } else if (categoryIndex == 1) {
        // Transformations as text list
        auto addItem = [&](const char* label, FilterType t) {
            if (ImGui::Selectable(label, gSelectedFilter == t)) gSelectedFilter = t;
        };
        addItem("Crop", FilterType::Crop);
        addItem("Resize", FilterType::Resize);
        addItem("Horizontal Flip", FilterType::HorizontalFlip);
        addItem("Vertical Flip", FilterType::VerticalFlip);
        addItem("Rotate", FilterType::Rotate);
        addItem("Skew", FilterType::Skew);
        addItem("Merge", FilterType::Merge);
    } else {
        // Effects: preview grid for previewables + text entries for non-previewables (Frame)
        std::vector<FilterType> effectsPreview = {
            FilterType::Blur,
            FilterType::Outline,
            FilterType::Purple,
            FilterType::Infrared,
            FilterType::Wave,
            FilterType::OilPainting,
            FilterType::Retro,
            FilterType::Vignette,
            FilterType::Warmth
        };
        renderFilterPreviewGrid(previewCache, processor, effectsPreview, gSelectedFilter, invalidate, "effects_sidebar", 2, ImVec2(120, 90));
        if (ImGui::Selectable("Frame", gSelectedFilter == FilterType::Frame)) gSelectedFilter = FilterType::Frame;
    }
    ImGui::EndChild();

    ImGui::Separator();
    ImGui::TextUnformatted("Filter Parameters");
    renderFilterParamsPanel(processor, gSelectedFilter, textureNeedsUpdate);

    ImGui::EndChild();
}

// --- Image Canvas -----------------------------------------------------------
static void drawImageCanvas(ImageProcessor &processor, float width) {
    ImGui::BeginChild("ImageCanvas", ImVec2(width, 0), true);
    const Image& currentImage = processor.getCurrentImage();
    gLastCanvasAvail = ImGui::GetContentRegionAvail();
    if(currentImage.width > 0 && currentImage.height > 0) {
        if(textureNeedsUpdate) {
            if(currentTextureID != 0) glDeleteTextures(1, &currentTextureID);
            currentTextureID = loadTexture(currentImage);
            textureNeedsUpdate = false;
        }
        if(!compareMode) {
            ImGuiIO& io = ImGui::GetIO();
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

        // Bottom overlay: Original <-> Effect slider
        ImDrawList* dl = ImGui::GetWindowDrawList();
        ImVec2 wpos = ImGui::GetWindowPos();
        ImVec2 wsz  = ImGui::GetWindowSize();
        ImVec2 overlayPos = ImVec2(wpos.x + 12.0f, wpos.y + wsz.y - 36.0f);
        ImVec2 overlaySize = ImVec2(wsz.x - 24.0f, 24.0f);
        dl->AddRectFilled(overlayPos, ImVec2(overlayPos.x + overlaySize.x, overlayPos.y + overlaySize.y), IM_COL32(20,20,20,160), 6.0f);
        ImGui::SetCursorScreenPos(ImVec2(overlayPos.x + 8.0f, overlayPos.y + 4.0f));
        ImGui::TextUnformatted("Original");
        ImGui::SameLine();
        float sliderW = overlaySize.x - 160.0f;
        ImGui::SetNextItemWidth(sliderW);
        ImGui::SliderFloat("##blend", &gBlendSlider, 0.0f, 1.0f, "", ImGuiSliderFlags_NoInput);
        ImGui::SameLine();
        ImGui::TextUnformatted("Effect");
    }
    ImGui::EndChild();
}

// --- Bottom Toolbar ---------------------------------------------------------
static void drawBottomToolbar(ImageProcessor &processor, float fullWidth) {
    float height = 36.0f;
    ImGui::BeginChild("BottomToolbar", ImVec2(0, height), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

    if(ImGui::Button(iconLabel(ICON_FA_ROTATE_LEFT, "Undo").c_str())) {
        if(processor.undo()) { textureNeedsUpdate = true; statusBarMessage = "Undo successful."; }
        else { statusBarMessage = "Nothing to undo."; }
    }
    ImGui::SameLine();
    if(ImGui::Button(iconLabel(ICON_FA_ROTATE_RIGHT, "Redo").c_str())) {
        if(processor.redo()) { textureNeedsUpdate = true; statusBarMessage = "Redo successful."; }
        else { statusBarMessage = "Nothing to redo."; }
    }
    ImGui::SameLine();
    if(ImGui::Button(iconLabel(ICON_FA_ARROWS_ROTATE, "Reset").c_str())) { zoom_level = 1.0f; pan_offset = ImVec2(0,0); }

    ImGui::SameLine();
    ImGui::Dummy(ImVec2(8, 1));
    ImGui::SameLine();

    // Zoom control
    ImGui::TextUnformatted("Zoom");
    ImGui::SameLine();
    float percent = zoom_level * 100.0f;
    ImGui::SetNextItemWidth(160.0f);
    if (ImGui::SliderFloat("##zoom", &percent, 10.0f, 400.0f, "%.0f%%")) {
        zoom_level = percent / 100.0f;
    }
    ImGui::SameLine();
    if (ImGui::Button("Fit")) {
        const Image& img = processor.getCurrentImage();
        if (img.width > 0 && img.height > 0 && gLastCanvasAvail.x > 0.0f && gLastCanvasAvail.y > 0.0f) {
            float zx = gLastCanvasAvail.x / img.width;
            float zy = gLastCanvasAvail.y / img.height;
            zoom_level = std::max(0.1f, std::min(zx, zy));
        }
    }

    // Centered transient status text
    float toolbarWidth = ImGui::GetWindowWidth();
    float text_width = ImGui::CalcTextSize(statusBarMessage.c_str()).x;
    ImGui::SameLine(std::max(0.0f, toolbarWidth * 0.5f - text_width * 0.5f));
    ImGui::Text("%s", statusBarMessage.c_str());

    // Right-aligned image details
    const Image& img = processor.getCurrentImage();
    auto gcd = [](int a, int b){ while(b){ int t=a%b; a=b; b=t;} return std::max(1, a); };
    int g = (img.width>0 && img.height>0) ? gcd(img.width, img.height) : 1;
    int arw = (img.width>0)? img.width / g : 0;
    int arh = (img.height>0)? img.height / g : 0;
    // Extract file name from stored path
    std::string fname = gCurrentImagePath;
    size_t pos = fname.find_last_of("/\\");
    if (pos != std::string::npos) fname = fname.substr(pos + 1);
    if (fname.empty()) fname = "Untitled";

    // Build info string: name | WxH | aspect | zoom
    char infoBuf[256];
    if (img.width > 0 && img.height > 0)
        std::snprintf(infoBuf, sizeof(infoBuf), "%s | %dx%d | %d:%d | %.0f%%",
                      fname.c_str(), img.width, img.height, arw, arh, zoom_level * 100.0f);
    else
        std::snprintf(infoBuf, sizeof(infoBuf), "%s", fname.c_str());

    float infoWidth = ImGui::CalcTextSize(infoBuf).x;
    ImGui::SameLine(std::max(0.0f, toolbarWidth - infoWidth - 8.0f));
    ImGui::TextUnformatted(infoBuf);

    ImGui::EndChild();
}

void renderGUI(ImageProcessor &processor) {
    ImGuiIO& io = ImGui::GetIO();
    const ImGuiViewport* main_viewport = ImGui::GetMainViewport();

    ImGui::SetNextWindowPos(main_viewport->WorkPos);
    ImGui::SetNextWindowSize(main_viewport->WorkSize);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    ImGui::Begin("Main Window", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    ImGui::PopStyleVar(3);

    // Top navigation bar
    drawTopNavBar(processor);

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
    // Reserve space for bottom toolbar so it doesn't overlap left panel
    const float toolbarHeight = 36.0f;
    ImGui::BeginChild("MainContent", ImVec2(0, -toolbarHeight), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    {
        // Layout widths (responsive)
        ImVec2 avail = ImGui::GetContentRegionAvail();
        float leftW  = std::max(180.0f, avail.x * kLeftPanelPct);
        float rightW = std::max(300.0f, avail.x * kRightPanelPct);
        float centerW = std::max(100.0f, avail.x - leftW - rightW);

        // Left tabs
        drawLeftSideTabs(leftW);
        ImGui::SameLine();

        // Center canvas
        drawImageCanvas(processor, centerW);
        ImGui::SameLine();

        // Right sidebar
        drawRightPanel(processor, rightW);
    }
    ImGui::EndChild();

    // Bottom toolbar across main window width
    drawBottomToolbar(processor, ImGui::GetWindowWidth());
    ImGui::End();

    
    // Below was an experimental history overlay left as a reference; disabled for now.
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