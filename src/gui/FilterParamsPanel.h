#pragma once

#include "FilterDefs.h"
#include "FilterParameters.h"
#include "PresetManager.h"
#include "RenderGUI.h"

// Forward declarations
extern bool gPreviewCacheNeedsUpdate;
#include "SelectionTools.h"
extern SelectionToolMode gSelectionTool;

struct ParamsInlineScope {
    ParamsInlineScope() { g_params_inline_mode = true; }
    ~ParamsInlineScope() { g_params_inline_mode = false; }
};

// Track open states for panels
static FilterType s_prevSelected = FilterType::None;
static FilterType s_lastAppliedFilter = FilterType::None;

// All filter panel open states
static bool s_rotateOpen = false;
static bool s_skewOpen = false;
static bool s_blurOpen = false;
static bool s_outlineOpen = false;
static bool s_purpleOpen = false;
static bool s_infraredOpen = false;
static bool s_waveOpen = false;
static bool s_oilOpen = false;
static bool s_retroOpen = false;
static bool s_vignetteOpen = false;
static bool s_warmthOpen = false;
static bool s_grayscaleOpen = false;
static bool s_invertOpen = false;
static bool s_blackAndWhiteOpen = false;
static bool s_flipOpen = false;
static bool s_brightnessOpen = false;
static bool s_contrastOpen = false;
static bool s_saturationOpen = false;
static bool s_oilPaintingOpen = false;
static bool s_frameOpen = false;
static bool s_mergeOpen = false;
static bool s_resizeOpen = false;
static bool s_cropOpen = false;

inline void onFilterApplied(FilterType ft) {
    s_lastAppliedFilter = ft;
    extern bool gHasOriginalImageForPreview;
    gHasOriginalImageForPreview = false;
}

// Helper: Check if the previous filter had a panel open with live preview
inline bool wasPreviousFilterPanelOpen() {
    switch (s_prevSelected) {
        case FilterType::Blur: return s_blurOpen;
        case FilterType::Brightness: return s_brightnessOpen;
        case FilterType::Contrast: return s_contrastOpen;
        case FilterType::Saturation: return s_saturationOpen;
        case FilterType::OilPainting: return s_oilPaintingOpen;
        case FilterType::Wave: return s_waveOpen;
        case FilterType::Purple: return s_purpleOpen;
        case FilterType::Skew: return s_skewOpen;
        case FilterType::Vignette: return s_vignetteOpen;
        case FilterType::Warmth: return s_warmthOpen;
        default: return false;
    }
}

// Helper: Close all filter panels
inline void closeAllFilterPanels() {
    s_blurOpen = false;
    s_brightnessOpen = false;
    s_contrastOpen = false;
    s_saturationOpen = false;
    s_oilPaintingOpen = false;
    s_waveOpen = false;
    s_purpleOpen = false;
    s_skewOpen = false;
    s_vignetteOpen = false;
    s_warmthOpen = false;
    s_rotateOpen = false;
    s_outlineOpen = false;
    s_infraredOpen = false;
    s_retroOpen = false;
    s_grayscaleOpen = false;
    s_invertOpen = false;
    s_blackAndWhiteOpen = false;
    s_flipOpen = false;
    s_frameOpen = false;
    s_mergeOpen = false;
    s_resizeOpen = false;
    s_cropOpen = false;
}

// Helper: Open the panel for the selected filter
inline void openFilterPanel(FilterType selected) {
    switch (selected) {
        case FilterType::Rotate: s_rotateOpen = true; break;
        case FilterType::Skew: s_skewOpen = true; break;
        case FilterType::Blur: s_blurOpen = true; break;
        case FilterType::Outline: s_outlineOpen = true; break;
        case FilterType::Purple: s_purpleOpen = true; break;
        case FilterType::Infrared: s_infraredOpen = true; break;
        case FilterType::Wave: s_waveOpen = true; break;
        case FilterType::OilPainting: s_oilPaintingOpen = true; break;
        case FilterType::Retro: s_retroOpen = true; break;
        case FilterType::Vignette: s_vignetteOpen = true; break;
        case FilterType::Warmth: s_warmthOpen = true; break;
        case FilterType::Grayscale: s_grayscaleOpen = true; break;
        case FilterType::Invert: s_invertOpen = true; break;
        case FilterType::BlackAndWhite: s_blackAndWhiteOpen = true; break;
        case FilterType::Flip: s_flipOpen = true; break;
        case FilterType::Brightness: s_brightnessOpen = true; break;
        case FilterType::Contrast: s_contrastOpen = true; break;
        case FilterType::Saturation: s_saturationOpen = true; break;
        case FilterType::Frame: s_frameOpen = true; break;
        case FilterType::Merge: s_mergeOpen = true; break;
        case FilterType::Resize: s_resizeOpen = true; break;
        case FilterType::Crop: s_cropOpen = true; break;
        default: break;
    }
}

