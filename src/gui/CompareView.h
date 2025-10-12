#pragma once
#include "imgui.h"
#include "LoadTexture.h"
#include "MemoryOperation.h"
#include <algorithm>
#include <cstdio>

void renderCompareView(ImageProcessor &processor, float &zoom_level, ImVec2 &pan_offset) {
    const Image& currentImage = processor.getCurrentImage();
    if (currentImage.width <= 0 || currentImage.height <= 0) return;

    GLuint currentTextureID = loadTexture(currentImage);

    ImageProcessor Copy = processor;
    Copy.undo();
    const Image& beforeImage = Copy.getCurrentImage(); 
    GLuint beforeTextureID = currentTextureID;
    if (beforeImage.width > 0 && beforeImage.height > 0) {
        beforeTextureID = loadTexture(beforeImage);
    }

    ImGuiIO& io = ImGui::GetIO();

    if (ImGui::IsWindowHovered() && io.MouseWheel != 0.0f) {
        zoom_level += io.MouseWheel * 0.1f;
        zoom_level = std::clamp(zoom_level, 0.1f, 10.0f);
    }
    if (ImGui::IsWindowHovered() && ImGui::IsMouseDragging(ImGuiMouseButton_Right)) {
        pan_offset.x += io.MouseDelta.x;
        pan_offset.y += io.MouseDelta.y;
    }

    ImVec2 zoomed_size = ImVec2(currentImage.width * zoom_level, currentImage.height * zoom_level);
    ImVec2 image_pos = ImGui::GetCursorScreenPos();
    ImVec2 p_min = ImVec2(image_pos.x + pan_offset.x, image_pos.y + pan_offset.y);
    ImVec2 p_max = ImVec2(p_min.x + zoomed_size.x, p_min.y + zoomed_size.y);

    static float split_ratio = 0.5f;
    split_ratio = std::clamp(split_ratio, 0.0f, 1.0f);

    float mid_x = p_min.x + zoomed_size.x * split_ratio;

    ImDrawList* dl = ImGui::GetWindowDrawList();

    ImVec2 left_min = p_min;
    ImVec2 left_max = ImVec2(mid_x, p_max.y);
    ImVec2 uv0_left = ImVec2(0.0f, 0.0f);
    ImVec2 uv1_left = ImVec2(split_ratio, 1.0f);
    if (left_max.x > left_min.x && left_max.y > left_min.y)
        dl->AddImage((void*)(intptr_t)beforeTextureID, left_min, left_max, uv0_left, uv1_left);

    ImVec2 right_min = ImVec2(mid_x, p_min.y);
    ImVec2 right_max = p_max;
    ImVec2 uv0_right = ImVec2(split_ratio, 0.0f);
    ImVec2 uv1_right = ImVec2(1.0f, 1.0f);
    if (right_max.x > right_min.x && right_max.y > right_min.y)
        dl->AddImage((void*)(intptr_t)currentTextureID, right_min, right_max, uv0_right, uv1_right);

    float handle_half_w = 4.0f;
    ImVec2 handle_min = ImVec2(mid_x - handle_half_w, p_min.y);
    ImVec2 handle_max = ImVec2(mid_x + handle_half_w, p_max.y);

    
    ImGui::SetCursorScreenPos(ImVec2(handle_min.x, handle_min.y));
    ImGui::InvisibleButton("split_drag", ImVec2(handle_half_w * 2.0f, zoomed_size.y));
    bool dragging = ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left);
    if (ImGui::IsItemActive()) {
        ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
    }
    if (dragging) {
        float dx = ImGui::GetIO().MouseDelta.x;
        split_ratio += dx / zoomed_size.x;
        split_ratio = std::clamp(split_ratio, 0.0f, 1.0f);
        mid_x = p_min.x + zoomed_size.x * split_ratio;
        handle_min.x = mid_x - handle_half_w;
        handle_max.x = mid_x + handle_half_w;
    }

    dl->AddRectFilled(handle_min, handle_max, IM_COL32(60, 60, 60, 200), 0.0f);
    dl->AddLine(ImVec2(mid_x, p_min.y + 8.0f), ImVec2(mid_x, p_max.y - 8.0f), IM_COL32(200,200,200,180), 1.0f);

   
    char pctbuf[32];
    float x = 1.0 - split_ratio;
    std::snprintf(pctbuf, sizeof(pctbuf), "After: %.0f%%", x * 100.0f);
    ImVec2 txt_sz = ImGui::CalcTextSize(pctbuf);
    ImVec2 txt_pos = ImVec2(mid_x + 8.0f, p_min.y + 8.0f);
    dl->AddRectFilled(ImVec2(txt_pos.x - 6.0f, txt_pos.y - 4.0f), ImVec2(txt_pos.x + txt_sz.x + 6.0f, txt_pos.y + txt_sz.y + 4.0f), IM_COL32(0,0,0,160), 4.0f);
    dl->AddText(txt_pos, IM_COL32(255,255,255,220), pctbuf);

    
    ImGui::SetCursorScreenPos(image_pos);
}
