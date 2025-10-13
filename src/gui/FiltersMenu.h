#pragma once
#include "core/ImageProcessor.h"
#include "FilterParameters.h"
#include "RenderGUI.h"

void filtersMenu(ImageProcessor &processor, bool &textureNeedsUpdate) {
    FilterParameters para(processor);

    if (ImGui::CollapsingHeader("Filters", ImGuiTreeNodeFlags_DefaultOpen)) {
        int count = 0;
        auto addButton = [&](const char* label, auto&& func) {
            ImGui::Indent(20.0f);  // Indent each filter inside its category
            if (ImGui::Button(label, ImVec2(150, 0))) {
                func();
            }
            ImGui::Unindent(20.0f);
            count++;
            if (count % 1 != 0) ImGui::SameLine();
        };

        static bool showGrayscale       = false;
        static bool showInvert          = false;
        static bool showBlur            = false;
        static bool showBlackWhite      = false;
        static bool showCrop            = false;
        static bool showResize          = false;
        static bool showBrightness      = false;
        static bool showFrame           = false;
        static bool showHorizontalFlip  = false;
        static bool showVerticalFlip    = false;
        static bool showMerge           = false;
        static bool showRotate          = false;
        static bool showOutline         = false;
        static bool showPurple          = false;
        static bool showInfrared        = false;
        static bool showWave            = false;
        static bool showOilPainting     = false;
        static bool showRetro           = false;
        static bool showSaturation      = false;
        static bool showContrast        = false;
        static bool showSkew            = false;
        static bool showVignette        = false;
        static bool showWarmth          = false;

        if (ImGui::CollapsingHeader("Basic Adjustments", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Indent(20.0f);
            count = 0;
            addButton("Grayscale",       [&] { showGrayscale = true; });
            addButton("Invert",          [&] { showInvert = true; });
            addButton("Black & White",   [&] { showBlackWhite = true; });
            addButton("Brightness",      [&] { showBrightness = true; });
            addButton("Contrast",        [&] { showContrast = true; });
            addButton("Saturation",      [&] { showSaturation = true; });
            ImGui::Unindent(20.0f);
            ImGui::NewLine();
        }

        if (ImGui::CollapsingHeader("Transformations", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Indent(20.0f);
            count = 0;
            addButton("Crop",             [&] { showCrop = true; });
            addButton("Resize",           [&] { showResize = true; });
            addButton("Horizontal Flip",  [&] { showHorizontalFlip = true; });
            addButton("Vertical Flip",    [&] { showVerticalFlip = true; });
            addButton("Rotate",           [&] { showRotate = true; });
            addButton("Skew",             [&] { showSkew = true; });
            addButton("Merge",            [&] { showMerge = true; });
            ImGui::Unindent(20.0f);
            ImGui::NewLine();
        }

        if (ImGui::CollapsingHeader("Effects", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Indent(20.0f);
            count = 0;
            addButton("Blur",            [&] { showBlur = true; });
            addButton("Frame",           [&] { showFrame = true; });
            addButton("Outline",         [&] { showOutline = true; });
            addButton("Purple",          [&] { showPurple = true; });
            addButton("Infrared",        [&] { showInfrared = true; });
            addButton("Wave",            [&] { showWave = true; });
            addButton("Oil Painting",    [&] { showOilPainting = true; });
            addButton("Retro",          [&] { showRetro = true; });
            addButton("Vignette",       [&] { showVignette = true; });
            addButton("Warmth",         [&] { showWarmth = true; });
            ImGui::Unindent(20.0f);
            ImGui::NewLine();
        }

        para.applyGrayscale(showGrayscale, textureNeedsUpdate);
        para.applyInvert(showInvert, textureNeedsUpdate);
        para.applyBlur(showBlur, textureNeedsUpdate);
        para.applyBlackAndWhite(showBlackWhite, textureNeedsUpdate);
        para.applyCrop(showCrop, textureNeedsUpdate);
        para.applyResize(showResize, textureNeedsUpdate);
        para.applyBrightness(showBrightness, textureNeedsUpdate);
        para.applyFrame(showFrame, textureNeedsUpdate);
        para.applyHorizontalFlip(showHorizontalFlip, textureNeedsUpdate);
        para.applyVerticalFlip(showVerticalFlip, textureNeedsUpdate);
        para.applyMerge(showMerge, textureNeedsUpdate);
        para.applyRotate(showRotate, textureNeedsUpdate);
        para.applyOutline(showOutline, textureNeedsUpdate);
        para.applyPurple(showPurple, textureNeedsUpdate);
        para.applyInfrared(showInfrared, textureNeedsUpdate);
        para.applyWave(showWave, textureNeedsUpdate);
        para.applyOilPainting(showOilPainting, textureNeedsUpdate);
        para.applyRetro(showRetro, textureNeedsUpdate);
        para.applySaturation(showSaturation, textureNeedsUpdate);
        para.applyContrast(showContrast, textureNeedsUpdate);
        para.applySkew(showSkew, textureNeedsUpdate);
        para.applyVignette(showVignette, textureNeedsUpdate);
        para.applyWarmth(showWarmth, textureNeedsUpdate);
    }
}
