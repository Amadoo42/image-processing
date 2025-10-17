#pragma once
#include "imgui.h"
#include "../FilterParamsPanel.h"
#include "../LoadTexture.h"
#include "../../filters/ResizeFilter.h"

static void drawLeftParamsPanel(ImageProcessor &processor, float width) {
    ImGui::BeginChild("LeftParamsPanel", ImVec2(width, 0), true);
    const Image &__img_left = processor.getCurrentImage();
    bool __hasImageLeft = (__img_left.width > 0 && __img_left.height > 0);
    ImGui::TextUnformatted("Filter Parameters");
    ImGui::Separator();
    if (!__hasImageLeft) {
        ImGui::TextDisabled("No image loaded.");
    } else {
        renderFilterParamsPanel(processor, gSelectedFilter, textureNeedsUpdate);
    }

    ImGui::NewLine();
    ImGui::TextUnformatted("History");
    ImGui::Separator();
    if (!__hasImageLeft) {
        ImGui::TextDisabled("Open an image to see history.");
    } else {
        static std::vector<GLuint> historyTextures;
        static bool historyValid = false;
        static size_t lastUndoCount = (size_t)-1;

        auto releaseHistoryTextures = [&]() {
            for (GLuint t : historyTextures) {
                if (t != 0) glDeleteTextures(1, &t);
            }
            historyTextures.clear();
        };

        const auto &undoH = processor.getUndoHistory();
        bool needRebuild = !historyValid || textureNeedsUpdate || gPreviewCacheNeedsUpdate || (undoH.size() != lastUndoCount);
        if (needRebuild) {
            releaseHistoryTextures();

            std::vector<const Image*> states;
            for (const Image &img : undoH) states.push_back(&img);
            states.push_back(&processor.getCurrentImage());

            historyTextures.reserve(states.size());

            const int targetW = 120;
            const int targetH = 90;
            for (const Image* src : states) {
                if (!src || src->width <= 0 || src->height <= 0) { historyTextures.push_back(0); continue; }
                float scale = std::min(targetW / (float)src->width, targetH / (float)src->height);
                int newW = std::max(1, (int)std::floor(src->width * scale));
                int newH = std::max(1, (int)std::floor(src->height * scale));
                Image thumb = *src;
                if (newW != src->width || newH != src->height) {
                    ResizeFilter rf(newW, newH);
                    rf.apply(thumb);
                }
                GLuint tex = loadTexture(thumb);
                historyTextures.push_back(tex);
            }
            lastUndoCount = undoH.size();
            historyValid = true;
        }

        const ImVec2 thumbSize = ImVec2(120, 90);
        ImGui::BeginChild("HistoryList", ImVec2(0, 0), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);
        for (size_t i = 0; i < historyTextures.size(); ++i) {
            GLuint tex = historyTextures[i];
            if (tex != 0) {
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 4.0f);
                ImGui::Image((void*)(intptr_t)tex, thumbSize);
            } else {
                ImGui::Dummy(thumbSize);
            }
            if (i + 1 < historyTextures.size()) ImGui::Separator();
        }
        ImGui::EndChild();
    }

    ImGui::EndChild();
}
