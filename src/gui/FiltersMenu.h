#pragma once
#include "core/ImageProcessor.h"
#include "FilterParameters.h"

void filtersMenu(ImageProcessor &processor) {
        if (ImGui::CollapsingHeader("Filters", ImGuiTreeNodeFlags_DefaultOpen)) {

        FilterParameters filterParams(processor);

        if (ImGui::Button("Grayscale")) filterParams.applyGrayscale();
        if (ImGui::Button("Invert")) filterParams.applyInvert();
        if (ImGui::Button("Blur")) filterParams.applyBlur();
        if (ImGui::Button("Black & white")) filterParams.applyBlackAndWhite();
        //TODO : add parameters
        if (ImGui::Button("Crop")) filterParams.applyCrop();
        if (ImGui::Button("Resize")) filterParams.applyResize();
        //TODO: ADD SLIDERS
        if (ImGui::Button("Darken")) filterParams.applyDarken();
        //TODO: ADD Sliders
        if (ImGui::Button("Lighten")) filterParams.applyLighten();
        //TODO: add parameter here
        if (ImGui::Button("Frame")) filterParams.applyFrame();
        if (ImGui::Button("Horizontal Flip")) filterParams.applyHorizontalFlip();
        if (ImGui::Button("Vertical Flip")) filterParams.applyVerticalFlip();
        if (ImGui::Button("Merge")) filterParams.applyMerge();
        //TODO: add parameter
        if (ImGui::Button("Rotate")) filterParams.applyRotate();
        if (ImGui::Button("Outline")) filterParams.applyOutline();
        if (ImGui::Button("Purple")) filterParams.applyPurple();
        if (ImGui::Button("Infrared")) filterParams.applyInfrared();
        //TODO: add slider
        if (ImGui::Button("Wave")) filterParams.applyWave();
        if (ImGui::Button("Oil Painting")) filterParams.applyOilPainting();
        if (ImGui::Button("Retro")) filterParams.applyRetro();
        //TODO: Add parameters
        if (ImGui::Button("Skew")) filterParams.applySkew();
        //TODO: Add sliders to the next 4
        if (ImGui::Button("Warmth")) filterParams.applyWarmth();
        if (ImGui::Button("Saturation")) filterParams.applySaturation();
        if (ImGui::Button("Contrast")) filterParams.applyContrast();
        if (ImGui::Button("Vigentte")) filterParams.applyVignette();
    }
}