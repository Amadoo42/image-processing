#pragma once
#include "core/ImageProcessor.h"
#include "FilterParameters.h"

void filtersMenu(ImageProcessor &processor) {
        if (ImGui::CollapsingHeader("Filters", ImGuiTreeNodeFlags_DefaultOpen)) {
        if (ImGui::Button("Grayscale")) applyGrayscale();
        if (ImGui::Button("Invert")) applyInvert();
        if (ImGui::Button("Blur")) applyBlur();
        if (ImGui::Button("Black & white")) applyBlackAndWhite();
        //TODO : add parameters
        if (ImGui::Button("Crop")) applyCrop();
        if (ImGui::Button("Resize")) applyResize();
        //TODO: ADD SLIDERS
        if (ImGui::Button("Darken")) applyDarken();
        //TODO: ADD Sliders
        if (ImGui::Button("Lighten")) applyLighten();
        //TODO: add parameter here
        if (ImGui::Button("Frame")) applyFrame();
        if (ImGui::Button("Horizontal Flip")) applyHorizontalFlip();
        if (ImGui::Button("Vertical Flip")) applyVerticalFlip();
        if (ImGui::Button("Merge")) applyMerge();
        //TODO: add parameter
        if (ImGui::Button("Rotate")) applyRotate();
        if (ImGui::Button("Outline")) applyOutline();
        if (ImGui::Button("Purple")) applyPurple();
        if (ImGui::Button("Infrared")) applyInfrared();
        //TODO: add slider
        if (ImGui::Button("Wave")) applyWave();
        if (ImGui::Button("Oil Painting")) applyOilPainting();
        if (ImGui::Button("Retro")) applyRetro();
        //TODO: Add parameters
        if (ImGui::Button("Skew")) applySkew();
        //TODO: Add sliders to the next 4
        if (ImGui::Button("Warmth")) applyWarmth();
        if (ImGui::Button("Saturation")) applySaturation();
        if (ImGui::Button("Contrast")) applyContrast();
        if (ImGui::Button("Vigentte")) applyVigentte();
    }
}