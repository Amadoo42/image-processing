#pragma once
#include "imgui.h"
#include "../Icons.h"
#include "../FilterDefs.h"
#include "../FilterParamsPanel.h"
#include "../FilterPreview.h"
#include "../MemoryOperation.h"
#include "../PresetManager.h"
#include "../../core/ImageProcessor.h"

static void drawTopNavBar(ImageProcessor &processor) {
    ImGuiIO& io = ImGui::GetIO();
    if(ImGui::BeginMainMenuBar()) {
        const Image &__img_nav = processor.getCurrentImage();
        bool __hasImage = (__img_nav.width > 0 && __img_nav.height > 0);
        if(ImGui::BeginMenu("File")) {
            if(ImGui::MenuItem(iconLabel(ICON_FA_FOLDER_OPEN, "Open").c_str(), "Ctrl+O")) {
                std::string selected =
#ifdef _WIN32
                    openFileDialog_Windows(false, false);
#else
                    openFileDialog_Linux();
#endif
                if(!selected.empty()) {
                    std::cout << "Image loaded successfully!\n";
                    processor.loadImage(selected);
                        processor.clearSelection();
                    guiSetCurrentImagePath(selected);
                    textureNeedsUpdate = true;
                    gPreviewCacheNeedsUpdate = true;
                    gImageSessionId++;
                    gSelectedFilter = FilterType::None;
                    statusBarMessage = "Image loaded successfully!";
                    gPresetManager.clearSession();
                }
                else {
                    std::cerr << "Failed to load image." << std::endl;
                    statusBarMessage = "Failed to load image.";
                }
            }
            if (!__hasImage) ImGui::BeginDisabled();
            if(ImGui::MenuItem(iconLabel(ICON_FA_FLOPPY_DISK, "Save").c_str(), "Ctrl+S")) {
                std::string selected =
#ifdef _WIN32
                    openFileDialog_Windows(true, false);
#else
                    saveFileDialog_Linux();
#endif
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
            if (!__hasImage) ImGui::EndDisabled();
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
                if(processor.undo()) { 
                    textureNeedsUpdate = true; 
                    gPreviewCacheNeedsUpdate = true; 
                    gSelectedFilter = FilterType::None;
                    gPresetManager.clearSession();
                    statusBarMessage = "Undo successful."; 
                }
                else { statusBarMessage = "Nothing to undo."; }
            }
            if (ImGui::MenuItem(iconLabel(ICON_FA_ROTATE_RIGHT, "Redo").c_str(), "Ctrl+Y")) {
                if(processor.redo()) { 
                    textureNeedsUpdate = true; 
                    gPreviewCacheNeedsUpdate = true; 
                    gSelectedFilter = FilterType::None;
                    gPresetManager.clearSession();
                    statusBarMessage = "Redo successful."; 
                }
                else { statusBarMessage = "Nothing to redo."; }
            }
            ImGui::Separator();
            if (ImGui::MenuItem(iconLabel(ICON_FA_GEAR, "Preferences").c_str())) {
                showPreferencesWindow = true;
            }
            if (!__hasImage) ImGui::BeginDisabled();
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
                    bool hasSelection = (gSelectionTool != SelectionToolMode::None);
                    if (hasSelection) ImGui::BeginDisabled();
                    if (ImGui::MenuItem("Crop", nullptr, gSelectedFilter == FilterType::Crop)) gSelectedFilter = FilterType::Crop;
                    if (ImGui::MenuItem("Resize", nullptr, gSelectedFilter == FilterType::Resize)) gSelectedFilter = FilterType::Resize;
                    if (ImGui::MenuItem("Flip", nullptr, gSelectedFilter == FilterType::Flip)) gSelectedFilter = FilterType::Flip;
                    if (ImGui::MenuItem("Rotate", nullptr, gSelectedFilter == FilterType::Rotate)) gSelectedFilter = FilterType::Rotate;
                    if (hasSelection) ImGui::EndDisabled();
                    if (ImGui::MenuItem("Skew", nullptr, gSelectedFilter == FilterType::Skew)) gSelectedFilter = FilterType::Skew;
                    if (hasSelection) ImGui::BeginDisabled();
                    if (ImGui::MenuItem("Merge", nullptr, gSelectedFilter == FilterType::Merge)) gSelectedFilter = FilterType::Merge;
                    if (hasSelection) ImGui::EndDisabled();
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Effects")) {
                    if (ImGui::MenuItem("Blur", nullptr, gSelectedFilter == FilterType::Blur)) gSelectedFilter = FilterType::Blur;
                    bool hasSelection = (gSelectionTool != SelectionToolMode::None);
                    if (hasSelection) ImGui::BeginDisabled();
                    if (ImGui::MenuItem("Frame", nullptr, gSelectedFilter == FilterType::Frame)) gSelectedFilter = FilterType::Frame;
                    if (hasSelection) ImGui::EndDisabled();
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
            if (!__hasImage) ImGui::EndDisabled();
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View")) {
            if (!__hasImage) ImGui::BeginDisabled();
            if (ImGui::MenuItem(iconLabel(ICON_FA_SQUARE_SPLIT, "Compare Side-by-Side").c_str(), nullptr, compareMode)) {
                compareMode = !compareMode;
            }
            if (!__hasImage) ImGui::EndDisabled();
            ImGui::EndMenu();
        }
        if(ImGui::BeginMenu("Help")) {
            if (ImGui::MenuItem(iconLabel(ICON_FA_QUESTION_CIRCLE, "About").c_str())) {
                showAboutWindow = true;
            }
            ImGui::EndMenu();
        }

        float totalWidth = ImGui::GetWindowWidth();
        float menuBarHeight = ImGui::GetFrameHeight();
        float currentX = ImGui::GetCursorPosX();
        
        float estRightWidth = 350.0f; 

        float availableCenterWidth = totalWidth - currentX - estRightWidth;
        float selectionToolsWidthEstimate = 500.0f;
        float padding = std::max(0.0f, (availableCenterWidth / 2.0f) - (selectionToolsWidthEstimate / 2.0f));

        if (padding > 0) {
            ImGui::SameLine(currentX + padding);
        } else {
            ImGui::SameLine();
        }

        ImGui::TableNextColumn();
        if (!__hasImage) ImGui::BeginDisabled();
        ImGui::TextUnformatted("Selection:"); ImGui::SameLine();
        bool selNone = (gSelectionTool == SelectionToolMode::None);
        if (ImGui::RadioButton("None", selNone)) {
            processor.clearSelection();
            gSelectionTool = SelectionToolMode::None;
        }
        ImGui::SameLine();
        bool selRect = (gSelectionTool == SelectionToolMode::Rectangle);
        if (ImGui::RadioButton("Rectangle", selRect)) {
            processor.clearSelection();
            gSelectionTool = SelectionToolMode::Rectangle;
        }
        ImGui::SameLine();
        bool selWand = (gSelectionTool == SelectionToolMode::MagicWand);
        if (ImGui::RadioButton("Magic Wand", selWand)) {
            processor.clearSelection();
            gSelectionTool = SelectionToolMode::MagicWand;
        }
        
        ImGui::SameLine();
        ImGui::Checkbox("Apply to outside", &gSelectionInverseApply);
        processor.setSelectionInvertApply(gSelectionInverseApply);
        ImGui::SameLine();
        if (ImGui::Button("Clear Selection")) {
            processor.clearSelection();
            statusBarMessage = "Selection cleared";
        }
        
        if (gSelectionTool == SelectionToolMode::MagicWand) {
            ImGui::SameLine();
            ImGui::Text("Tolerance:");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(200.0f);
            ImGui::SliderInt("##wand_tol", &gMagicWandTolerance, 0, 200, "%d");
        }

        if (!__hasImage) ImGui::EndDisabled();

        auto executeQuickCommand = [&](const std::string &qraw){
            std::string q = qraw; for (auto &c : q) c = (char)tolower(c);
            if (q.find("open") != std::string::npos || q == "load") {
                std::string selected =
#ifdef _WIN32
                    openFileDialog_Windows(false, false);
#else
                    openFileDialog_Linux();
#endif
                if(!selected.empty()) { processor.loadImage(selected); guiSetCurrentImagePath(selected); textureNeedsUpdate = true; gPreviewCacheNeedsUpdate = true; gImageSessionId++; gSelectedFilter = FilterType::None; statusBarMessage = "Image loaded successfully!"; }
            } else if (q.find("save") != std::string::npos) {
                const Image &img = processor.getCurrentImage();
                if (img.width <= 0 || img.height <= 0) { statusBarMessage = "No image loaded."; }
                else {
                    std::string selected =
#ifdef _WIN32
                        openFileDialog_Windows(true, false);
#else
                        saveFileDialog_Linux();
#endif
                    if (!selected.empty()) { if (processor.saveImage(selected)) { guiSetCurrentImagePath(selected); statusBarMessage = "Image saved to " + selected; } }
                }
            } else if (q.find("undo") != std::string::npos) {
                if(processor.undo()) { textureNeedsUpdate = true; gPreviewCacheNeedsUpdate = true; statusBarMessage = "Undo successful."; } else { statusBarMessage = "Nothing to undo."; }
            } else if (q.find("redo") != std::string::npos) {
                if(processor.redo()) { textureNeedsUpdate = true; gPreviewCacheNeedsUpdate = true; statusBarMessage = "Redo successful."; } else { statusBarMessage = "Nothing to redo."; }
            } else if (q.find("reset") != std::string::npos || q.find("1:1") != std::string::npos) {
                const Image &img = processor.getCurrentImage();
                if (img.width > 0 && img.height > 0) { zoom_level = 1.0f; pan_offset = ImVec2(0,0); } else { statusBarMessage = "No image loaded."; }
            } else if (q.find("fit") != std::string::npos) {
                const Image& img = processor.getCurrentImage();
                if (img.width > 0 && img.height > 0 && gLastCanvasAvail.x > 0.0f && gLastCanvasAvail.y > 0.0f) {
                    zoom_level = 1.0f; pan_offset = ImVec2(0,0);
                    float zx = gLastCanvasAvail.x / img.width; float zy = gLastCanvasAvail.y / img.height; zoom_level = std::max(0.1f, std::min(zx, zy));
                }
            } else if (q.find("compare") != std::string::npos) {
                const Image &img = processor.getCurrentImage();
                if (img.width > 0 && img.height > 0) compareMode = !compareMode; else statusBarMessage = "No image loaded.";
            }
        };

        float rightSpace = ImGui::GetWindowWidth();
        ImGui::SameLine(rightSpace - 320.0f);
        ImGui::SetNextItemWidth(300.0f);
        static bool searchPopupOpen = false;
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
            searchPopupOpen = false;
        }
        else if (ImGui::IsItemDeactivated()) {
            searchPopupOpen = false;
        }
        if (ImGui::IsItemActivated()) searchPopupOpen = true;

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
                        searchPopupOpen = false;
                    }
                    if (++shown >= 7) break;
                }
            }
            ImGui::End();
        }
        ImGui::EndMainMenuBar();
    }
}
