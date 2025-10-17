#pragma once
#include "imgui.h"
#include "FiltersMenu.h"
#include "Icons.h"
#include "FilterDefs.h"
#include "LoadTexture.h"
#include "MemoryOperation.h"
#include "CompareView.h"
#include "FilterPreview.h"
#include "FilterParamsPanel.h"
#include "../filters/ResizeFilter.h"
#include "../filters/FlipFilter.h"
#include "PresetManager.h"
#include "ui/TopNavBar.h"
#include "ui/LeftParamsPanel.h"
#include "ui/RightPanel.h"
#include "ui/ImageCanvas.h"
#include "ui/BottomToolbar.h"
#include <string>
#include <algorithm>
#include <cmath>
#include <vector>
#include <sstream>

static bool is_dark_theme = true;
static int preferences_theme = 1;
static float zoom_level = 1.0f;
static ImVec2 pan_offset = ImVec2(0, 0);
static bool compareMode = false;
static bool showPreferencesWindow = false;
static bool showAboutWindow = false;
static FilterType gSelectedFilter = FilterType::None;
static GLuint currentTextureID = 0;
bool textureNeedsUpdate = false;
static std::string statusBarMessage = "Welcome to Image Processor!";

// Selection state
#include "SelectionTools.h"
SelectionToolMode gSelectionTool = SelectionToolMode::None;
static bool gSelectionInverseApply = false;
static int  gMagicWandTolerance = 40;
static bool gHasActiveRectDrag = false;
static ImVec2 gRectDragStart = ImVec2(0,0);
static ImVec2 gRectDragEnd = ImVec2(0,0);
Image gOriginalImageForPreview;
bool gHasOriginalImageForPreview = false;
static int gUndoHistorySize = 20;
static char gBatchOutputDirectory[256] = "output";

// UI state
static char gSearchBuffer[128] = {0};
static ImVec2 gLastCanvasAvail = ImVec2(0, 0);
static float kLeftPanelPct = 0.26f;
static float kRightPanelPct = 0.1f;
bool  gPreviewCacheNeedsUpdate = true;
static std::string gCurrentImagePath;
// Session id increments when a new image is loaded
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

// moved to ui/TopNavBar.h

// moved to ui/LeftParamsPanel.h

// (Removed unused Histogram placeholder)

// moved to ui/RightPanel.h

// moved to ui/ImageCanvas.h

