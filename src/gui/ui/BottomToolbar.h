#pragma once
#include "imgui.h"

static void drawBottomToolbar(ImageProcessor &processor, float fullWidth) {
    float height = 52.0f;
    ImGui::BeginChild("BottomToolbar", ImVec2(0, height), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    
    if (ImGui::BeginTable("BottomToolbarTable", 2, ImGuiTableFlags_None)) {
        ImGui::TableSetupColumn("Left", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("Right", ImGuiTableColumnFlags_WidthStretch);
        
        ImGui::TableNextRow();
        ImGui::TableNextColumn();

        const Image &__img_toolbar = processor.getCurrentImage();
        bool __hasImageToolbar = (__img_toolbar.width > 0 && __img_toolbar.height > 0);

        if (ImGui::Button(iconLabel(ICON_FA_ROTATE_LEFT, "Undo").c_str())) {
            if (processor.undo()) {
                textureNeedsUpdate = true;
                gPreviewCacheNeedsUpdate = true;
                gSelectedFilter = FilterType::None;
                gPresetManager.clearSession();
                statusBarMessage = "Undo successful.";
            } else {
                statusBarMessage = "Nothing to undo.";
            }
        }
        ImGui::SameLine();

        if (ImGui::Button(iconLabel(ICON_FA_ROTATE_RIGHT, "Redo").c_str())) {
            if (processor.redo()) {
                textureNeedsUpdate = true;
                gPreviewCacheNeedsUpdate = true;
                gSelectedFilter = FilterType::None;
                gPresetManager.clearSession();
                statusBarMessage = "Redo successful.";
            } else {
                statusBarMessage = "Nothing to redo.";
            }
        }
        ImGui::SameLine();

        if (!__hasImageToolbar) ImGui::BeginDisabled();
        if (ImGui::Button(iconLabel(ICON_FA_ARROWS_ROTATE, "Reset").c_str())) {
            zoom_level = 1.0f;
            pan_offset = ImVec2(0, 0);
        }
        if (!__hasImageToolbar) ImGui::EndDisabled();

        ImGui::SameLine();
        ImGui::Dummy(ImVec2(8, 1));
        ImGui::SameLine();

        ImGui::TextUnformatted("Zoom");
        ImGui::SameLine();
        float percent = zoom_level * 100.0f;
        ImGui::SetNextItemWidth(160.0f);
        if (!__hasImageToolbar) ImGui::BeginDisabled();
        if (ImGui::SliderFloat("##zoom", &percent, 10.0f, 400.0f, "%.0f%%")) {
            zoom_level = percent / 100.0f;
        }
        ImGui::SameLine();
        if (ImGui::Button("Fit")) {
            zoom_level = 1.0f; pan_offset = ImVec2(0, 0);
            const Image& img = processor.getCurrentImage();
            if (img.width > 0 && img.height > 0 && gLastCanvasAvail.x > 0.0f && gLastCanvasAvail.y > 0.0f) {
                float zx = gLastCanvasAvail.x / img.width;
                float zy = gLastCanvasAvail.y / img.height;
                zoom_level = std::max(0.1f, std::min(zx, zy));
            }
        }
        if (!__hasImageToolbar) ImGui::EndDisabled();

        ImGui::TableNextColumn();
        
        const Image& img = processor.getCurrentImage();
        auto gcd = [](int a, int b){ while(b){ int t=a%b; a=b; b=t;} return std::max(1, a); };
        int g = (img.width>0 && img.height>0) ? gcd(img.width, img.height) : 1;
        int arw = (img.width>0)? img.width / g : 0;
        int arh = (img.height>0)? img.height / g : 0;
        std::string fname = gCurrentImagePath;
        size_t pos = fname.find_last_of("/\\");
        if (pos != std::string::npos) fname = fname.substr(pos + 1);
        if (fname.empty()) fname = "Untitled";
        
        char infoBuf[256];
        if (img.width > 0 && img.height > 0)
            std::snprintf(infoBuf, sizeof(infoBuf), "%s | %dx%d | %d:%d | %.0f%%",
                        fname.c_str(), img.width, img.height, arw, arh, zoom_level * 100.0f);
        else
            std::snprintf(infoBuf, sizeof(infoBuf), "%s", fname.c_str());

        float statusWidth = ImGui::CalcTextSize(statusBarMessage.c_str()).x;
        float infoWidth = ImGui::CalcTextSize(infoBuf).x;
        float gap = 12.0f;
        float compositeWidth = statusWidth + gap + infoWidth;
        float avail = ImGui::GetContentRegionAvail().x;
        
        float startX = ImGui::GetCursorPosX() + std::max(0.0f, avail - compositeWidth);
        
        ImGui::SetCursorPosX(startX);
        ImGui::TextUnformatted(statusBarMessage.c_str());
        
        ImGui::SameLine(0.0f, gap);
        ImGui::TextUnformatted(infoBuf);

        ImGui::EndTable();
    }

    ImGui::EndChild();
}