inline void renderFilterParamsPanel(ImageProcessor &processor, FilterType selected, bool &textureNeedsUpdate)
{
    static FilterParameters params(processor);
    ParamsInlineScope scope; // ensure inline rendering within this panel
    if(selected != s_prevSelected) {
        if(wasPreviousFilterPanelOpen()) {
            extern Image gOriginalImageForPreview;
            extern bool gHasOriginalImageForPreview;
            if(gHasOriginalImageForPreview) processor.setImage(gOriginalImageForPreview);
        }
    
        textureNeedsUpdate = true;

        closeAllFilterPanels();
        clearStoredOriginalImage();

        s_prevSelected = selected;
        openFilterPanel(selected);
        switch (selected) {
            case FilterType::Blur:
            case FilterType::Brightness:
            case FilterType::Contrast:
            case FilterType::Saturation:
            case FilterType::OilPainting:
            case FilterType::Wave:
            case FilterType::Purple:
            case FilterType::Skew:
            case FilterType::Vignette:
            case FilterType::Warmth:
                storeOriginalImageForPreview(processor.getCurrentImage());
                break;
            default:
                // This filter doesn't have a live preview panel, so do nothing.
                break;
        }
        gPreviewCacheNeedsUpdate = true;
    }

    // Check if current filter should be disabled when selection tools are active
    bool shouldDisableFilter = false;
    if (selected == FilterType::Resize || selected == FilterType::Flip || selected == FilterType::Rotate || 
        selected == FilterType::Merge || selected == FilterType::Crop || selected == FilterType::Frame) {
        shouldDisableFilter = (gSelectionTool != SelectionToolMode::None);
    }
    
    if (shouldDisableFilter) ImGui::BeginDisabled();
    
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
                    GrayscaleFilter f;
                    if (processor.hasSelection()) {
                        processor.applyFilterSelective(f, processor.getSelectionInvertApply());
                    } else {
                        processor.applyFilter(f);
                    }
                    textureNeedsUpdate = true;
                    gPreviewCacheNeedsUpdate = true;
                    gPresetManager.recordStep(FilterStep{FilterType::Grayscale, {}, ""});
                    onFilterApplied(FilterType::Grayscale);
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
                    InvertFilter f;
                    if (processor.hasSelection()) {
                        processor.applyFilterSelective(f, processor.getSelectionInvertApply());
                    } else {
                        processor.applyFilter(f);
                    }
                    textureNeedsUpdate = true;
                    gPreviewCacheNeedsUpdate = true;
                    gPresetManager.recordStep(FilterStep{FilterType::Invert, {}, ""});
                    onFilterApplied(FilterType::Invert);
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
                    BlackAndWhiteFilter f;
                    if (processor.hasSelection()) {
                        processor.applyFilterSelective(f, processor.getSelectionInvertApply());
                    } else {
                        processor.applyFilter(f);
                    }
                    textureNeedsUpdate = true;
                    gPreviewCacheNeedsUpdate = true;
                    gPresetManager.recordStep(FilterStep{FilterType::BlackAndWhite, {}, ""});
                    onFilterApplied(FilterType::BlackAndWhite);
                    s_blackAndWhiteOpen = false;
                }
            }
            break;
        }
        
        case FilterType::Flip: {
            ImGui::TextUnformatted("Flip");
            static bool horizontalFlip = false;
            static bool verticalFlip = false;
            
            if (!s_flipOpen) {
                if (ImGui::Button("Open Flip")) s_flipOpen = true;
            } else {
                ImGui::Checkbox("Horizontal Flip", &horizontalFlip);
                ImGui::Checkbox("Vertical Flip", &verticalFlip);
                
                if (ImGui::Button("Apply")) {
                    if (horizontalFlip || verticalFlip) {
                        FlipFilter f(horizontalFlip, verticalFlip);
                        processor.applyFilter(f);
                        textureNeedsUpdate = true;
                        gPreviewCacheNeedsUpdate = true;
                        gPresetManager.recordStep(FilterStep{FilterType::Flip, {(double)(horizontalFlip ? 1.0 : 0.0), (double)(verticalFlip ? 1.0 : 0.0)}, ""});
                        onFilterApplied(FilterType::Flip);
                    }
                    s_flipOpen = false;
                    horizontalFlip = false;
                    verticalFlip = false;
                }
                ImGui::SameLine();
                if (ImGui::Button("Cancel")) {
                    s_flipOpen = false;
                    horizontalFlip = false;
                    verticalFlip = false;
                    gPreviewCacheNeedsUpdate = true;
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
                    RetroFilter f;
                    if (processor.hasSelection()) {
                        processor.applyFilterSelective(f, processor.getSelectionInvertApply());
                    } else {
                        processor.applyFilter(f);
                    }
                    textureNeedsUpdate = true;
                    gPreviewCacheNeedsUpdate = true;
                    gPresetManager.recordStep(FilterStep{FilterType::Retro, {}, ""});
                    onFilterApplied(FilterType::Retro);
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
                    InfraredFilter f;
                    if (processor.hasSelection()) {
                        processor.applyFilterSelective(f, processor.getSelectionInvertApply());
                    } else {
                        processor.applyFilter(f);
                    }
                    textureNeedsUpdate = true;
                    gPreviewCacheNeedsUpdate = true;
                    gPresetManager.recordStep(FilterStep{FilterType::Infrared, {}, ""});
                    onFilterApplied(FilterType::Infrared);
                    s_infraredOpen = false;
                }
            }
            break;
        }

        case FilterType::Outline: {
            ImGui::TextUnformatted("Outline");
            if (!s_outlineOpen) {
                if (ImGui::Button("Open Outline")) s_outlineOpen = true;
            } else {
                if (ImGui::Button("Apply")) {
                    OutlineFilter f;
                    if (processor.hasSelection()) {
                        processor.applyFilterSelective(f, processor.getSelectionInvertApply());
                    } else {
                        processor.applyFilter(f);
                    }
                    textureNeedsUpdate = true;
                    gPreviewCacheNeedsUpdate = true;
                    gPresetManager.recordStep(FilterStep{FilterType::Outline, {}, ""});
                    onFilterApplied(FilterType::Outline);
                    s_outlineOpen = false;
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
                bool show = s_mergeOpen;
                params.applyMerge(show, textureNeedsUpdate);
                if (!show) {
                    s_mergeOpen = false;
                    gPreviewCacheNeedsUpdate = true;
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
                if (!show) {
                    s_blurOpen = false;
                    gPreviewCacheNeedsUpdate = true;
                }
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
                if (!show) {
                    s_brightnessOpen = false;
                    gPreviewCacheNeedsUpdate = true;
                }
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
                if (!show) {
                    s_contrastOpen = false;
                    gPreviewCacheNeedsUpdate = true;
                }
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
                if (!show) {
                    s_saturationOpen = false;
                    gPreviewCacheNeedsUpdate = true;
                }
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
                if (!show) {
                    s_oilPaintingOpen = false;
                    gPreviewCacheNeedsUpdate = true;
                }
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
                if (!show) {
                    s_rotateOpen = false;
                    gPreviewCacheNeedsUpdate = true;
                }
            }
            break;
        }
        
        case FilterType::Wave: {
            ImGui::TextUnformatted("Wave");
            if (!s_waveOpen) {
                if (ImGui::Button("Open Wave")) s_waveOpen = true;
            } else {
                bool show = s_waveOpen;
                params.applyWave(show, textureNeedsUpdate);
                if (!show) {
                    s_waveOpen = false;
                    gPreviewCacheNeedsUpdate = true;
                }
            }
            break;
        }
        
        case FilterType::Purple: {
            ImGui::TextUnformatted("Purple");
            if (!s_purpleOpen) {
                if (ImGui::Button("Open Purple")) s_purpleOpen = true;
            } else {
                bool show = s_purpleOpen;
                params.applyPurple(show, textureNeedsUpdate);
                if (!show) {
                    s_purpleOpen = false;
                    gPreviewCacheNeedsUpdate = true;
                }
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
                if (!show) {
                    s_skewOpen = false;
                    gPreviewCacheNeedsUpdate = true;
                }
            }
            break;
        }
        
        case FilterType::Vignette: {
            ImGui::TextUnformatted("Vignette");
            if (!s_vignetteOpen) {
                if (ImGui::Button("Open Vignette")) s_vignetteOpen = true;
            } else {
                bool show = s_vignetteOpen;
                params.applyVignette(show, textureNeedsUpdate);
                if (!show) {
                    s_vignetteOpen = false;
                    gPreviewCacheNeedsUpdate = true;
                }
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
        case FilterType::Resize: {
            ImGui::TextUnformatted("Resize (overlay)");
            if (!s_resizeOpen) {
                if (ImGui::Button("Open Resize")) s_resizeOpen = true;
            } else {
                bool show = s_resizeOpen;
                params.applyResize(show, textureNeedsUpdate);
                if (!show) {
                    s_resizeOpen = false;
                    gPreviewCacheNeedsUpdate = true;
                }
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
                if (!show) {
                    s_cropOpen = false;
                    gPreviewCacheNeedsUpdate = true;
                }
            }
            break;
        }
    }
    
    if (shouldDisableFilter) ImGui::EndDisabled();
}