// moved to ui/BottomToolbar.h


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
            
            ImGui::Separator();
            ImGui::Text("Undo/Redo History");
            ImGui::Separator();
            ImGui::SliderInt("History Size", &gUndoHistorySize, 5, 100, "%d steps");
            ImGui::Text("Current history size: %d steps", gUndoHistorySize);
            if (ImGui::Button("Apply History Size")) {
                processor.setHistorySize(gUndoHistorySize);
            }
        }
        ImGui::End();
    }

    if (showAboutWindow) {
        if (ImGui::Begin("About & Documentation", &showAboutWindow, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Text("Image Processing GUI v1.0");
            ImGui::Separator();
            
            ImGui::Text("Features:");
            ImGui::BulletText("Advanced image editing with 20+ filters");
            ImGui::BulletText("Real-time preview and parameter adjustment");
            ImGui::BulletText("Selection tools (Rectangle, Magic Wand)");
            ImGui::BulletText("Preset management and batch processing");
            ImGui::BulletText("Undo/Redo with 20-step history");
            ImGui::BulletText("Compare view for before/after comparison");
            
            ImGui::Separator();
            ImGui::Text("Supported Formats:");
            ImGui::Text("Input: JPG, PNG, BMP, TGA");
            ImGui::Text("Output: JPG, PNG, BMP");
            
            ImGui::Separator();
            ImGui::Text("Keyboard Shortcuts:");
            ImGui::Text("Ctrl+O - Open Image");
            ImGui::Text("Ctrl+S - Save Image");
            ImGui::Text("Ctrl+Z - Undo");
            ImGui::Text("Ctrl+Y - Redo");
            
            ImGui::Separator();
            ImGui::Text("Built with:");
            ImGui::Text("• Dear ImGui for the interface");
            ImGui::Text("• SDL2 for window management");
            ImGui::Text("• OpenGL for rendering");
            ImGui::Text("• Custom C++ image processing engine");
            
            ImGui::Separator();
            ImGui::Text("Usage Tips:");
            ImGui::BulletText("Use mouse wheel to zoom in/out");
            ImGui::BulletText("Right-click and drag to pan around the image");
            ImGui::BulletText("Selection tools work with most filters");
            ImGui::BulletText("Create presets to save filter combinations");
            ImGui::BulletText("Use batch processing for multiple images");
            
            if (ImGui::Button("Close")) {
                showAboutWindow = false;
            }
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
                if (applyIdx >= 0) { gPresetManager.applyPresetBatch(processor, presets[applyIdx]); textureNeedsUpdate = true; }
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
            static char statusBuf[512] = {0};
            static bool batchRunning = false;
            static size_t total = 0;
            static size_t currentIndex = 0;
            static size_t processed = 0;
            static int skipped = 0;
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
            ImGui::Text("Output Directory");
            ImGui::InputText("##outputDir", gBatchOutputDirectory, IM_ARRAYSIZE(gBatchOutputDirectory));
            ImGui::SameLine();
            if (ImGui::Button("Browse")) {
                std::string selectedDir = openDirectoryDialog();
                if (!selectedDir.empty()) {
                    strncpy(gBatchOutputDirectory, selectedDir.c_str(), IM_ARRAYSIZE(gBatchOutputDirectory) - 1);
                    gBatchOutputDirectory[IM_ARRAYSIZE(gBatchOutputDirectory) - 1] = '\0';
                    statusBarMessage = "Output directory set to: " + selectedDir;
                }
            }

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
                PresetManager::ensureOutputFolder(gBatchOutputDirectory);
                total = selectedFiles.size();
                currentIndex = 0;
                processed = 0;
                skipped = 0;
                progress = 0.0f;
                std::snprintf(statusBuf, sizeof(statusBuf), "Starting...");
                statusBarMessage = "Batch started";
                batchRunning = true;
            }
            ImGui::SameLine();
            if (ImGui::Button("Reset")) {
                selectedFiles.clear();
                currentIndex = 0;
                total = 0;
                processed = 0;
                skipped = 0;
                progress = 0.0f;
                std::snprintf(statusBuf, sizeof(statusBuf), "Ready");
                batchRunning = false;
            }
            if (batchRunning) {
                if (currentIndex < total) {
                    const std::string &path = selectedFiles[currentIndex];
                    bool ok = true;
                    Image img;
                    try { img = Image(path); } catch (...) { ok = false; }
                    if (ok && img.width > 0 && img.height > 0) {
                        ImageProcessor localProc;
                        localProc.setImage(img);
                        if (mode == 0) {
                            if (presetIdx >= 0) gPresetManager.applyPresetBatch(localProc, presets[presetIdx]);
                        } else {
                            if (!batchFilterTypes.empty() && filterIdx >= 0 && filterIdx < (int)batchFilterTypes.size()) {
                                FilterStep step{ batchFilterTypes[filterIdx], {}, "" };
                                PresetDefinition single{ "__single__", { step } };
                                gPresetManager.applyPresetBatch(localProc, single);
                            } else {
                                ok = false;
                            }
                        }
                        if (ok) {
                            std::string filename = path;
                            size_t pos = filename.find_last_of("/\\");
                            if (pos != std::string::npos) filename = filename.substr(pos + 1);
                            std::string outPath = std::string(gBatchOutputDirectory) + "/" + filename;
                            try { localProc.saveImage(outPath); } catch (...) { ok = false; }
                        }
                    }
                    if (!ok) skipped++; else processed++;
                    currentIndex++;
                    progress = (total > 0) ? (float)processed / (float)total : 0.0f;
                    std::snprintf(statusBuf, sizeof(statusBuf), "Processing image %zu of %zu...", processed, total);
                }
                if (currentIndex >= total) {
                    batchRunning = false;
                    std::snprintf(statusBuf, sizeof(statusBuf), "Completed. %zu processed, %d skipped. Images saved to: %s/", processed, skipped, gBatchOutputDirectory);
                    progress = 1.0f;
                    statusBarMessage = "Batch completed - images saved to " + std::string(gBatchOutputDirectory) + "/ folder";
                }
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
            static float rotateDeg = 0.0f;
            static float skewDeg = 0.0f;
            static float waveAmp = 0.0f, waveLen = 10.0f;
            static int oilRadius = 5, oilIntensity = 20;
            static int resizeW = 0, resizeH = 0;
            static int cropX = 0, cropY = 0, cropW = 0, cropH = 0;
            static float vignette = 1.0f;
            static float warmth = 1.0f;
            static std::string filePath;

            // Build available filter list for builder (exclude Resize/Crop/Merge per requirement)
            static std::vector<FilterType> builderTypes;
            static std::vector<std::string> builderNames;
            if (builderTypes.empty()) {
                for (int t = (int)FilterType::Grayscale; t <= (int)FilterType::Warmth; ++t) {
                    FilterType ft = (FilterType)t;
                    if (ft == FilterType::None || ft == FilterType::Resize || ft == FilterType::Crop || ft == FilterType::Merge) continue;
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
                    ImGui::SliderFloat("Factor", &contrast, 0.0f, 3.0f, "%.2f");
                    break;
                case FilterType::Saturation:
                    ImGui::SliderFloat("Factor", &saturation, 0.0f, 3.0f, "%.2f");
                    break;
                case FilterType::Rotate: {
                    ImGui::SliderFloat("Degrees", &rotateDeg, -180.0f, 180.0f, "%.0f°");
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
                    ImGui::SliderFloat("Factor", &vignette, 0.0f, 3.0f, "%.2f");
                    break;
                case FilterType::Warmth:
                    ImGui::SliderFloat("Factor", &warmth, -3.0f, 3.0f, "%.2f");
                    break;
                case FilterType::Frame:
                case FilterType::Merge: {
                    ImGui::Text("Path: %s", filePath.empty() ? "<none>" : filePath.c_str());
                    if (ImGui::Button("Choose File")) { 
                        std::string p =
#ifdef _WIN32
                            openFileDialog_Windows(false, false);
#else
                            openFileDialog_Linux();
#endif
                        if (!p.empty()) filePath = p; 
                    }
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
                    case FilterType::Rotate: s.params = { (double)((int)(rotateDeg > 0 ? rotateDeg + 0.5f : rotateDeg - 0.5f)) }; break;
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
            bool disableSave = draftSteps.empty();
            if (disableSave) ImGui::BeginDisabled();
            if (ImGui::Button("Save Preset")) {
                if (presetNameBuf[0] != '\0') {
                    // Filter out any disallowed steps (safety)
                    std::vector<FilterStep> filtered;
                    filtered.reserve(draftSteps.size());
                    for (const auto &s : draftSteps) {
                        if (s.type == FilterType::Resize || s.type == FilterType::Crop || s.type == FilterType::Merge) continue;
                        filtered.push_back(s);
                    }
                    gPresetManager.addPreset(presetNameBuf, filtered);
                    draftSteps.clear();
                    presetNameBuf[0] = '\0';
                    showPresetBuilderWindow = false;
                }
            }
            if (disableSave) ImGui::EndDisabled();
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