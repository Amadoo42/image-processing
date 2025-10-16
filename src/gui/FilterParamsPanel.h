#pragma once

#include "FilterDefs.h"
#include "FilterParameters.h"
#include "PresetManager.h"
// This header is included by multiple translation units. Keep definitions here either static or inline.
// The global is actually defined in RenderGUI.h, so we only declare it here.
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
// Effect panels open-state
static bool s_blurOpen = false;
static bool s_outlineOpen = false;
static bool s_purpleOpen = false;
static bool s_infraredOpen = false;
static bool s_waveOpen = false;
static bool s_oilOpen = false;
static bool s_retroOpen = false;
static bool s_vignetteOpen = false;
static bool s_warmthOpen = false;
// Simple filter panels open-state
static bool s_grayscaleOpen = false;
static bool s_invertOpen = false;
static bool s_blackAndWhiteOpen = false;
static bool s_horizontalFlipOpen = false;
static bool s_verticalFlipOpen = false;
// Parameter-based filter panels open-state
static bool s_brightnessOpen = false;
static bool s_contrastOpen = false;
static bool s_saturationOpen = false;
static bool s_oilPaintingOpen = false;
// File dialog filter panels open-state
static bool s_frameOpen = false;
static bool s_mergeOpen = false;
// Overlay filter panels open-state
static bool s_resizeOpen = false;
static bool s_cropOpen = false;

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
        if (selected == FilterType::Blur) s_blurOpen = true;
        if (selected == FilterType::Outline) s_outlineOpen = true;
        if (selected == FilterType::Purple) s_purpleOpen = true;
        if (selected == FilterType::Infrared) s_infraredOpen = true;
        if (selected == FilterType::Wave) s_waveOpen = true;
        if (selected == FilterType::OilPainting) s_oilOpen = true;
        if (selected == FilterType::Retro) s_retroOpen = true;
        if (selected == FilterType::Vignette) s_vignetteOpen = true;
        if (selected == FilterType::Warmth) s_warmthOpen = true;
        // Simple filters
        if (selected == FilterType::Grayscale) s_grayscaleOpen = true;
        if (selected == FilterType::Invert) s_invertOpen = true;
        if (selected == FilterType::BlackAndWhite) s_blackAndWhiteOpen = true;
        if (selected == FilterType::HorizontalFlip) s_horizontalFlipOpen = true;
        if (selected == FilterType::VerticalFlip) s_verticalFlipOpen = true;
        // Parameter-based filters
        if (selected == FilterType::Brightness) s_brightnessOpen = true;
        if (selected == FilterType::Contrast) s_contrastOpen = true;
        if (selected == FilterType::Saturation) s_saturationOpen = true;
        // File dialog filters
        if (selected == FilterType::Frame) s_frameOpen = true;
        if (selected == FilterType::Merge) s_mergeOpen = true;
        // Overlay filters
        if (selected == FilterType::Resize) s_resizeOpen = true;
        if (selected == FilterType::Crop) s_cropOpen = true;
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
            if (!s_grayscaleOpen) {
                if (ImGui::Button("Open Grayscale")) s_grayscaleOpen = true;
            } else {
                if (ImGui::Button("Apply")) {
                    GrayscaleFilter f; processor.applyFilter(f);
                    textureNeedsUpdate = true; gPreviewCacheNeedsUpdate = true;
                    gPresetManager.recordStep(FilterStep{FilterType::Grayscale, {}, ""});
                    s_grayscaleOpen = false;
                }
            }
            break;
        }
        case FilterType::Invert: {
            ImGui::TextUnformatted("Invert");
            if (!s_invertOpen) {
                if (ImGui::Button("Open Invert")) s_invertOpen = true;
            } else {
                if (ImGui::Button("Apply")) {
                    InvertFilter f; processor.applyFilter(f);
                    textureNeedsUpdate = true; gPreviewCacheNeedsUpdate = true;
                    gPresetManager.recordStep(FilterStep{FilterType::Invert, {}, ""});
                    s_invertOpen = false;
                }
            }
            break;
        }
        case FilterType::BlackAndWhite: {
            ImGui::TextUnformatted("Black & White");
            if (!s_blackAndWhiteOpen) {
                if (ImGui::Button("Open Black & White")) s_blackAndWhiteOpen = true;
            } else {
                if (ImGui::Button("Apply")) {
                    BlackAndWhiteFilter f; processor.applyFilter(f);
                    textureNeedsUpdate = true; gPreviewCacheNeedsUpdate = true;
                    gPresetManager.recordStep(FilterStep{FilterType::BlackAndWhite, {}, ""});
                    s_blackAndWhiteOpen = false;
                }
            }
            break;
        }
        case FilterType::HorizontalFlip: {
            ImGui::TextUnformatted("Horizontal Flip");
            if (!s_horizontalFlipOpen) {
                if (ImGui::Button("Open Horizontal Flip")) s_horizontalFlipOpen = true;
            } else {
                if (ImGui::Button("Apply")) {
                    HorizontalFlipFilter f; processor.applyFilter(f);
                    textureNeedsUpdate = true; gPreviewCacheNeedsUpdate = true;
                    gPresetManager.recordStep(FilterStep{FilterType::HorizontalFlip, {}, ""});
                    s_horizontalFlipOpen = false;
                }
            }
            break;
        }
        case FilterType::VerticalFlip: {
            ImGui::TextUnformatted("Vertical Flip");
            if (!s_verticalFlipOpen) {
                if (ImGui::Button("Open Vertical Flip")) s_verticalFlipOpen = true;
            } else {
                if (ImGui::Button("Apply")) {
                    VerticalFlipFilter f; processor.applyFilter(f);
                    textureNeedsUpdate = true; gPreviewCacheNeedsUpdate = true;
                    gPresetManager.recordStep(FilterStep{FilterType::VerticalFlip, {}, ""});
                    s_verticalFlipOpen = false;
                }
            }
            break;
        }
        case FilterType::Retro: {
            ImGui::TextUnformatted("Retro");
            if (!s_retroOpen) {
                if (ImGui::Button("Open Retro")) s_retroOpen = true;
            } else {
                if (ImGui::Button("Apply")) {
                    RetroFilter f; processor.applyFilter(f);
                    textureNeedsUpdate = true; gPreviewCacheNeedsUpdate = true;
                    gPresetManager.recordStep(FilterStep{FilterType::Retro, {}, ""});
                    s_retroOpen = false;
                }
            }
            break;
        }
        case FilterType::Infrared: {
            ImGui::TextUnformatted("Infrared");
            if (!s_infraredOpen) {
                if (ImGui::Button("Open Infrared")) s_infraredOpen = true;
            } else {
                if (ImGui::Button("Apply")) {
                    InfraredFilter f; processor.applyFilter(f);
                    textureNeedsUpdate = true; gPreviewCacheNeedsUpdate = true;
                    gPresetManager.recordStep(FilterStep{FilterType::Infrared, {}, ""});
                    s_infraredOpen = false;
                }
            }
            break;
        }
        case FilterType::Frame: {
            ImGui::TextUnformatted("Frame");
            if (!s_frameOpen) {
                if (ImGui::Button("Open Frame")) s_frameOpen = true;
            } else {
                if (ImGui::Button("Choose Frame Image")) {
                    std::string path =
#ifdef _WIN32
                        openFileDialog_Windows(false, false);
#else
                        openFileDialog_Linux();
#endif
                    if (!path.empty()) {
                        Image frame_image(path);
                        FrameFilter filter(frame_image);
                        processor.applyFilter(filter);
                        textureNeedsUpdate = true;
                        gPreviewCacheNeedsUpdate = true;
                    // Do not record Frame/Merge/Crop/Resize per requirement
                        s_frameOpen = false;
                    }
                }
            }
            break;
        }
        case FilterType::Merge: {
            ImGui::TextUnformatted("Merge");
            if (!s_mergeOpen) {
                if (ImGui::Button("Open Merge")) s_mergeOpen = true;
            } else {
                if (ImGui::Button("Choose Merge Image")) {
                    std::string path =
#ifdef _WIN32
                        openFileDialog_Windows(false, false);
#else
                        openFileDialog_Linux();
#endif
                    if (!path.empty()) {
                        Image merge_image(path);
                        MergeFilter filter(merge_image);
                        processor.applyFilter(filter);
                        textureNeedsUpdate = true;
                        gPreviewCacheNeedsUpdate = true;
                        // Do not record Merge per requirement
                        s_mergeOpen = false;
                    }
                }
                {
                    bool show = s_mergeOpen;
                    params.applyMerge(show, textureNeedsUpdate);
                    if (!show) { s_mergeOpen = false; gPreviewCacheNeedsUpdate = true; }
                }
            }
            break;
        }

        case FilterType::Blur: {
            ImGui::TextUnformatted("Blur");
            if (!s_blurOpen) {
                if (ImGui::Button("Open Blur")) s_blurOpen = true;
            } else {
                bool show = s_blurOpen;
                params.applyBlur(show, textureNeedsUpdate);
                if (!show) { s_blurOpen = false; gPreviewCacheNeedsUpdate = true; }
            }
            break;
        }
        case FilterType::Brightness: {
            ImGui::TextUnformatted("Brightness");
            if (!s_brightnessOpen) {
                if (ImGui::Button("Open Brightness")) s_brightnessOpen = true;
            } else {
                bool show = s_brightnessOpen;
                params.applyBrightness(show, textureNeedsUpdate);
                if (!show) { s_brightnessOpen = false; gPreviewCacheNeedsUpdate = true; }
            }
            break;
        }
        case FilterType::Contrast: {
            ImGui::TextUnformatted("Contrast");
            if (!s_contrastOpen) {
                if (ImGui::Button("Open Contrast")) s_contrastOpen = true;
            } else {
                bool show = s_contrastOpen;
                params.applyContrast(show, textureNeedsUpdate);
                if (!show) { s_contrastOpen = false; gPreviewCacheNeedsUpdate = true; }
            }
            break;
        }
        case FilterType::Saturation: {
            ImGui::TextUnformatted("Saturation");
            if (!s_saturationOpen) {
                if (ImGui::Button("Open Saturation")) s_saturationOpen = true;
            } else {
                bool show = s_saturationOpen;
                params.applySaturation(show, textureNeedsUpdate);
                if (!show) { s_saturationOpen = false; gPreviewCacheNeedsUpdate = true; }
            }
            break;
        }
        case FilterType::OilPainting: {
            ImGui::TextUnformatted("Oil Painting");
            if (!s_oilPaintingOpen) {
                if (ImGui::Button("Open Oil Painting")) s_oilPaintingOpen = true;
            } else {
                bool show = s_oilPaintingOpen;
                params.applyOilPainting(show, textureNeedsUpdate);
                if (!show) { s_oilPaintingOpen = false; gPreviewCacheNeedsUpdate = true; }
            }
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
            ImGui::TextUnformatted("Wave");
            if (!s_waveOpen) { if (ImGui::Button("Open Wave")) s_waveOpen = true; }
            else {
                bool show = s_waveOpen; params.applyWave(show, textureNeedsUpdate);
                if (!show) { s_waveOpen = false; gPreviewCacheNeedsUpdate = true; }
            }
            break;
        }
        case FilterType::Purple: {
            ImGui::TextUnformatted("Purple");
            if (!s_purpleOpen) { if (ImGui::Button("Open Purple")) s_purpleOpen = true; }
            else {
                bool show = s_purpleOpen; params.applyPurple(show, textureNeedsUpdate);
                if (!show) { s_purpleOpen = false; gPreviewCacheNeedsUpdate = true; }
            }
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
            ImGui::TextUnformatted("Vignette");
            if (!s_vignetteOpen) { if (ImGui::Button("Open Vignette")) s_vignetteOpen = true; }
            else {
                bool show = s_vignetteOpen; params.applyVignette(show, textureNeedsUpdate);
                if (!show) { s_vignetteOpen = false; gPreviewCacheNeedsUpdate = true; }
            }
            break;
        }
        case FilterType::Warmth: {
            ImGui::TextUnformatted("Warmth");
            if (!s_warmthOpen) { if (ImGui::Button("Open Warmth")) s_warmthOpen = true; }
            else {
                bool show = s_warmthOpen; params.applyWarmth(show, textureNeedsUpdate);
                if (!show) { s_warmthOpen = false; gPreviewCacheNeedsUpdate = true; }
            }
            break;
        }
        case FilterType::Outline: {
            ImGui::TextUnformatted("Outline");
            if (!s_outlineOpen) {
                if (ImGui::Button("Open Outline")) s_outlineOpen = true;
            } else {
                if (ImGui::Button("Apply")) {
                    OutlineFilter f; processor.applyFilter(f);
                    textureNeedsUpdate = true; gPreviewCacheNeedsUpdate = true;
                    gPresetManager.recordStep(FilterStep{FilterType::Outline, {}, ""});
                    s_outlineOpen = false;
                }
            }
            break;
        }
        case FilterType::Resize: {
            ImGui::TextUnformatted("Resize (overlay)");
            if (!s_resizeOpen) {
                if (ImGui::Button("Open Resize")) s_resizeOpen = true;
            } else {
                bool show = s_resizeOpen;
                params.applyResize(show, textureNeedsUpdate);
                if (!show) { s_resizeOpen = false; gPreviewCacheNeedsUpdate = true; }
            }
            break;
        }
        case FilterType::Crop: {
            ImGui::TextUnformatted("Crop (overlay)");
            if (!s_cropOpen) {
                if (ImGui::Button("Open Crop")) s_cropOpen = true;
            } else {
                bool show = s_cropOpen;
                params.applyCrop(show, textureNeedsUpdate);
                if (!show) { s_cropOpen = false; gPreviewCacheNeedsUpdate = true; }
            }
            break;
        }
    }
}
