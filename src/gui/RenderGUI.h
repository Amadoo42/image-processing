#pragma once
#include "FiltersMenu.h"
#include "Icons.h"
#include "FilterDefs.h"
#include "LoadTexture.h"
#include "MemoryOperation.h"
#include "CompareView.h"
#include "FilterPreview.h"
#include "FilterParamsPanel.h"
#include "../filters/ResizeFilter.h"
#include "PresetManager.h"
#include <string>
#include <algorithm>
#include <cmath>
#include <vector>
#include <sstream>

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
// Left panel is now dedicated filter parameter panel
static char gSearchBuffer[128] = {0};             // Top-right quick action search
static ImVec2 gLastCanvasAvail = ImVec2(0, 0);    // For Fit-to-screen calculations
static float kLeftPanelPct = 0.26f;               // wider left: params panel
static float kRightPanelPct = 0.1f;              // right as wide as left
bool  gPreviewCacheNeedsUpdate = true;            // controls when thumbnails rebuild (exported)
static std::string gCurrentImagePath;             // last opened/saved path for display
// Incremented whenever a new image is loaded to reset in-progress UIs
int gImageSessionId = 0;
static bool showPresetsWindow = false;
static bool showBatchWindow = false;
static bool showSaveCurrentPresetPopup = false;
static bool showPresetBuilderWindow = false;

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
                    gPreviewCacheNeedsUpdate = true;
                    gImageSessionId++; // reset parameter sessions
                    gSelectedFilter = FilterType::None; // reset to default state
                    statusBarMessage = "Image loaded successfully!";
                    gPresetManager.clearSession();
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
            if (ImGui::MenuItem("Batch Process Images")) {
                showBatchWindow = true;
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

        // Right-aligned search field (functional + sticky suggestions)
        auto executeQuickCommand = [&](const std::string &qraw){
            std::string q = qraw; for (auto &c : q) c = (char)tolower(c);
            if (q.find("open") != std::string::npos || q == "load") {
                std::string selected = openFileDialog_Linux();
                if(!selected.empty()) { processor.loadImage(selected); guiSetCurrentImagePath(selected); textureNeedsUpdate = true; gPreviewCacheNeedsUpdate = true; gImageSessionId++; gSelectedFilter = FilterType::None; statusBarMessage = "Image loaded successfully!"; }
            } else if (q.find("save") != std::string::npos) {
                std::string selected = saveFileDialog_Linux();
                if (!selected.empty()) { if (processor.saveImage(selected)) { guiSetCurrentImagePath(selected); statusBarMessage = "Image saved to " + selected; } }
            } else if (q.find("undo") != std::string::npos) {
                if(processor.undo()) { textureNeedsUpdate = true; statusBarMessage = "Undo successful."; } else { statusBarMessage = "Nothing to undo."; }
            } else if (q.find("redo") != std::string::npos) {
                if(processor.redo()) { textureNeedsUpdate = true; statusBarMessage = "Redo successful."; } else { statusBarMessage = "Nothing to redo."; }
            } else if (q.find("reset") != std::string::npos || q.find("1:1") != std::string::npos) {
                zoom_level = 1.0f; pan_offset = ImVec2(0,0);
            } else if (q.find("fit") != std::string::npos) {
                const Image& img = processor.getCurrentImage();
                if (img.width > 0 && img.height > 0 && gLastCanvasAvail.x > 0.0f && gLastCanvasAvail.y > 0.0f) {
                    float zx = gLastCanvasAvail.x / img.width; float zy = gLastCanvasAvail.y / img.height; zoom_level = std::max(0.1f, std::min(zx, zy));
                }
            } else if (q.find("compare") != std::string::npos) {
                compareMode = !compareMode;
            }
        };

        float rightSpace = ImGui::GetWindowWidth();
        ImGui::SameLine(rightSpace - 320.0f);
        ImGui::SetNextItemWidth(300.0f);
        static bool searchPopupOpen = false;
        // Ensure first click both opens suggestions and focuses the input
        ImVec2 inputPos = ImGui::GetCursorScreenPos();
        ImVec2 inputMinRect = inputPos;
        ImVec2 inputMaxRect = ImVec2(inputPos.x + 300.0f, inputPos.y + ImGui::GetFrameHeight());
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && ImGui::IsMouseHoveringRect(inputMinRect, inputMaxRect)) {
            ImGui::SetKeyboardFocusHere();
            searchPopupOpen = true;
        }
        if (ImGui::InputTextWithHint("##top_search", "What do you want to do?", gSearchBuffer, IM_ARRAYSIZE(gSearchBuffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
            executeQuickCommand(gSearchBuffer);
            gSearchBuffer[0] = '\0';
            searchPopupOpen = false; // close only on Enter
        }
        else if (ImGui::IsItemDeactivated()) {
            searchPopupOpen = false;
        }
        if (ImGui::IsItemActivated()) searchPopupOpen = true; // open when focus enters

        // Autocomplete popup window stays while user interacts with it
        static const struct { const char* cmd; const char* hint; const char* shortcut; } kCmds[] = {
            {"open",   "Open image",           "Ctrl+O"},
            {"save",   "Save image",           "Ctrl+S"},
            {"undo",   "Undo last edit",       "Ctrl+Z"},
            {"redo",   "Redo last edit",       "Ctrl+Y"},
            {"reset",  "Reset zoom/pan",       ""},
            {"fit",    "Fit image to view",    ""},
            {"compare","Toggle compare view",   ""},
        };
        ImVec2 inputMin = ImGui::GetItemRectMin();
        ImVec2 inputMax = ImGui::GetItemRectMax();
        if (searchPopupOpen) {
            ImGui::SetNextWindowPos(ImVec2(inputMin.x, inputMax.y));
            ImGui::SetNextWindowSize(ImVec2(300, 0));
            ImGui::Begin("##autocomplete", &searchPopupOpen, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoFocusOnAppearing);
            std::string q = gSearchBuffer; for (auto &c : q) c = (char)tolower(c);
            int shown = 0;
            for (auto &cdef : kCmds) {
                std::string name = cdef.cmd; std::string hint = cdef.hint;
                std::string combined = name + " " + hint;
                if (q.empty() || combined.find(q) != std::string::npos) {
                    if (ImGui::Selectable((name + " — " + hint + (cdef.shortcut && *cdef.shortcut ? std::string("  ") + cdef.shortcut : "")).c_str())) {
                        executeQuickCommand(name);
                        gSearchBuffer[0] = '\0';
                        searchPopupOpen = false; // close when a function is clicked
                    }
                    if (++shown >= 7) break;
                }
            }
            ImGui::End();
        }
        ImGui::EndMainMenuBar();
    }
}

