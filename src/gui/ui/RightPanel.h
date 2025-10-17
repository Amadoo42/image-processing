#pragma once
#include "imgui.h"
#include "../FilterPreview.h"
#include "../FilterParamsPanel.h"
#include "../FilterDefs.h"
#include "../PresetManager.h"
#include "../LoadTexture.h"

static void drawRightPanel(ImageProcessor &processor, float width) {
    ImGui::BeginChild("RightPanel", ImVec2(width, 0), true);

    float full = ImGui::GetContentRegionAvail().x;
    float btnW = 80.0f;
    ImGui::SetCursorPosX(std::max(0.0f, full - (btnW + 0.0f)));
    const Image &__img_right = processor.getCurrentImage();
    bool __hasImage = (__img_right.width > 0 && __img_right.height > 0);
    if (!__hasImage) ImGui::BeginDisabled();
    if (ImGui::Button("Save", ImVec2(btnW, 0))) {
        std::string selected =
#ifdef _WIN32
            openFileDialog_Windows(true, false);
#else
            saveFileDialog_Linux();
#endif
        if (!selected.empty()) {
            if (processor.saveImage(selected)) { statusBarMessage = std::string("Image saved to ") + selected; }
            else { statusBarMessage = "Failed to save image."; }
        }
    }
    if (!__hasImage) ImGui::EndDisabled();

    ImGui::Separator();
    ImGui::TextUnformatted("Presets");
    if (ImGui::Button("Manage Presets")) { showPresetsWindow = true; }
    ImGui::SameLine();
    bool hasSession = !gPresetManager.isSessionEmpty();
    if (!hasSession) ImGui::BeginDisabled();
    if (ImGui::Button("Save Current as Preset")) { showSaveCurrentPresetPopup = true; ImGui::OpenPopup("SaveCurrentPreset"); }
    if (!hasSession) ImGui::EndDisabled();
    ImGui::Separator();
    ImGui::TextUnformatted("Filters");

    static int categoryIndex = 0;
    const char* categories[] = {"Basic Adjustments", "Transformations", "Effects"};
    const Image &__img_filters = processor.getCurrentImage();
    bool __hasImageFilters = (__img_filters.width > 0 && __img_filters.height > 0);
    ImGui::SetNextItemWidth(-1);
    if (!__hasImageFilters) ImGui::BeginDisabled();
    if (ImGui::BeginCombo("##filterCategory", categories[categoryIndex])) {
        for (int i = 0; i < 3; ++i) {
            bool selected = categoryIndex == i;
            if (ImGui::Selectable(categories[i], selected)) {
                categoryIndex = i;
            }
        }
        ImGui::EndCombo();
    }
    if (!__hasImageFilters) ImGui::EndDisabled();

    static FilterPreviewCache previewCache;
    bool invalidate = gPreviewCacheNeedsUpdate;
    static Image frozenForPreviews;
    if (invalidate || frozenForPreviews.width == 0) {
        frozenForPreviews = processor.getCurrentImage();
    }
    ImGui::BeginChild("FiltersContent", ImVec2(0, 0), false, ImGuiWindowFlags_AlwaysVerticalScrollbar);
    if (!__hasImageFilters) {
        ImGui::TextDisabled("Load an image to preview and apply filters.");
    } else {
    if (categoryIndex == 0) {
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
        auto addItem = [&](const char* label, FilterType t) {
            if (ImGui::Selectable(label, gSelectedFilter == t)) gSelectedFilter = t;
        };
        bool hasSelection = (gSelectionTool != SelectionToolMode::None);
        if (hasSelection) ImGui::BeginDisabled();
        addItem("Crop", FilterType::Crop);
        addItem("Resize", FilterType::Resize);
        addItem("Flip", FilterType::Flip);
        addItem("Rotate", FilterType::Rotate);
        if (hasSelection) ImGui::EndDisabled();
        addItem("Skew", FilterType::Skew);
        if (hasSelection) ImGui::BeginDisabled();
        addItem("Merge", FilterType::Merge);
        if (hasSelection) ImGui::EndDisabled();
    } else {
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
        if (gSelectionTool != SelectionToolMode::None) {
            effectsPreview.erase(std::remove(effectsPreview.begin(), effectsPreview.end(), FilterType::Frame), effectsPreview.end());
        }
        renderFilterPreviewGrid(previewCache, processor, effectsPreview, gSelectedFilter, invalidate, "effects_sidebar", 2, ImVec2(120, 90), &frozenForPreviews);
    }
    }
    ImGui::EndChild();
    if (invalidate) { gPreviewCacheNeedsUpdate = false; }

    ImGui::EndChild();
}
