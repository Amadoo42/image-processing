#pragma once

#include "FilterDefs.h"
#include "FilterParameters.h"

// Global flag controlling when filter preview thumbnails should refresh.
// Defined in main.cpp
extern bool gPreviewCacheNeedsUpdate;

// Render inline parameter panels by forcing inline mode for parameter windows.
struct ParamsInlineScope {
    ParamsInlineScope() { g_params_inline_mode = true; }
    ~ParamsInlineScope() { g_params_inline_mode = false; }
};

inline void renderFilterParamsPanel(ImageProcessor &processor, FilterType selected, bool &textureNeedsUpdate)
{
    static FilterParameters params(processor);
    ParamsInlineScope scope; // ensure inline rendering within this panel

    auto applySimple = [&](auto makeFilter) {
        if (ImGui::Button("Apply")) {
            auto f = makeFilter();
            processor.applyFilter(f);
            textureNeedsUpdate = true;
            gPreviewCacheNeedsUpdate = true;
        }
    };

    switch (selected) {
        case FilterType::None:
            ImGui::TextDisabled("Select a filter to edit its parameters.");
            break;
        case FilterType::Grayscale: {
            ImGui::TextUnformatted("Grayscale");
            applySimple([&]{ return GrayscaleFilter(); });
            break;
        }
        case FilterType::Invert: {
            ImGui::TextUnformatted("Invert");
            applySimple([&]{ return InvertFilter(); });
            break;
        }
        case FilterType::BlackAndWhite: {
            ImGui::TextUnformatted("Black & White");
            applySimple([&]{ return BlackAndWhiteFilter(); });
            break;
        }
        case FilterType::HorizontalFlip: {
            ImGui::TextUnformatted("Horizontal Flip");
            applySimple([&]{ return HorizontalFlipFilter(); });
            break;
        }
        case FilterType::VerticalFlip: {
            ImGui::TextUnformatted("Vertical Flip");
            applySimple([&]{ return VerticalFlipFilter(); });
            break;
        }
        case FilterType::Retro: {
            ImGui::TextUnformatted("Retro");
            applySimple([&]{ return RetroFilter(); });
            break;
        }
        case FilterType::Infrared: {
            ImGui::TextUnformatted("Infrared");
            applySimple([&]{ return InfraredFilter(); });
            break;
        }
        case FilterType::Frame: {
            ImGui::TextUnformatted("Frame");
            if (ImGui::Button("Choose Frame Image")) {
                Image frame_image(openFileDialog_Linux());
                FrameFilter filter(frame_image);
                processor.applyFilter(filter);
                textureNeedsUpdate = true;
                gPreviewCacheNeedsUpdate = true;
            }
            break;
        }
        case FilterType::Merge: {
            ImGui::TextUnformatted("Merge");
            if (ImGui::Button("Choose Merge Image")) {
                Image merge_image(openFileDialog_Linux());
                MergeFilter filter(merge_image);
                processor.applyFilter(filter);
                textureNeedsUpdate = true;
                gPreviewCacheNeedsUpdate = true;
            }
            break;
        }

        case FilterType::Blur: {
            bool show = true;
            ImGui::TextUnformatted("Blur");
            params.applyBlur(show, textureNeedsUpdate);
            if (!show) { gPreviewCacheNeedsUpdate = true; }
            break;
        }
        case FilterType::Brightness: {
            bool show = true;
            ImGui::TextUnformatted("Brightness");
            params.applyBrightness(show, textureNeedsUpdate);
            if (!show) { gPreviewCacheNeedsUpdate = true; }
            break;
        }
        case FilterType::Contrast: {
            bool show = true;
            ImGui::TextUnformatted("Contrast");
            params.applyContrast(show, textureNeedsUpdate);
            if (!show) { gPreviewCacheNeedsUpdate = true; }
            break;
        }
        case FilterType::Saturation: {
            bool show = true;
            ImGui::TextUnformatted("Saturation");
            params.applySaturation(show, textureNeedsUpdate);
            if (!show) { gPreviewCacheNeedsUpdate = true; }
            break;
        }
        case FilterType::OilPainting: {
            bool show = true;
            ImGui::TextUnformatted("Oil Painting");
            params.applyOilPainting(show, textureNeedsUpdate);
            if (!show) { gPreviewCacheNeedsUpdate = true; }
            break;
        }
        case FilterType::Rotate: {
            bool show = true;
            ImGui::TextUnformatted("Rotate");
            params.applyRotate(show, textureNeedsUpdate);
            if (!show) { gPreviewCacheNeedsUpdate = true; }
            break;
        }
        case FilterType::Wave: {
            bool show = true;
            ImGui::TextUnformatted("Wave");
            params.applyWave(show, textureNeedsUpdate);
            if (!show) { gPreviewCacheNeedsUpdate = true; }
            break;
        }
        case FilterType::Purple: {
            bool show = true;
            ImGui::TextUnformatted("Purple");
            params.applyPurple(show, textureNeedsUpdate);
            if (!show) { gPreviewCacheNeedsUpdate = true; }
            break;
        }
        case FilterType::Skew: {
            bool show = true;
            ImGui::TextUnformatted("Skew");
            params.applySkew(show, textureNeedsUpdate);
            if (!show) { gPreviewCacheNeedsUpdate = true; }
            break;
        }
        case FilterType::Vignette: {
            bool show = true;
            ImGui::TextUnformatted("Vignette");
            params.applyVignette(show, textureNeedsUpdate);
            if (!show) { gPreviewCacheNeedsUpdate = true; }
            break;
        }
        case FilterType::Warmth: {
            bool show = true;
            ImGui::TextUnformatted("Warmth");
            params.applyWarmth(show, textureNeedsUpdate);
            if (!show) { gPreviewCacheNeedsUpdate = true; }
            break;
        }
        case FilterType::Outline: {
            ImGui::TextUnformatted("Outline");
            applySimple([&]{ return OutlineFilter(); });
            break;
        }
        case FilterType::Resize: {
            static bool show = true;
            ImGui::TextUnformatted("Resize");
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