// --- Left Parameters Panel --------------------------------------------------
static void drawLeftParamsPanel(ImageProcessor &processor, float width) {
    ImGui::BeginChild("LeftParamsPanel", ImVec2(width, 0), true);
    // Filter parameters first
    ImGui::TextUnformatted("Filter Parameters");
    ImGui::Separator();
    renderFilterParamsPanel(processor, gSelectedFilter, textureNeedsUpdate);

    ImGui::NewLine();
    ImGui::TextUnformatted("History");
    ImGui::Separator();
    {
        static std::vector<GLuint> historyTextures;
        static bool historyValid = false;
        static size_t lastUndoCount = (size_t)-1;

        auto releaseHistoryTextures = [&]() {
            for (GLuint t : historyTextures) {
                if (t != 0) glDeleteTextures(1, &t);
            }
            historyTextures.clear();
        };

        const auto &undoH = processor.getUndoHistory();
        bool needRebuild = !historyValid || textureNeedsUpdate || gPreviewCacheNeedsUpdate || (undoH.size() != lastUndoCount);
        if (needRebuild) {
            releaseHistoryTextures();

            std::vector<const Image*> states;
            for (const Image &img : undoH) states.push_back(&img);
            states.push_back(&processor.getCurrentImage());

            historyTextures.reserve(states.size());

            // Downscale to fit within preview thumbnail size to reduce memory
            const int targetW = 120;
            const int targetH = 90;
            for (const Image* src : states) {
                if (!src || src->width <= 0 || src->height <= 0) { historyTextures.push_back(0); continue; }
                float scale = std::min(targetW / (float)src->width, targetH / (float)src->height);
                int newW = std::max(1, (int)std::floor(src->width * scale));
                int newH = std::max(1, (int)std::floor(src->height * scale));
                Image thumb = *src;
                if (newW != src->width || newH != src->height) {
                    ResizeFilter rf(newW, newH);
                    rf.apply(thumb);
                }
                GLuint tex = loadTexture(thumb);
                historyTextures.push_back(tex);
            }
            lastUndoCount = undoH.size();
            historyValid = true;
        }

        // Display at the same size as filter preview thumbnails
        const ImVec2 thumbSize = ImVec2(120, 90);
        // Take the remaining vertical space in the left panel
        ImGui::BeginChild("HistoryList", ImVec2(0, 0), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);
        for (size_t i = 0; i < historyTextures.size(); ++i) {
            GLuint tex = historyTextures[i];
            if (tex != 0) {
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 4.0f);
                ImGui::Image((void*)(intptr_t)tex, thumbSize);
            } else {
                ImGui::Dummy(thumbSize);
            }
            if (i + 1 < historyTextures.size()) ImGui::Separator();
        }
        ImGui::EndChild();
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
    // Presets management header and controls
    ImGui::TextUnformatted("Presets");
    if (ImGui::Button("Manage Presets")) { showPresetsWindow = true; }
    ImGui::SameLine();
    bool hasSession = !gPresetManager.isSessionEmpty();
    if (!hasSession) ImGui::BeginDisabled();
    if (ImGui::Button("Save Current as Preset")) { showSaveCurrentPresetPopup = true; ImGui::OpenPopup("SaveCurrentPreset"); }
    if (!hasSession) ImGui::EndDisabled();
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
    bool invalidate = gPreviewCacheNeedsUpdate;
    // Use a frozen snapshot for previews so slider live previews do not mutate thumbnails
    static Image frozenForPreviews;
    if (invalidate || frozenForPreviews.width == 0) {
        frozenForPreviews = processor.getCurrentImage();
    }
    ImGui::BeginChild("FiltersContent", ImVec2(0, 0), false, ImGuiWindowFlags_AlwaysVerticalScrollbar);
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
        renderFilterPreviewGrid(previewCache, processor, basics, gSelectedFilter, invalidate, "basic_sidebar", 2, ImVec2(120, 90), &frozenForPreviews);
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
        // Effects: preview grid for previewables + text entries for non-previewables
        std::vector<FilterType> effectsPreview = {
            FilterType::Blur,
            FilterType::Outline,
            FilterType::Purple,
            FilterType::Infrared,
            FilterType::Wave,
            FilterType::OilPainting,
            FilterType::Retro,
            FilterType::Vignette,
            FilterType::Warmth,
            FilterType::Frame
        };
        renderFilterPreviewGrid(previewCache, processor, effectsPreview, gSelectedFilter, invalidate, "effects_sidebar", 2, ImVec2(120, 90), &frozenForPreviews);
    }
    ImGui::EndChild();
    if (invalidate) { gPreviewCacheNeedsUpdate = false; }

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
            // Clamp pan so the image never leaves the canvas completely
            ImVec2 base_pos = ImVec2(
                (window_size.x - zoomed_size.x) * 0.5f,
                (window_size.y - zoomed_size.y) * 0.5f
            );
            float minX = -zoomed_size.x * 0.5f;
            float maxX = window_size.x - zoomed_size.x * 0.5f;
            float minY = -zoomed_size.y * 0.5f;
            float maxY = window_size.y - zoomed_size.y * 0.5f;
            pan_offset.x = std::clamp(pan_offset.x, minX - base_pos.x, maxX - base_pos.x);
            pan_offset.y = std::clamp(pan_offset.y, minY - base_pos.y, maxY - base_pos.y);
            ImVec2 image_pos = ImVec2(base_pos.x + pan_offset.x, base_pos.y + pan_offset.y);
            ImGui::SetCursorPos(image_pos);
            ImGui::Image((void*)(intptr_t)currentTextureID, zoomed_size);
        } else {
            renderCompareView(processor, zoom_level, pan_offset);
        }
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

    // Presets management window
    if (showPresetsWindow) {
        ImGui::SetNextWindowSize(ImVec2(420, 360), ImGuiCond_FirstUseEver);
        if (ImGui::Begin("Preset Manager", &showPresetsWindow, ImGuiWindowFlags_AlwaysAutoResize)) {
            static int selectedIndex = -1;
            const auto &presets = gPresetManager.getPresets();
            ImGui::Text("Saved Presets");
            ImGui::Separator();
            if (ImGui::BeginListBox("##presetList", ImVec2(280, 160))) {
                for (int i = 0; i < (int)presets.size(); ++i) {
                    bool sel = selectedIndex == i;
                    if (ImGui::Selectable(presets[i].name.c_str(), sel)) selectedIndex = i;
                }
                ImGui::EndListBox();
            }
            ImGui::Separator();
            if (ImGui::Button("Create New")) { showPresetBuilderWindow = true; }
            ImGui::SameLine();
            bool disableActions = (selectedIndex < 0);
            if (disableActions) ImGui::BeginDisabled();
            if (ImGui::Button("Rename")) ImGui::OpenPopup("RenamePresetPopup");
            ImGui::SameLine();
            if (ImGui::Button("Delete")) {
                if (selectedIndex >= 0 && selectedIndex < (int)presets.size()) {
                    gPresetManager.deletePreset(selectedIndex);
                    selectedIndex = -1;
                }
            }
            if (disableActions) ImGui::EndDisabled();

            if (ImGui::BeginPopupModal("CreatePresetPopup", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
                static char nameBuf[128] = {0};
                const auto &steps = gPresetManager.getSessionSteps();
                ImGui::Text("Filters recorded: %zu", steps.size());
                if (steps.empty()) ImGui::TextDisabled("No filters recorded yet.");
                ImGui::InputText("Preset Name", nameBuf, IM_ARRAYSIZE(nameBuf));
                bool canSave = nameBuf[0] != '\0' && !steps.empty();
                if (!canSave) ImGui::BeginDisabled();
                if (ImGui::Button("Save")) {
                    gPresetManager.addPreset(nameBuf, steps);
                    nameBuf[0] = '\0';
                    ImGui::CloseCurrentPopup();
                }
                if (!canSave) ImGui::EndDisabled();
                ImGui::SameLine();
                if (ImGui::Button("Cancel")) { ImGui::CloseCurrentPopup(); }
                ImGui::EndPopup();
            }
            if (ImGui::BeginPopupModal("RenamePresetPopup", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
                static char nameBuf2[128] = {0};
                if (selectedIndex >= 0 && selectedIndex < (int)presets.size())
                    if (nameBuf2[0] == '\0') std::snprintf(nameBuf2, sizeof(nameBuf2), "%s", presets[selectedIndex].name.c_str());
                ImGui::InputText("New Name", nameBuf2, IM_ARRAYSIZE(nameBuf2));
                bool canRename = (selectedIndex >= 0) && (nameBuf2[0] != '\0');
                if (!canRename) ImGui::BeginDisabled();
                if (ImGui::Button("Rename")) { gPresetManager.renamePreset(selectedIndex, nameBuf2); nameBuf2[0] = '\0'; ImGui::CloseCurrentPopup(); }
                if (!canRename) ImGui::EndDisabled();
                ImGui::SameLine();
                if (ImGui::Button("Cancel")) { nameBuf2[0] = '\0'; ImGui::CloseCurrentPopup(); }
                ImGui::EndPopup();
            }

            // Quick apply preset to current image
            ImGui::Separator();
            ImGui::Text("Apply preset to current image");
            static int applyIdx = -1;
            if (ImGui::BeginCombo("Preset", (applyIdx >=0 && applyIdx < (int)presets.size()) ? presets[applyIdx].name.c_str() : "Select...")) {
                for (int i = 0; i < (int)presets.size(); ++i) {
                    bool sel = applyIdx == i;
                    if (ImGui::Selectable(presets[i].name.c_str(), sel)) applyIdx = i;
                }
                ImGui::EndCombo();
            }
            if (applyIdx < 0) ImGui::BeginDisabled();
            if (ImGui::Button("Apply Now")) {
                if (applyIdx >= 0) { gPresetManager.applyPreset(processor, presets[applyIdx]); textureNeedsUpdate = true; }
            }
            if (applyIdx < 0) ImGui::EndDisabled();
        }
        ImGui::End();
    }
    // Quick save current chain as preset popup
    // Always keep the modal available when flag is set
    if (showSaveCurrentPresetPopup) {
        ImGui::OpenPopup("SaveCurrentPreset");
        if (ImGui::BeginPopupModal("SaveCurrentPreset", &showSaveCurrentPresetPopup, ImGuiWindowFlags_AlwaysAutoResize)) {
            static char nameBuf3[128] = {0};
            const auto &steps = gPresetManager.getSessionSteps();
            ImGui::Text("Filters recorded: %zu", steps.size());
            if (steps.empty()) {
                ImGui::TextDisabled("No filters have been applied yet.");
            }
            ImGui::InputText("Preset Name", nameBuf3, IM_ARRAYSIZE(nameBuf3));
            bool canSave = nameBuf3[0] != '\0' && !steps.empty();
            if (!canSave) ImGui::BeginDisabled();
            if (ImGui::Button("Save")) {
                gPresetManager.addPreset(nameBuf3, steps);
                statusBarMessage = std::string("Preset saved: ") + nameBuf3;
                nameBuf3[0] = '\0';
                ImGui::CloseCurrentPopup();
                showSaveCurrentPresetPopup = false;
            }
            if (!canSave) ImGui::EndDisabled();
            ImGui::SameLine();
            if (ImGui::Button("Cancel")) { ImGui::CloseCurrentPopup(); showSaveCurrentPresetPopup = false; }
            ImGui::EndPopup();
        }
    }
    // Batch processing window
    if (showBatchWindow) {
        if (ImGui::Begin("Batch Process Images", &showBatchWindow, ImGuiWindowFlags_AlwaysAutoResize)) {
            static std::vector<std::string> selectedFiles;
            static int mode = 0; // 0 = preset, 1 = single filter
            static int presetIdx = -1;
            static int filterIdx = 0;
            static float progress = 0.0f;
            static char statusBuf[256] = {0};
            const char* modeLabels[] = {"Apply Preset", "Apply Single Filter"};

            // Build filter list (exclude filters needing extra inputs)
            std::vector<FilterType> batchFilterTypes;
            std::vector<std::string> batchFilterNames;
            for (int t = (int)FilterType::Grayscale; t <= (int)FilterType::Warmth; ++t) {
                FilterType ft = (FilterType)t;
                if (ft == FilterType::None || ft == FilterType::Frame || ft == FilterType::Merge || ft == FilterType::Resize || ft == FilterType::Crop)
                    continue;
                batchFilterTypes.push_back(ft);
                batchFilterNames.push_back(filterTypeName(ft));
            }

            if (ImGui::Button("Select Images")) {
                selectedFiles.clear();
                std::string joined = openMultipleFilesDialog_Linux();
                if (!joined.empty()) {
                    std::stringstream ss(joined);
                    std::string line;
                    while (std::getline(ss, line)) {
                        if (!line.empty()) selectedFiles.push_back(line);
                    }
                }
            }
            ImGui::SameLine();
            ImGui::Text("%zu selected", selectedFiles.size());

            ImGui::Separator();
            ImGui::Text("Mode");
            ImGui::RadioButton(modeLabels[0], &mode, 0); ImGui::SameLine();
            ImGui::RadioButton(modeLabels[1], &mode, 1);

            const auto &presets = gPresetManager.getPresets();
            if (mode == 0) {
                if (ImGui::BeginCombo("Preset", (presetIdx >=0 && presetIdx < (int)presets.size()) ? presets[presetIdx].name.c_str() : "Select...")) {
                    for (int i = 0; i < (int)presets.size(); ++i) {
                        bool sel = presetIdx == i;
                        if (ImGui::Selectable(presets[i].name.c_str(), sel)) presetIdx = i;
                    }
                    ImGui::EndCombo();
                }
            } else {
                if (ImGui::BeginCombo("Filter", (!batchFilterNames.empty() && filterIdx < (int)batchFilterNames.size()) ? batchFilterNames[filterIdx].c_str() : "Select...")) {
                    for (int i = 0; i < (int)batchFilterNames.size(); ++i) {
                        bool sel = filterIdx == i;
                        if (ImGui::Selectable(batchFilterNames[i].c_str(), sel)) filterIdx = i;
                    }
                    ImGui::EndCombo();
                }
            }

            ImGui::Separator();
            bool disableBatch = selectedFiles.empty() || (mode == 0 && presetIdx < 0);
            if (disableBatch) ImGui::BeginDisabled();
            if (ImGui::Button("Start Batch")) {
                PresetManager::ensureOutputFolder("output");
                size_t total = selectedFiles.size();
                size_t processed = 0;
                int skipped = 0;
                statusBarMessage = "Batch started";
                for (size_t i = 0; i < total; ++i) {
                    const std::string &path = selectedFiles[i];
                    bool ok = true;
                    Image img;
                    try { img = Image(path); } catch (...) { ok = false; }
                    if (!ok || img.width <= 0 || img.height <= 0) { skipped++; continue; }
                    ImageProcessor localProc;
                    localProc.setImage(img);
                    if (mode == 0) {
                        if (presetIdx >= 0) gPresetManager.applyPreset(localProc, presets[presetIdx]);
                    } else {
                        if (!batchFilterTypes.empty() && filterIdx >= 0 && filterIdx < (int)batchFilterTypes.size()) {
                            FilterStep step{ batchFilterTypes[filterIdx], {}, "" };
                            PresetDefinition single{ "__single__", { step } };
                            gPresetManager.applyPreset(localProc, single);
                        }
                    }
                    // Save output
                    std::string filename = path;
                    size_t pos = filename.find_last_of("/\\");
                    if (pos != std::string::npos) filename = filename.substr(pos + 1);
                    std::string outPath = std::string("output/") + filename;
                    try { localProc.saveImage(outPath); } catch (...) { skipped++; continue; }
                    processed++;
                    progress = (float)processed / (float)total;
                    std::snprintf(statusBuf, sizeof(statusBuf), "Processing image %zu of %zu...", processed, total);
                }
                std::snprintf(statusBuf, sizeof(statusBuf), "Completed. %zu processed, %d skipped.", selectedFiles.size(), skipped);
                progress = 1.0f;
                statusBarMessage = "Batch completed";
            }
            if (disableBatch) ImGui::EndDisabled();

            ImGui::ProgressBar(progress, ImVec2(320, 0));
            ImGui::TextUnformatted(statusBuf);
        }
        ImGui::End();
    }

    // Preset Builder window
    if (showPresetBuilderWindow) {
        if (ImGui::Begin("Preset Builder", &showPresetBuilderWindow, ImGuiWindowFlags_AlwaysAutoResize)) {
            static std::vector<FilterStep> draftSteps;
            static int addTypeIdx = 0;
            static char presetNameBuf[128] = {0};
            // Parameter widgets state
            static int blurKernel = 13; static float blurSigma = 2.0f;
            static float brightness = 1.0f;
            static float contrast = 1.0f;
            static float saturation = 1.0f;
            static int rotateDeg = 90;
            static float skewDeg = 0.0f;
            static float waveAmp = 0.0f, waveLen = 10.0f;
            static int oilRadius = 5, oilIntensity = 20;
            static int resizeW = 0, resizeH = 0;
            static int cropX = 0, cropY = 0, cropW = 0, cropH = 0;
            static float vignette = 1.0f;
            static float warmth = 1.0f;
            static std::string filePath;

            // Build available filter list for builder (allow all)
            static std::vector<FilterType> builderTypes;
            static std::vector<std::string> builderNames;
            if (builderTypes.empty()) {
                for (int t = (int)FilterType::Grayscale; t <= (int)FilterType::Warmth; ++t) {
                    FilterType ft = (FilterType)t;
                    if (ft == FilterType::None) continue;
                    builderTypes.push_back(ft);
                    builderNames.push_back(filterTypeName(ft));
                }
            }

            ImGui::Text("Add Step");
            if (ImGui::BeginCombo("Filter", builderNames[addTypeIdx].c_str())) {
                for (int i = 0; i < (int)builderNames.size(); ++i) {
                    bool sel = addTypeIdx == i;
                    if (ImGui::Selectable(builderNames[i].c_str(), sel)) addTypeIdx = i;
                }
                ImGui::EndCombo();
            }

            FilterType chosen = builderTypes[addTypeIdx];
            // Show parameter controls per filter
            switch (chosen) {
                case FilterType::Blur:
                    ImGui::InputInt("Kernel (odd)", &blurKernel); if (blurKernel <= 1) blurKernel = 3; if (blurKernel % 2 == 0) blurKernel += 1;
                    ImGui::InputFloat("Sigma", &blurSigma);
                    break;
                case FilterType::Brightness:
                    ImGui::SliderFloat("Factor", &brightness, 0.0f, 3.0f, "%.2f");
                    break;
                case FilterType::Contrast:
                    ImGui::SliderFloat("Factor", &contrast, -3.0f, 3.0f, "%.2f");
                    break;
                case FilterType::Saturation:
                    ImGui::SliderFloat("Factor", &saturation, -3.0f, 3.0f, "%.2f");
                    break;
                case FilterType::Rotate: {
                    const char* items[] = {"90","180","270"};
                    int idx = (rotateDeg==180?1:(rotateDeg==270?2:0));
                    if (ImGui::Combo("Degrees", &idx, items, 3)) rotateDeg = idx==0?90:idx==1?180:270;
                    break; }
                case FilterType::Skew:
                    ImGui::SliderFloat("Angle", &skewDeg, -90.0f, 90.0f, "%.1f");
                    break;
                case FilterType::Wave:
                    ImGui::InputFloat("Amplitude", &waveAmp);
                    ImGui::InputFloat("Wavelength", &waveLen);
                    break;
                case FilterType::OilPainting: {
                    const char* items[] = {"Low","Medium","High"};
                    int idx = (oilIntensity<=10?0:(oilIntensity<=20?1:2));
                    if (ImGui::Combo("Intensity", &idx, items, 3)) oilIntensity = idx==0?10:idx==1?20:40;
                    ImGui::InputInt("Radius", &oilRadius);
                    break; }
                case FilterType::Resize:
                    ImGui::InputInt("Width", &resizeW);
                    ImGui::InputInt("Height", &resizeH);
                    break;
                case FilterType::Crop:
                    ImGui::InputInt("X", &cropX); ImGui::InputInt("Y", &cropY);
                    ImGui::InputInt("W", &cropW); ImGui::InputInt("H", &cropH);
                    break;
                case FilterType::Vignette:
                    ImGui::SliderFloat("Factor", &vignette, -3.0f, 3.0f, "%.2f");
                    break;
                case FilterType::Warmth:
                    ImGui::SliderFloat("Factor", &warmth, -3.0f, 3.0f, "%.2f");
                    break;
                case FilterType::Frame:
                case FilterType::Merge: {
                    ImGui::Text("Path: %s", filePath.empty() ? "<none>" : filePath.c_str());
                    if (ImGui::Button("Choose File")) { std::string p = openFileDialog_Linux(); if (!p.empty()) filePath = p; }
                    break; }
                default: break;
            }

            if (ImGui::Button("Add Step")) {
                FilterStep s{chosen, {}, ""};
                switch (chosen) {
                    case FilterType::Blur: s.params = { (double)blurKernel, (double)blurSigma }; break;
                    case FilterType::Brightness: s.params = { (double)brightness }; break;
                    case FilterType::Contrast: s.params = { (double)contrast }; break;
                    case FilterType::Saturation: s.params = { (double)saturation }; break;
                    case FilterType::Rotate: s.params = { (double)rotateDeg }; break;
                    case FilterType::Skew: s.params = { (double)skewDeg }; break;
                    case FilterType::Wave: s.params = { (double)waveAmp, (double)waveLen }; break;
                    case FilterType::OilPainting: s.params = { (double)oilRadius, (double)oilIntensity }; break;
                    case FilterType::Resize: s.params = { (double)resizeW, (double)resizeH }; break;
                    case FilterType::Crop: s.params = { (double)cropX, (double)cropY, (double)cropW, (double)cropH }; break;
                    case FilterType::Vignette: s.params = { (double)vignette }; break;
                    case FilterType::Warmth: s.params = { (double)warmth }; break;
                    case FilterType::Frame: s.path = filePath; break;
                    case FilterType::Merge: s.path = filePath; break;
                    default: break;
                }
                draftSteps.push_back(s);
            }

            ImGui::Separator();
            ImGui::Text("Steps (%zu)", draftSteps.size());
            for (size_t i = 0; i < draftSteps.size(); ++i) {
                ImGui::PushID((int)i);
                ImGui::Text("%zu. %s", i + 1, filterTypeName(draftSteps[i].type));
                ImGui::SameLine(); if (ImGui::Button("Up") && i > 0) std::swap(draftSteps[i-1], draftSteps[i]);
                ImGui::SameLine(); if (ImGui::Button("Down") && i + 1 < draftSteps.size()) std::swap(draftSteps[i+1], draftSteps[i]);
                ImGui::SameLine(); if (ImGui::Button("Remove")) { draftSteps.erase(draftSteps.begin() + i); ImGui::PopID(); break; }
                ImGui::PopID();
            }

            ImGui::Separator();
            ImGui::InputText("Preset Name", presetNameBuf, IM_ARRAYSIZE(presetNameBuf));
            if (draftSteps.empty()) ImGui::BeginDisabled();
            if (ImGui::Button("Save Preset")) {
                if (presetNameBuf[0] != '\0') {
                    gPresetManager.addPreset(presetNameBuf, draftSteps);
                    draftSteps.clear();
                    presetNameBuf[0] = '\0';
                    showPresetBuilderWindow = false;
                }
            }
            if (draftSteps.empty()) ImGui::EndDisabled();
            ImGui::SameLine(); if (ImGui::Button("Cancel")) { draftSteps.clear(); presetNameBuf[0] = '\0'; showPresetBuilderWindow = false; }
        }
        ImGui::End();
    }
    // Reserve space for bottom toolbar so it doesn't overlap left panel
    const float toolbarHeight = 36.0f;
    ImGui::BeginChild("MainContent", ImVec2(0, -toolbarHeight), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    {
        // Layout widths (responsive)
        ImVec2 avail = ImGui::GetContentRegionAvail();
        float leftW  = std::max(260.0f, avail.x * kLeftPanelPct);
        float rightW = std::max(300.0f, avail.x * kRightPanelPct); // keep right as wide as left
        float centerW = std::max(100.0f, avail.x - leftW - rightW);

        // Left parameters panel
        drawLeftParamsPanel(processor, leftW);
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
}