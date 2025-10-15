#pragma once

#include "FilterDefs.h"
#include "FilterParameters.h"
#include "PresetManager.h"

// Global flag controlling when filter preview thumbnails should refresh.
// Defined in main.cpp
extern bool gPreviewCacheNeedsUpdate;

// Render inline parameter panels by forcing inline mode for parameter windows.
struct ParamsInlineScope {
    ParamsInlineScope() { g_params_inline_mode = true; }
    ~ParamsInlineScope() { g_params_inline_mode = false; }
};

// Track open states for panels that should close on Apply and reopen on selection
static FilterType s_prevSelected = FilterType::None;
static bool s_rotateOpen = false;
static bool s_skewOpen = false;

inline void onFilterClicked(FilterType ft) {
    if (ft == FilterType::Rotate) s_rotateOpen = true;
    if (ft == FilterType::Skew)   s_skewOpen = true;
}

inline void renderFilterParamsPanel(ImageProcessor &processor, FilterType selected, bool &textureNeedsUpdate)
{
    static FilterParameters params(processor);
    ParamsInlineScope scope; // ensure inline rendering within this panel
    if (selected != s_prevSelected) {
        s_prevSelected = selected;
        // Auto-open when a filter is newly selected
        if (selected == FilterType::Rotate) s_rotateOpen = true;
        if (selected == FilterType::Skew) s_skewOpen = true;
    }

    auto applySimple = [&](FilterType type, auto makeFilter) {
        if (ImGui::Button("Apply")) {
            auto f = makeFilter();
            processor.applyFilter(f);
            textureNeedsUpdate = true;
            gPreviewCacheNeedsUpdate = true;
            // record step for presets
            gPresetManager.recordStep(FilterStep{type, {}, ""});
        }
    };

    switch (selected) {
        case FilterType::None:
            ImGui::TextDisabled("Select a filter to edit its parameters.");
            break;
        case FilterType::Grayscale: {
            ImGui::TextUnformatted("Grayscale");
            applySimple(FilterType::Grayscale, [&]{ return GrayscaleFilter(); });
            break;
        }
        case FilterType::Invert: {
            ImGui::TextUnformatted("Invert");
            applySimple(FilterType::Invert, [&]{ return InvertFilter(); });
            break;
        }
        case FilterType::BlackAndWhite: {
            ImGui::TextUnformatted("Black & White");
            applySimple(FilterType::BlackAndWhite, [&]{ return BlackAndWhiteFilter(); });
            break;
        }
        case FilterType::HorizontalFlip: {
            ImGui::TextUnformatted("Horizontal Flip");
            applySimple(FilterType::HorizontalFlip, [&]{ return HorizontalFlipFilter(); });
            break;
        }
        case FilterType::VerticalFlip: {
            ImGui::TextUnformatted("Vertical Flip");
            applySimple(FilterType::VerticalFlip, [&]{ return VerticalFlipFilter(); });
            break;
        }
        case FilterType::Retro: {
            ImGui::TextUnformatted("Retro");
            applySimple(FilterType::Retro, [&]{ return RetroFilter(); });
            break;
        }
        case FilterType::Infrared: {
            ImGui::TextUnformatted("Infrared");
            applySimple(FilterType::Infrared, [&]{ return InfraredFilter(); });
            break;
        }
        case FilterType::Frame: {
            ImGui::TextUnformatted("Frame");
            if (ImGui::Button("Choose Frame Image")) {
                std::string path = openFileDialog_Linux();
                if (!path.empty()) {
                    Image frame_image(path);
                FrameFilter filter(frame_image);
                processor.applyFilter(filter);
                textureNeedsUpdate = true;
                gPreviewCacheNeedsUpdate = true;
                    gPresetManager.recordStep(FilterStep{FilterType::Frame, {}, path});
                }
            }
            break;
        }
        case FilterType::Merge: {
            ImGui::TextUnformatted("Merge");
            if (ImGui::Button("Choose Merge Image")) {
                std::string path = openFileDialog_Linux();
                if (!path.empty()) {
                    Image merge_image(path);
                    MergeFilter filter(merge_image);
                    processor.applyFilter(filter);
                    textureNeedsUpdate = true;
                    gPreviewCacheNeedsUpdate = true;
                    gPresetManager.recordStep(FilterStep{FilterType::Merge, {}, path});
                }
            }
            break;
        }

        case FilterType::Blur: {
            static bool show = true;
            ImGui::TextUnformatted("Blur");
            params.applyBlur(show, textureNeedsUpdate);
            if (!show) gPreviewCacheNeedsUpdate = true;
            break;
        }
        case FilterType::Brightness: {
            static bool show = true;
            ImGui::TextUnformatted("Brightness");
            params.applyBrightness(show, textureNeedsUpdate);
            if (!show) gPreviewCacheNeedsUpdate = true;
            break;
        }
        case FilterType::Contrast: {
            static bool show = true;
            ImGui::TextUnformatted("Contrast");
            params.applyContrast(show, textureNeedsUpdate);
            if (!show) gPreviewCacheNeedsUpdate = true;
            break;
        }
        case FilterType::Saturation: {
            static bool show = true;
            ImGui::TextUnformatted("Saturation");
            params.applySaturation(show, textureNeedsUpdate);
            if (!show) gPreviewCacheNeedsUpdate = true;
            break;
        }
        case FilterType::OilPainting: {
            static bool show = true;
            ImGui::TextUnformatted("Oil Painting");
            params.applyOilPainting(show, textureNeedsUpdate);
            if (!show) gPreviewCacheNeedsUpdate = true;
            break;
        }
        case FilterType::Rotate: {
            ImGui::TextUnformatted("Rotate");
            if (!s_rotateOpen) {
                if (ImGui::Button("Open Rotate")) s_rotateOpen = true;
            } else {
                bool show = s_rotateOpen;
                params.applyRotate(show, textureNeedsUpdate);
                if (!show) { s_rotateOpen = false; gPreviewCacheNeedsUpdate = true; }
            }
            break;
        }
        case FilterType::Wave: {
            static bool show = true;
            ImGui::TextUnformatted("Wave");
            params.applyWave(show, textureNeedsUpdate);
            if (!show) gPreviewCacheNeedsUpdate = true;
            break;
        }
        case FilterType::Purple: {
            static bool show = true;
            ImGui::TextUnformatted("Purple");
            params.applyPurple(show, textureNeedsUpdate);
            if (!show) gPreviewCacheNeedsUpdate = true;
            break;
        }
        case FilterType::Skew: {
            ImGui::TextUnformatted("Skew");
            if (!s_skewOpen) {
                if (ImGui::Button("Open Skew")) s_skewOpen = true;
            } else {
                bool show = s_skewOpen;
                params.applySkew(show, textureNeedsUpdate);
                if (!show) { s_skewOpen = false; gPreviewCacheNeedsUpdate = true; }
            }
            break;
        }
        case FilterType::Vignette: {
            static bool show = true;
            ImGui::TextUnformatted("Vignette");
            params.applyVignette(show, textureNeedsUpdate);
            if (!show) gPreviewCacheNeedsUpdate = true;
            break;
        }
        case FilterType::Warmth: {
            static bool show = true;
            ImGui::TextUnformatted("Warmth");
            params.applyWarmth(show, textureNeedsUpdate);
            if (!show) gPreviewCacheNeedsUpdate = true;
            break;
        }
        case FilterType::Outline: {
            ImGui::TextUnformatted("Outline");
            applySimple(FilterType::Outline, [&]{ return OutlineFilter(); });
            break;
        }
        case FilterType::Resize: {
            static bool show = false; // open overlay on demand
            ImGui::TextUnformatted("Resize (overlay)");
            if (ImGui::Button("Open Resize Overlay")) {
                show = true;
            }
            params.applyResize(show, textureNeedsUpdate);
            if (!show) gPreviewCacheNeedsUpdate = true;
            break;
        }
        case FilterType::Crop: {
            static bool show = false; // open overlay on demand
            ImGui::TextUnformatted("Crop (overlay)");
            if (ImGui::Button("Open Crop Overlay")) {
                show = true;
            }
            params.applyCrop(show, textureNeedsUpdate);
            if (!show) gPreviewCacheNeedsUpdate = true;
            break;
        }
    }
}
