#pragma once

#include "../core/ImageProcessor.h"
#include "MemoryOperation.h"
#include "LoadTexture.h"
#include "../filters/GrayscaleFilter.h"
#include "../filters/InvertFilter.h"
#include "../filters/BlurFilter.h"
#include "../filters/BlackAndWhiteFilter.h"
#include "../filters/CropFilter.h"
#include "../filters/ResizeFilter.h"
#include "../filters/DarkenFilter.h"
#include "../filters/LightenFilter.h"
#include "../filters/FrameFilter.h"
#include "../filters/HorizontalFlipFilter.h"
#include "../filters/VerticalFlipFilter.h"
#include "../filters/MergeFilter.h"
#include "../filters/RotateFilter.h"
#include "../filters/OutlineFilter.h"
#include "../filters/PurpleFilter.h"
#include "../filters/InfraredFilter.h"
#include "../filters/WaveFilter.h"
#include "../filters/OilPaintingFilter.h"
#include "../filters/ContrastFilter.h"
#include "../filters/RetroFilter.h"
#include "../filters/SaturationFilter.h"
#include "../filters/SkewFilter.h"
#include "../filters/VigentteFilter.h"
#include "../filters/WarmthFilter.h"

inline ImVec2 operator+(const ImVec2& a, const ImVec2& b) { return ImVec2(a.x + b.x, a.y + b.y); }
inline ImVec2 operator-(const ImVec2& a, const ImVec2& b) { return ImVec2(a.x - b.x, a.y - b.y); }
inline ImVec2 operator*(const ImVec2& a, float b) { return ImVec2(a.x * b, a.y * b); }

// Inline rendering flag for parameter UIs. When true, parameter widgets
// are rendered inline without opening separate windows.
static bool g_params_inline_mode = false;

inline bool BeginParamsUI(const char* title, bool* p_open)
{
    if (g_params_inline_mode) {
        ImGui::TextUnformatted(title);
        ImGui::Separator();
        return true;
    }
    return ImGui::Begin(title, p_open);
}

inline void EndParamsUI()
{
    if (!g_params_inline_mode) ImGui::End();
}

class FilterParameters {
public:
    FilterParameters(ImageProcessor &processor) : processor(processor) {};

    void applyGrayscale(bool &show, bool &textureNeedsUpdate) {
        if(show){
            GrayscaleFilter filter;
            processor.applyFilter(filter);
            std::cout << "Applied Grayscale Filter\n";
            
            show = false;
            textureNeedsUpdate = true;
        }
    }
    void applyInvert(bool &show, bool &textureNeedsUpdate) {
        if(show){
            InvertFilter filter;
            processor.applyFilter(filter);
            std::cout << "Applied Invert Filter\n";
            
            show = false;
            textureNeedsUpdate = true;       
        }
    }

    void applyBlur(bool &show, bool &textureNeedsUpdate) {
        static int currentItem = 0;
        const char* items[] = {"Low", "Medium", "High"};
        int values1[] = {3, 7, 15};
        double values2[] = {0.8, 1.6, 3.0};

        static Image originalImage;
        static bool init = false;
        if(show){
            if (!BeginParamsUI("Blur Parameters", &show)) return;

            if(!init){
                originalImage = processor.getCurrentImage();
                init = true;
            }

            ImGui::Text("Intenisty:");
            ImGui::SameLine();
            bool changed = false;
            if(ImGui::Combo("##Intenisty", &currentItem, items, IM_ARRAYSIZE(items))) changed = true;

            if(changed){
                processor.setImage(originalImage);
                BlurFilter filter(values1[currentItem], values2[currentItem]); 
                processor.applyFilter(filter);
                textureNeedsUpdate = true;
            }
            ImGui::Separator();

            if(ImGui::Button("Apply")){
                BlurFilter filter(values1[currentItem], values2[currentItem]); 
                processor.setImage(originalImage);
                processor.applyFilter(filter);
                show = false;
                init = false;
                textureNeedsUpdate = true;
            }

            ImGui::SameLine();
            if(ImGui::Button("Cancel")){
                processor.setImage(originalImage);
                show = false;
                init = false;
                textureNeedsUpdate = true;
            }

            EndParamsUI();
        }else init = false;
    }
    void applyBlackAndWhite(bool &show, bool &textureNeedsUpdate) {
        if(show){
            BlackAndWhiteFilter filter;
            processor.applyFilter(filter);
            std::cout << "Applied Black and White Filter\n";
            
            show = false;
            textureNeedsUpdate = true;
        }
    }

    void applyCrop(bool &show, bool &textureNeedsUpdate) {
        static int posX = 50, posY = 50, newWidth = 200, newHeight = 200;
        static Image originalImage;
        static bool init = false;
        static GLuint textureID = 0;

        if (!show) { 
            if (textureID != 0) {
                glDeleteTextures(1, &textureID);
                textureID = 0;
            }
            init = false; 
            return; 
        }

        ImGuiIO& io = ImGui::GetIO();

        if (!init) {
            ImGui::OpenPopup("Crop Overlay");
            originalImage = processor.getCurrentImage();
            posX = 0;
            posY = 0;
            newWidth = originalImage.width;
            newHeight = originalImage.height;

            textureID = loadTexture(originalImage);
            init = true;
        }

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(io.DisplaySize);

        ImGuiWindowFlags flags =
            ImGuiWindowFlags_NoDecoration |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoSavedSettings |
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoScrollbar |
            ImGuiWindowFlags_NoScrollWithMouse;

        if (!ImGui::BeginPopupModal("Crop Overlay", &show, flags)) {
            return;
        }

        ImDrawList* draw = ImGui::GetWindowDrawList();
        draw->AddRectFilled(ImVec2(0, 0), io.DisplaySize, IM_COL32(0, 0, 0, 220));

        bool changed = false;

        float displayedWidth = (float)originalImage.width;
        float displayedHeight = (float)originalImage.height;

        float scale = 1.0f;
        if (displayedWidth > io.DisplaySize.x) scale = io.DisplaySize.x / displayedWidth;
        if (displayedHeight * scale > io.DisplaySize.y) scale = io.DisplaySize.y / displayedHeight;
        displayedWidth *= scale;
        displayedHeight *= scale;

        ImVec2 imagePos(
            (io.DisplaySize.x - displayedWidth) * 0.5f,
            (io.DisplaySize.y - displayedHeight) * 0.5f
        );

        ImVec2 imageMax(imagePos.x + displayedWidth, imagePos.y + displayedHeight);

        draw->AddImage((void*)(intptr_t)textureID, imagePos, imageMax, ImVec2(0, 0), ImVec2(1, 1));

        float scaleX = displayedWidth / (float)originalImage.width;
        float scaleY = displayedHeight / (float)originalImage.height;

        ImVec2 cropMin(imagePos.x + posX * scaleX, imagePos.y + posY * scaleY);
        ImVec2 cropMax(cropMin.x + newWidth * scaleX, cropMin.y + newHeight * scaleY);

        draw->AddRectFilled(imagePos, ImVec2(imageMax.x, cropMin.y), IM_COL32(0, 0, 0, 160));
        draw->AddRectFilled(ImVec2(imagePos.x, cropMax.y), imageMax, IM_COL32(0, 0, 0, 160));
        draw->AddRectFilled(ImVec2(imagePos.x, cropMin.y), ImVec2(cropMin.x, cropMax.y), IM_COL32(0, 0, 0, 160));
        draw->AddRectFilled(ImVec2(cropMax.x, cropMin.y), ImVec2(imageMax.x, cropMax.y), IM_COL32(0, 0, 0, 160));
        draw->AddRect(cropMin, cropMax, IM_COL32(255, 255, 255, 255), 0, 0, 2.0f);

        // Movable & resizable mini window (top-left aligned)
        static ImVec2 panelPos = ImVec2(0, 0);
        static ImVec2 panelSize = ImVec2(360, 220);
        ImVec2 winPos = ImGui::GetWindowPos();
        ImVec2 panelTL = winPos + panelPos;
        ImVec2 panelBR = ImVec2(panelTL.x + panelSize.x, panelTL.y + panelSize.y);
        const float headerH = 28.0f;

        // Panel background and border
        draw->AddRectFilled(panelTL, panelBR, IM_COL32(20, 20, 20, 230), 6.0f);
        draw->AddRect(panelTL, panelBR, IM_COL32(255, 255, 255, 64), 6.0f, 0, 1.5f);

        // Panel contents
        ImGui::SetCursorPos(panelPos + ImVec2(0, headerH));
        ImGui::BeginChild("Crop Controls", panelSize - ImVec2(0, headerH), false,
                          ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
        ImGui::Dummy(ImVec2(0, 4));
        ImGui::Text("Crop Parameters");
        ImGui::Separator();
        if (ImGui::InputInt("X", &posX)) changed = true;
        if (ImGui::InputInt("Y", &posY)) changed = true;
        if (ImGui::InputInt("Width", &newWidth)) changed = true;
        if (ImGui::InputInt("Height", &newHeight)) changed = true;

        if (ImGui::Button("Apply")) {
            processor.setImage(originalImage);
            CropFilter f(posX, posY, newWidth, newHeight);
            processor.applyFilter(f);
            textureNeedsUpdate = true;
            if (textureID != 0) { glDeleteTextures(1, &textureID); textureID = 0; }
            ImGui::CloseCurrentPopup();
            show = false;
            init = false;
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
            processor.setImage(originalImage);
            textureNeedsUpdate = true;
            if (textureID != 0) { glDeleteTextures(1, &textureID); textureID = 0; }
            ImGui::CloseCurrentPopup();
            show = false;
            init = false;
        }
        ImGui::EndChild();

        // Header (move) and size grip (resize)
        ImVec2 headerBR = ImVec2(panelBR.x, panelTL.y + headerH);
        draw->AddRectFilled(panelTL, headerBR, IM_COL32(45, 45, 45, 255), 6.0f);
        draw->AddText(ImVec2(panelTL.x + 8, panelTL.y + 6), IM_COL32(255,255,255,255), "Crop Controls");
        ImGui::SetCursorScreenPos(panelTL);
        ImGui::InvisibleButton("##crop_panel_move", ImVec2(panelSize.x, headerH));
        static bool panelDragging = false;
        static ImVec2 panelDragStart;
        if (ImGui::IsItemActive() && ImGui::IsMouseDragging(0)) {
            if (!panelDragging) { panelDragging = true; panelDragStart = io.MousePos; }
            ImVec2 delta = io.MousePos - panelDragStart;
            panelPos.x = std::clamp(panelPos.x + delta.x, 0.0f, io.DisplaySize.x - panelSize.x);
            panelPos.y = std::clamp(panelPos.y + delta.y, 0.0f, io.DisplaySize.y - panelSize.y);
            panelDragStart = io.MousePos;
        }
        if (!ImGui::IsMouseDown(0)) panelDragging = false;

        ImVec2 gripTL = ImVec2(panelBR.x - 18, panelBR.y - 18);
        ImGui::SetCursorScreenPos(gripTL);
        ImGui::InvisibleButton("##crop_panel_resize", ImVec2(18, 18));
        draw->AddTriangleFilled(ImVec2(panelBR.x - 14, panelBR.y - 2), ImVec2(panelBR.x - 2, panelBR.y - 2), ImVec2(panelBR.x - 2, panelBR.y - 14), IM_COL32(200, 200, 200, 200));
        static bool panelResizing = false;
        static ImVec2 panelResizeStart;
        if (ImGui::IsItemActive() && ImGui::IsMouseDragging(0)) {
            if (!panelResizing) { panelResizing = true; panelResizeStart = io.MousePos; }
            ImVec2 delta = io.MousePos - panelResizeStart;
            panelSize.x = std::clamp(panelSize.x + delta.x, 240.0f, io.DisplaySize.x - panelPos.x);
            panelSize.y = std::clamp(panelSize.y + delta.y, 140.0f, io.DisplaySize.y - panelPos.y);
            panelResizeStart = io.MousePos;
            panelBR = ImVec2(panelTL.x + panelSize.x, panelTL.y + panelSize.y);
        }
        if (!ImGui::IsMouseDown(0)) panelResizing = false;

        static bool dragging = false;
        static bool resizing = false;
        static int resizeCorner = -1;
        static ImVec2 dragStart;
        static int startX, startY, startW, startH;
        ImVec2 mouse = io.MousePos;

        const float cornerSize = 15.0f;

        ImVec2 corners[4] = {
            cropMin,                                    
            ImVec2(cropMax.x, cropMin.y),          
            cropMax,                                    
            ImVec2(cropMin.x, cropMax.y)            
        };

        for (int i = 0; i < 4; i++) {
            draw->AddCircleFilled(corners[i], 6.0f, IM_COL32(255, 255, 255, 255));
            draw->AddCircle(corners[i], 6.0f, IM_COL32(0, 0, 0, 255), 0, 2.0f);
        }

        bool insideCrop = mouse.x >= cropMin.x && mouse.x <= cropMax.x && mouse.y >= cropMin.y && mouse.y <= cropMax.y;
        bool overPanelArea = mouse.x >= panelTL.x && mouse.x <= panelBR.x && mouse.y >= panelTL.y && mouse.y <= panelBR.y;
        bool overCorner = false;
        int hoveredCorner = -1;

        if (!dragging && !resizing) {
            for (int i = 0; i < 4; i++) {
                float dist = sqrtf((mouse.x - corners[i].x) * (mouse.x - corners[i].x) + 
                                (mouse.y - corners[i].y) * (mouse.y - corners[i].y));
                if (dist <= cornerSize) {
                    overCorner = true;
                    hoveredCorner = i;
                    break;
                }
            }
        }

        if (overCorner && !overPanelArea && ImGui::IsMouseClicked(0)) {
            resizing = true;
            resizeCorner = hoveredCorner;
            dragStart = mouse;
            startX = posX;
            startY = posY;
            startW = newWidth;
            startH = newHeight;
        }

        else if (insideCrop && !overCorner && !overPanelArea && ImGui::IsMouseClicked(0)) {
            dragging = true;
            dragStart = mouse;
            startX = posX;
            startY = posY;
        }

        if (resizing && ImGui::IsMouseDown(0)) {
            ImVec2 delta = ImVec2(mouse.x - dragStart.x, mouse.y - dragStart.y);
            int dx = (int)(delta.x / scaleX);
            int dy = (int)(delta.y / scaleY);

            switch (resizeCorner) {
                case 0:
                    posX = std::clamp(startX + dx, 0, startX + startW - 1);
                    posY = std::clamp(startY + dy, 0, startY + startH - 1);
                    newWidth = (startX + startW) - posX;
                    newHeight = (startY + startH) - posY;
                    break;
                case 1:
                    posY = std::clamp(startY + dy, 0, startY + startH - 1);
                    newWidth = std::clamp(startW + dx, 1, originalImage.width - startX);
                    newHeight = (startY + startH) - posY;
                    break;
                case 2:
                    newWidth = std::clamp(startW + dx, 1, originalImage.width - startX);
                    newHeight = std::clamp(startH + dy, 1, originalImage.height - startY);
                    break;
                case 3:
                    posX = std::clamp(startX + dx, 0, startX + startW - 1);
                    newWidth = (startX + startW) - posX;
                    newHeight = std::clamp(startH + dy, 1, originalImage.height - startY);
                    break;
            }
            changed = true;
        }

        if (dragging && ImGui::IsMouseDown(0)) {
            ImVec2 delta = ImVec2(mouse.x - dragStart.x, mouse.y - dragStart.y);
            int dx = (int)(delta.x / scaleX);
            int dy = (int)(delta.y / scaleY);
            posX = std::clamp(startX + dx, 0, originalImage.width - newWidth);
            posY = std::clamp(startY + dy, 0, originalImage.height - newHeight);
            changed = true;
        }

        if (!ImGui::IsMouseDown(0)) {
            dragging = false;
            resizing = false;
            resizeCorner = -1;
        }

        newWidth = std::max(1, newWidth);
        newHeight = std::max(1, newHeight);
        posX = std::clamp(posX, 0, std::max(0, originalImage.width - newWidth));
        posY = std::clamp(posY, 0, std::max(0, originalImage.height - newHeight));

        ImGui::EndPopup();
    }


    // #TODO: overlay
    void applyResize(bool &show, bool &textureNeedsUpdate) {
        static int newWidth = 0, newHeight = 0;
        static Image originalImage;
        static bool init = false;
        static GLuint textureID = 0;

        static bool resizingCorner = false;
        static bool resizingEdge = false;
        static int activeCorner = -1; // 0=TL,1=TR,2=BR,3=BL
        static int activeEdge = -1;   // 0=L,1=T,2=R,3=B
        static ImVec2 dragStart;
        static int startW, startH;

        if (!show) {
            if (textureID != 0) {
                glDeleteTextures(1, &textureID);
                textureID = 0;
            }
            init = false;
            resizingCorner = false;
            resizingEdge = false;
            activeCorner = -1;
            activeEdge = -1;
            return;
        }

        ImGuiIO &io = ImGui::GetIO();

        if (!init) {
            ImGui::OpenPopup("Resize Overlay");
            originalImage = processor.getCurrentImage();
            newWidth = originalImage.width;
            newHeight = originalImage.height;
            textureID = loadTexture(originalImage);
            init = true;
        }

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(io.DisplaySize);
        ImGuiWindowFlags flags =
            ImGuiWindowFlags_NoDecoration |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoSavedSettings |
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoScrollbar |
            ImGuiWindowFlags_NoScrollWithMouse;

        if (!ImGui::BeginPopupModal("Resize Overlay", &show, flags))
            return;

        ImDrawList *draw = ImGui::GetWindowDrawList();
        draw->AddRectFilled(ImVec2(0, 0), io.DisplaySize, IM_COL32(0, 0, 0, 220));

        float displayedWidth = (float)originalImage.width;
        float displayedHeight = (float)originalImage.height;

        float scale = 1.0f;
        if (displayedWidth > io.DisplaySize.x) scale = io.DisplaySize.x / displayedWidth;
        if (displayedHeight * scale > io.DisplaySize.y) scale = io.DisplaySize.y / displayedHeight;
        displayedWidth *= scale;
        displayedHeight *= scale;

        ImVec2 imagePos(
            (io.DisplaySize.x - displayedWidth) * 0.5f,
            (io.DisplaySize.y - displayedHeight) * 0.5f
        );

        ImVec2 imageMax(imagePos.x + displayedWidth, imagePos.y + displayedHeight);
        // Don't draw full-size image; we'll draw the scaled preview version below

        // === Controls: movable/resizable top-left panel ===
        static bool keepAspect = true;
        float aspect = (float)originalImage.width / std::max(1, originalImage.height);

        static ImVec2 panelPos = ImVec2(0, 0);
        static ImVec2 panelSize = ImVec2(300, 200);

        ImVec2 winPos = ImGui::GetWindowPos();
        ImVec2 panelTL = winPos + panelPos;
        ImVec2 panelBR = ImVec2(panelTL.x + panelSize.x, panelTL.y + panelSize.y);

        // Panel background and border
        draw->AddRectFilled(panelTL, panelBR, IM_COL32(20, 20, 20, 230), 6.0f);
        draw->AddRect(panelTL, panelBR, IM_COL32(255, 255, 255, 64), 6.0f, 0, 1.5f);

        // Child contents below header
        const float headerH = 28.0f;
        ImVec2 childPos = panelPos + ImVec2(0, headerH);
        ImVec2 childSize = panelSize - ImVec2(0, headerH);
        ImGui::SetCursorPos(childPos);
        float dpi = std::max(1.0f, ImGui::GetIO().DisplayFramebufferScale.x);
        ImGui::PushStyleVar(ImGuiStyleVar_GrabMinSize,  std::max(12.0f, 14.0f * dpi));
        ImGuiWindowFlags childFlags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
        ImGui::BeginChild("Resize Controls", childSize, false, childFlags);
        ImGui::Dummy(ImVec2(0, 6));
        if (ImGui::InputInt("Width", &newWidth)) {
            newWidth = std::max(1, newWidth);
            if (keepAspect) newHeight = std::max(1, (int)(newWidth / aspect));
        }
        if (ImGui::InputInt("Height", &newHeight)) {
            newHeight = std::max(1, newHeight);
            if (keepAspect) newWidth = std::max(1, (int)(newHeight * aspect));
        }
        ImGui::Checkbox("Keep Aspect Ratio", &keepAspect);
        ImGui::Text("Scale: %.2fx", (float)newWidth / std::max(1, originalImage.width));
        if (ImGui::Button("Apply")) {
            processor.setImage(originalImage);
            ResizeFilter f(newWidth, newHeight);
            processor.applyFilter(f);
            textureNeedsUpdate = true;
            if (textureID != 0) { glDeleteTextures(1, &textureID); textureID = 0; }
            show = false;
            init = false;
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
            processor.setImage(originalImage);
            textureNeedsUpdate = true;
            if (textureID != 0) { glDeleteTextures(1, &textureID); textureID = 0; }
            show = false;
            init = false;
        }
        ImGui::EndChild();
        ImGui::PopStyleVar(1);

        // === Draw frame ===
        float previewScaleX = (float)newWidth / originalImage.width;
        float previewScaleY = (float)newHeight / originalImage.height;
        float previewW = displayedWidth * previewScaleX;
        float previewH = displayedHeight * previewScaleY;

        ImVec2 previewPos((io.DisplaySize.x - previewW) * 0.5f, (io.DisplaySize.y - previewH) * 0.5f);
        ImVec2 previewMax(previewPos.x + previewW, previewPos.y + previewH);
        // Real-time preview: draw the image scaled to the preview rectangle
        draw->AddImage((void *)(intptr_t)textureID, previewPos, previewMax);
        draw->AddRect(previewPos, previewMax, IM_COL32(255, 255, 255, 200), 0, 0, 2.0f);

        // === Handles: corners and edges ===
        ImVec2 corners[4] = {
            previewPos,
            ImVec2(previewMax.x, previewPos.y),
            previewMax,
            ImVec2(previewPos.x, previewMax.y)
        };

        const float handleR = 8.0f;
        for (auto &c : corners) {
            bool cInPanel = (c.x >= panelTL.x && c.x <= panelBR.x && c.y >= panelTL.y && c.y <= panelBR.y);
            if (cInPanel) continue;
            draw->AddCircleFilled(c, handleR, IM_COL32(255, 255, 255, 255));
            draw->AddCircle(c, handleR, IM_COL32(0, 0, 0, 255), 0, 2.0f);
        }

        // Edge handles (midpoints)
        ImVec2 edgeCenters[4] = {
            ImVec2((previewPos.x + previewPos.x) * 0.5f, (previewPos.y + previewMax.y) * 0.5f), // L
            ImVec2((previewPos.x + previewMax.x) * 0.5f, (previewPos.y + previewPos.y) * 0.5f), // T
            ImVec2((previewMax.x + previewMax.x) * 0.5f, (previewPos.y + previewMax.y) * 0.5f), // R
            ImVec2((previewPos.x + previewMax.x) * 0.5f, (previewMax.y + previewMax.y) * 0.5f)  // B
        };
        const ImVec2 edgeSize(18, 18);
        for (int e = 0; e < 4; ++e) {
            ImVec2 tl(edgeCenters[e].x - edgeSize.x * 0.5f, edgeCenters[e].y - edgeSize.y * 0.5f);
            ImVec2 br(edgeCenters[e].x + edgeSize.x * 0.5f, edgeCenters[e].y + edgeSize.y * 0.5f);
            bool intersectsPanel = !(br.x <= panelTL.x || tl.x >= panelBR.x || br.y <= panelTL.y || tl.y >= panelBR.y);
            if (!intersectsPanel) {
                draw->AddRectFilled(tl, br, IM_COL32(255, 255, 255, 220), 3.0f);
                draw->AddRect(tl, br, IM_COL32(0, 0, 0, 255), 3.0f, 0, 2.0f);
            }
            if (!intersectsPanel) {
                ImGui::SetCursorScreenPos(tl);
                ImGui::InvisibleButton((std::string("##resize_edge_") + char('0' + e)).c_str(), ImVec2(edgeSize.x, edgeSize.y));
                if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0)) {
                    resizingEdge = true; activeEdge = e; dragStart = io.MousePos; startW = newWidth; startH = newHeight;
                }
            }
        }

        // Corner hover/click
        ImVec2 mouse = io.MousePos;
        int hoveredCorner = -1;
        bool overPanelArea = mouse.x >= panelTL.x && mouse.x <= panelBR.x && mouse.y >= panelTL.y && mouse.y <= panelBR.y;
        if (!resizingEdge && !resizingCorner) {
            for (int i = 0; i < 4; ++i) {
                float dx = mouse.x - corners[i].x;
                float dy = mouse.y - corners[i].y;
                bool cornerInPanel = (corners[i].x >= panelTL.x && corners[i].x <= panelBR.x && corners[i].y >= panelTL.y && corners[i].y <= panelBR.y);
                if (!cornerInPanel && dx * dx + dy * dy <= handleR * handleR * 2.0f) { hoveredCorner = i; break; }
            }
        }
        if (hoveredCorner != -1 && !overPanelArea && ImGui::IsMouseClicked(0)) {
            resizingCorner = true; activeCorner = hoveredCorner; dragStart = mouse; startW = newWidth; startH = newHeight;
        }

        // Dragging corner
        if (resizingCorner && ImGui::IsMouseDown(0)) {
            ImVec2 delta(mouse.x - dragStart.x, mouse.y - dragStart.y);
            const float previewScaleX = displayedWidth / (float)originalImage.width;
            const float previewScaleY = displayedHeight / (float)originalImage.height;
            int dx = (int)(delta.x / previewScaleX);
            int dy = (int)(delta.y / previewScaleY);
            int tmpW = startW, tmpH = startH;
            switch (activeCorner) {
                case 0: tmpW = startW - dx; tmpH = startH - dy; break;
                case 1: tmpW = startW + dx; tmpH = startH - dy; break;
                case 2: tmpW = startW + dx; tmpH = startH + dy; break;
                case 3: tmpW = startW - dx; tmpH = startH + dy; break;
            }
            if (keepAspect) {
                if (fabs(delta.x) > fabs(delta.y)) { newWidth = std::max(1, tmpW); newHeight = std::max(1, (int)(newWidth / aspect)); }
                else { newHeight = std::max(1, tmpH); newWidth = std::max(1, (int)(newHeight * aspect)); }
            } else {
                newWidth = std::max(1, tmpW);
                newHeight = std::max(1, tmpH);
            }
        }

        // Dragging edge
        if (resizingEdge && ImGui::IsMouseDown(0)) {
            ImVec2 delta(mouse.x - dragStart.x, mouse.y - dragStart.y);
            const float previewScaleX = displayedWidth / (float)originalImage.width;
            const float previewScaleY = displayedHeight / (float)originalImage.height;
            int dx = (int)(delta.x / previewScaleX);
            int dy = (int)(delta.y / previewScaleY);
            int tmpW = startW, tmpH = startH;
            switch (activeEdge) {
                case 0: tmpW = startW - dx; break; // left
                case 1: tmpH = startH - dy; break; // top
                case 2: tmpW = startW + dx; break; // right
                case 3: tmpH = startH + dy; break; // bottom
            }
            if (keepAspect) {
                if (activeEdge == 0 || activeEdge == 2) { // width change
                    newWidth = std::max(1, tmpW); newHeight = std::max(1, (int)(newWidth / aspect));
                } else { // height change
                    newHeight = std::max(1, tmpH); newWidth = std::max(1, (int)(newHeight * aspect));
                }
            } else {
                newWidth = std::max(1, tmpW);
                newHeight = std::max(1, tmpH);
            }
        }

        // Release
        if (!ImGui::IsMouseDown(0)) {
            resizingCorner = false;
            resizingEdge = false;
            activeCorner = -1;
            activeEdge = -1;
        }

        // Draw header last so it stays on top and handles move
        ImVec2 headerBR2 = ImVec2(panelBR.x, panelTL.y + headerH);
        draw->AddRectFilled(panelTL, headerBR2, IM_COL32(45, 45, 45, 255), 6.0f);
        draw->AddText(ImVec2(panelTL.x + 8, panelTL.y + 6), IM_COL32(255,255,255,255), "Resize Controls");
        ImGui::SetCursorScreenPos(panelTL);
        ImGui::InvisibleButton("##resize_panel_move", ImVec2(panelSize.x, headerH));
        static bool panelDragging = false;
        static ImVec2 panelDragStart;
        if (ImGui::IsItemActive() && ImGui::IsMouseDragging(0)) {
            if (!panelDragging) { panelDragging = true; panelDragStart = io.MousePos; }
            ImVec2 delta = io.MousePos - panelDragStart;
            panelPos.x = std::clamp(panelPos.x + delta.x, 0.0f, io.DisplaySize.x - panelSize.x);
            panelPos.y = std::clamp(panelPos.y + delta.y, 0.0f, io.DisplaySize.y - panelSize.y);
            panelDragStart = io.MousePos;
        }
        if (!ImGui::IsMouseDown(0)) panelDragging = false;

        // Size grip bottom-right (last to capture input)
        ImVec2 gripTL2 = ImVec2(panelBR.x - 18, panelBR.y - 18);
        ImGui::SetCursorScreenPos(gripTL2);
        ImGui::InvisibleButton("##resize_panel_resize", ImVec2(18, 18));
        draw->AddTriangleFilled(ImVec2(panelBR.x - 14, panelBR.y - 2), ImVec2(panelBR.x - 2, panelBR.y - 2), ImVec2(panelBR.x - 2, panelBR.y - 14), IM_COL32(200, 200, 200, 200));
        static bool panelResizing = false;
        static ImVec2 panelResizeStart;
        if (ImGui::IsItemActive() && ImGui::IsMouseDragging(0)) {
            if (!panelResizing) { panelResizing = true; panelResizeStart = io.MousePos; }
            ImVec2 delta = io.MousePos - panelResizeStart;
            panelSize.x = std::clamp(panelSize.x + delta.x, 240.0f, io.DisplaySize.x - panelPos.x);
            panelSize.y = std::clamp(panelSize.y + delta.y, 140.0f, io.DisplaySize.y - panelPos.y);
            panelResizeStart = io.MousePos;
        }
        if (!ImGui::IsMouseDown(0)) panelResizing = false;

        ImGui::EndPopup();
    }





    void applyBrightness(bool &show, bool &textureNeedsUpdate) {
        static float factor = 1.0f;
        static Image originalImage;
        static bool init = false;
        if (show) {
            if (!BeginParamsUI("Brightness Parameters", &show)) return;

            if(!init){
                originalImage = processor.getCurrentImage();
                init = true;
            }


            ImGui::Text("Factor:");
            ImGui::SameLine();
            bool changed = false;
            if(ImGui::SliderFloat("##FactorSlider", &factor, 0.0f, 3.0f, "%.2f"))changed = true;

            
            ImGui::SameLine();
            if(ImGui::InputFloat("##FactorInput", &factor, 0.0f, 0.0f, "%.2f"))changed = true;

            if(changed){
                processor.setImage(originalImage);
                DarkenFilter filter(factor);
                processor.applyFilter(filter);
                textureNeedsUpdate = true;
                
            }
            ImGui::Separator();

            if (ImGui::Button("Apply")) {
                DarkenFilter filter(factor);
                processor.setImage(originalImage);
                processor.applyFilter(filter);
                std::cout << "Brightness filter applied with factor: " << factor << std::endl;
                show = false;
                init = false;
                textureNeedsUpdate = true;
            }

            ImGui::SameLine();
            if (ImGui::Button("Cancel")) {
                processor.setImage(originalImage);
                show = false;
                init = false;
                textureNeedsUpdate = true;
            }

            EndParamsUI();
        }else init = false;
    }

    void applyFrame(bool &show, bool &textureNeedsUpdate) {
        if(show){
            Image frame_image(openFileDialog_Linux());
            FrameFilter filter(frame_image);
            processor.applyFilter(filter);
            std::cout << "Applied Frame Filter\n";
            show = false;
            textureNeedsUpdate = true;
        }
    }
    void applyHorizontalFlip(bool &show, bool &textureNeedsUpdate) {
        if(show){
            HorizontalFlipFilter filter;
            processor.applyFilter(filter);
            std::cout << "Applied Horizontal Flip Filter\n";
            
            show = false;
            textureNeedsUpdate = true;
        }
    }
    void applyVerticalFlip(bool &show, bool &textureNeedsUpdate) {
        if(show){
            VerticalFlipFilter filter;
            processor.applyFilter(filter);
            std::cout << "Applied Vertical Flip Filter\n";
            
            show = false;
            textureNeedsUpdate = true;
        }
    }

    // Merge overlay: drag to position, resize via corners and edge handles
    void applyMerge(bool &show, bool &textureNeedsUpdate) {
        static Image baseImage;
        static Image overlayImage;
        static bool init = false;
        static bool overlayLoaded = false;
        static GLuint baseTexID = 0;
        static GLuint overlayTexID = 0;

        static int posX = 0, posY = 0;
        static int overW = 0, overH = 0;
        static float alpha = 0.5f;
        static bool keepAspect = true;

        static bool dragging = false;
        static bool resizingCorner = false;
        static bool resizingEdge = false;
        static int activeCorner = -1; // 0=TL,1=TR,2=BR,3=BL
        static int activeEdge = -1;   // 0=L,1=T,2=R,3=B
        static ImVec2 dragStart;
        static int startX, startY, startW, startH;

        if (!show) {
            if (baseTexID) { glDeleteTextures(1, &baseTexID); baseTexID = 0; }
            if (overlayTexID) { glDeleteTextures(1, &overlayTexID); overlayTexID = 0; }
            init = false; overlayLoaded = false;
            dragging = resizingCorner = resizingEdge = false; activeCorner = activeEdge = -1;
            return;
        }

        ImGuiIO &io = ImGui::GetIO();

        if (!init) {
            ImGui::OpenPopup("Merge Overlay");
            baseImage = processor.getCurrentImage();
            baseTexID = loadTexture(baseImage);
            overlayLoaded = false;
            posX = posY = 0;
            overW = std::max(1, baseImage.width / 3);
            overH = std::max(1, baseImage.height / 3);
            alpha = 0.5f; keepAspect = true;
            init = true;
        }

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(io.DisplaySize);
        ImGuiWindowFlags flags =
            ImGuiWindowFlags_NoDecoration |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoSavedSettings |
            ImGuiWindowFlags_NoCollapse;

        if (!ImGui::BeginPopupModal("Merge Overlay", &show, flags)) return;

        ImDrawList *draw = ImGui::GetWindowDrawList();
        draw->AddRectFilled(ImVec2(0, 0), io.DisplaySize, IM_COL32(0, 0, 0, 220));

        float displayedWidth = (float)baseImage.width;
        float displayedHeight = (float)baseImage.height;
        float scale = 1.0f;
        if (displayedWidth > io.DisplaySize.x) scale = io.DisplaySize.x / displayedWidth;
        if (displayedHeight * scale > io.DisplaySize.y) scale = io.DisplaySize.y / displayedHeight;
        displayedWidth *= scale;
        displayedHeight *= scale;

        ImVec2 imagePos(
            (io.DisplaySize.x - displayedWidth) * 0.5f,
            (io.DisplaySize.y - displayedHeight) * 0.5f
        );
        ImVec2 imageMax(imagePos.x + displayedWidth, imagePos.y + displayedHeight);

        draw->AddImage((void *)(intptr_t)baseTexID, imagePos, imageMax);

        float scaleX = displayedWidth / (float)baseImage.width;
        float scaleY = displayedHeight / (float)baseImage.height;

        if (!overlayLoaded) {
            // Movable/resizable control panel (top-left by default)
            static ImVec2 panelPos = ImVec2(0, 0);
            static ImVec2 panelSize = ImVec2(340, 150);

            ImVec2 winPos = ImGui::GetWindowPos();
            ImVec2 panelTL = winPos + panelPos;
            ImVec2 panelBR = ImVec2(panelTL.x + panelSize.x, panelTL.y + panelSize.y);

            const float headerH = 28.0f;
            // Panel background and border
            draw->AddRectFilled(panelTL, panelBR, IM_COL32(20, 20, 20, 230), 6.0f);
            draw->AddRect(panelTL, panelBR, IM_COL32(255, 255, 255, 64), 6.0f, 0, 1.5f);

            // Contents area below header
            ImGui::SetCursorPos(panelPos + ImVec2(0, headerH));
            ImGui::BeginChild("Merge Controls (loader)", panelSize - ImVec2(0, headerH), false,
                              ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
            ImGui::Dummy(ImVec2(0, 2));
            ImGui::TextDisabled("Choose image to merge");
            if (ImGui::Button("Choose Merge Image")) {
                std::string path = openFileDialog_Linux();
                if (!path.empty()) {
                    overlayImage = Image(path);
                    overlayTexID = loadTexture(overlayImage);
                    overW = std::min(overW, overlayImage.width);
                    overH = std::min(overH, overlayImage.height);
                    if (keepAspect) {
                        float asp = (float)overlayImage.width / std::max(1, overlayImage.height);
                        overH = std::max(1, (int)(overW / asp));
                    }
                    overlayLoaded = true;
                }
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel")) {
                processor.setImage(baseImage);
                textureNeedsUpdate = true;
                if (baseTexID) { glDeleteTextures(1, &baseTexID); baseTexID = 0; }
                if (overlayTexID) { glDeleteTextures(1, &overlayTexID); overlayTexID = 0; }
                ImGui::CloseCurrentPopup();
                show = false; init = false;
            }
            ImGui::EndChild();

            // Header for moving (placed after child to capture input)
            ImVec2 headerBR = ImVec2(panelBR.x, panelTL.y + headerH);
            draw->AddRectFilled(panelTL, headerBR, IM_COL32(45, 45, 45, 255), 6.0f);
            draw->AddText(ImVec2(panelTL.x + 8, panelTL.y + 6), IM_COL32(255,255,255,255), "Merge Controls");
            ImGui::SetCursorScreenPos(panelTL);
            ImGui::InvisibleButton("##merge_panel_move", ImVec2(panelSize.x, headerH));
            static bool panelDragging = false;
            static ImVec2 panelDragStart;
            if (ImGui::IsItemActive() && ImGui::IsMouseDragging(0)) {
                if (!panelDragging) { panelDragging = true; panelDragStart = io.MousePos; }
                ImVec2 delta = io.MousePos - panelDragStart;
                panelPos.x = std::clamp(panelPos.x + delta.x, 0.0f, io.DisplaySize.x - panelSize.x);
                panelPos.y = std::clamp(panelPos.y + delta.y, 0.0f, io.DisplaySize.y - panelSize.y);
                panelDragStart = io.MousePos;
            }
            if (!ImGui::IsMouseDown(0)) panelDragging = false;

            // Size grip bottom-right (after child for input priority)
            ImVec2 gripTL = ImVec2(panelBR.x - 18, panelBR.y - 18);
            ImGui::SetCursorScreenPos(gripTL);
            ImGui::InvisibleButton("##merge_panel_resize", ImVec2(18, 18));
            draw->AddTriangleFilled(ImVec2(panelBR.x - 14, panelBR.y - 2), ImVec2(panelBR.x - 2, panelBR.y - 2), ImVec2(panelBR.x - 2, panelBR.y - 14), IM_COL32(200, 200, 200, 200));
            static bool panelResizing = false;
            static ImVec2 panelResizeStart;
            if (ImGui::IsItemActive() && ImGui::IsMouseDragging(0)) {
                if (!panelResizing) { panelResizing = true; panelResizeStart = io.MousePos; }
                ImVec2 delta = io.MousePos - panelResizeStart;
                panelSize.x = std::clamp(panelSize.x + delta.x, 260.0f, io.DisplaySize.x - panelPos.x);
                panelSize.y = std::clamp(panelSize.y + delta.y, 120.0f, io.DisplaySize.y - panelPos.y);
                panelResizeStart = io.MousePos;
            }
            if (!ImGui::IsMouseDown(0)) panelResizing = false;
            ImGui::EndPopup();
            return;
        }

        // Movable/resizable control panel (top-left by default)
        static ImVec2 panelPos = ImVec2(0, 0);
        static ImVec2 panelSize = ImVec2(340, 240);

        ImVec2 winPos = ImGui::GetWindowPos();
        ImVec2 panelTL = winPos + panelPos;
        ImVec2 panelBR = ImVec2(panelTL.x + panelSize.x, panelTL.y + panelSize.y);

        const float headerH = 28.0f;
        // Panel background and border
        draw->AddRectFilled(panelTL, panelBR, IM_COL32(20, 20, 20, 230), 6.0f);
        draw->AddRect(panelTL, panelBR, IM_COL32(255, 255, 255, 64), 6.0f, 0, 1.5f);

        // Contents area below header
        ImGui::SetCursorPos(panelPos + ImVec2(0, headerH));
        ImGui::BeginChild("Merge Controls", panelSize - ImVec2(0, headerH), false,
                          ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
        ImGui::Dummy(ImVec2(0, 2));
        if (ImGui::Button("Change Merge Image")) {
            std::string path = openFileDialog_Linux();
            if (!path.empty()) {
                overlayImage = Image(path);
                if (overlayTexID) glDeleteTextures(1, &overlayTexID);
                overlayTexID = loadTexture(overlayImage);
                if (keepAspect) {
                    float asp = (float)overlayImage.width / std::max(1, overlayImage.height);
                    overH = std::max(1, (int)(overW / asp));
                }
            }
        }
        ImGui::Checkbox("Keep Aspect Ratio (corners)", &keepAspect);
        ImGui::SliderFloat("Opacity", &alpha, 0.0f, 1.0f, "%.2f");
        ImGui::Text("Pos: %d, %d  Size: %dx%d", posX, posY, overW, overH);

        if (ImGui::Button("Apply")) {
            processor.setImage(baseImage);
            MergeFilter f(overlayImage, posX, posY, overW, overH, alpha);
            processor.applyFilter(f);
            textureNeedsUpdate = true;
            if (baseTexID) { glDeleteTextures(1, &baseTexID); baseTexID = 0; }
            if (overlayTexID) { glDeleteTextures(1, &overlayTexID); overlayTexID = 0; }
            ImGui::CloseCurrentPopup();
            show = false; init = false; overlayLoaded = false;
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
            processor.setImage(baseImage);
            textureNeedsUpdate = true;
            if (baseTexID) { glDeleteTextures(1, &baseTexID); baseTexID = 0; }
            if (overlayTexID) { glDeleteTextures(1, &overlayTexID); overlayTexID = 0; }
            ImGui::CloseCurrentPopup();
            show = false; init = false; overlayLoaded = false;
        }
        ImGui::EndChild();

        // Header for moving
        ImVec2 headerBR = ImVec2(panelBR.x, panelTL.y + headerH);
        draw->AddRectFilled(panelTL, headerBR, IM_COL32(45, 45, 45, 255), 6.0f);
        draw->AddText(ImVec2(panelTL.x + 8, panelTL.y + 6), IM_COL32(255,255,255,255), "Merge Controls");
        ImGui::SetCursorScreenPos(panelTL);
        ImGui::InvisibleButton("##merge_panel_move2", ImVec2(panelSize.x, headerH));
        static bool panelDragging = false;
        static ImVec2 panelDragStart;
        if (ImGui::IsItemActive() && ImGui::IsMouseDragging(0)) {
            if (!panelDragging) { panelDragging = true; panelDragStart = io.MousePos; }
            ImVec2 delta = io.MousePos - panelDragStart;
            panelPos.x = std::clamp(panelPos.x + delta.x, 0.0f, io.DisplaySize.x - panelSize.x);
            panelPos.y = std::clamp(panelPos.y + delta.y, 0.0f, io.DisplaySize.y - panelSize.y);
            panelDragStart = io.MousePos;
        }
        if (!ImGui::IsMouseDown(0)) panelDragging = false;

        // Size grip bottom-right
        ImVec2 gripTL = ImVec2(panelBR.x - 18, panelBR.y - 18);
        ImGui::SetCursorScreenPos(gripTL);
        ImGui::InvisibleButton("##merge_panel_resize2", ImVec2(18, 18));
        draw->AddTriangleFilled(ImVec2(panelBR.x - 14, panelBR.y - 2), ImVec2(panelBR.x - 2, panelBR.y - 2), ImVec2(panelBR.x - 2, panelBR.y - 14), IM_COL32(200, 200, 200, 200));
        static bool panelResizing = false;
        static ImVec2 panelResizeStart;
        if (ImGui::IsItemActive() && ImGui::IsMouseDragging(0)) {
            if (!panelResizing) { panelResizing = true; panelResizeStart = io.MousePos; }
            ImVec2 delta = io.MousePos - panelResizeStart;
            panelSize.x = std::clamp(panelSize.x + delta.x, 260.0f, io.DisplaySize.x - panelPos.x);
            panelSize.y = std::clamp(panelSize.y + delta.y, 120.0f, io.DisplaySize.y - panelPos.y);
            panelResizeStart = io.MousePos;
            panelBR = ImVec2(panelTL.x + panelSize.x, panelTL.y + panelSize.y);
        }
        if (!ImGui::IsMouseDown(0)) panelResizing = false;

        // Overlay rect in screen space
        ImVec2 overMin(imagePos.x + posX * scaleX, imagePos.y + posY * scaleY);
        ImVec2 overMax(overMin.x + overW * scaleX, overMin.y + overH * scaleY);

        // Draw overlay image and frame
        draw->AddImage((void *)(intptr_t)overlayTexID, overMin, overMax, ImVec2(0, 0), ImVec2(1, 1), IM_COL32(255, 255, 255, (int)(alpha * 255)));
        draw->AddRect(overMin, overMax, IM_COL32(255, 255, 255, 255), 0, 0, 2.0f);

        // Corner handles
        ImVec2 corners[4] = { overMin, ImVec2(overMax.x, overMin.y), overMax, ImVec2(overMin.x, overMax.y) };
        const float handleR = 8.0f;
        for (auto &c : corners) {
            draw->AddCircleFilled(c, handleR, IM_COL32(255, 255, 255, 255));
            draw->AddCircle(c, handleR, IM_COL32(0, 0, 0, 255), 0, 2.0f);
        }

        // Edge handles as draggable invisible buttons (L,T,R,B)
        ImVec2 edgeCenters[4] = {
            ImVec2((overMin.x + overMin.x) * 0.5f, (overMin.y + overMax.y) * 0.5f), // L
            ImVec2((overMin.x + overMax.x) * 0.5f, (overMin.y + overMin.y) * 0.5f), // T
            ImVec2((overMax.x + overMax.x) * 0.5f, (overMin.y + overMax.y) * 0.5f), // R
            ImVec2((overMin.x + overMax.x) * 0.5f, (overMax.y + overMax.y) * 0.5f)  // B
        };
        const ImVec2 edgeSize(18, 18);
        bool edgeHoveredAny = false;
        for (int e = 0; e < 4; ++e) {
            ImVec2 tl(edgeCenters[e].x - edgeSize.x * 0.5f, edgeCenters[e].y - edgeSize.y * 0.5f);
            ImVec2 br(edgeCenters[e].x + edgeSize.x * 0.5f, edgeCenters[e].y + edgeSize.y * 0.5f);
            draw->AddRectFilled(tl, br, IM_COL32(255, 255, 255, 220), 3.0f);
            draw->AddRect(tl, br, IM_COL32(0, 0, 0, 255), 3.0f, 0, 2.0f);
            ImGui::SetCursorScreenPos(tl);
            ImGui::InvisibleButton((std::string("##edge_") + char('0' + e)).c_str(), ImVec2(edgeSize.x, edgeSize.y));
            if (ImGui::IsItemHovered()) edgeHoveredAny = true;
            if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0)) {
                resizingEdge = true; activeEdge = e; dragStart = io.MousePos; startX = posX; startY = posY; startW = overW; startH = overH;
            }
            if (ImGui::IsItemActive() && ImGui::IsMouseDragging(0)) {
                if (!resizingEdge) {
                    resizingEdge = true; activeEdge = e; dragStart = io.MousePos; startX = posX; startY = posY; startW = overW; startH = overH;
                }
            }
        }

        // Hover corner detection if not resizing edge
        ImVec2 mouse = io.MousePos;
        int hoveredCorner = -1;
        bool overPanelArea = mouse.x >= panelTL.x && mouse.x <= panelBR.x && mouse.y >= panelTL.y && mouse.y <= panelBR.y;
        if (!dragging && !resizingCorner && !resizingEdge && !overPanelArea) {
            for (int i = 0; i < 4; ++i) {
                float dx = mouse.x - corners[i].x;
                float dy = mouse.y - corners[i].y;
                if (dx * dx + dy * dy <= handleR * handleR * 2.0f) { hoveredCorner = i; break; }
            }
        }
        
        if (hoveredCorner != -1 && !overPanelArea && ImGui::IsMouseClicked(0)) {
            resizingCorner = true; activeCorner = hoveredCorner; dragStart = mouse;
            startX = posX; startY = posY; startW = overW; startH = overH;
        }

        // Drag inside overlay to move (but not when interacting with handles or control panel)
        bool insideOverlay = mouse.x >= overMin.x && mouse.x <= overMax.x && mouse.y >= overMin.y && mouse.y <= overMax.y;
        if (insideOverlay && !resizingCorner && !resizingEdge && !edgeHoveredAny && hoveredCorner == -1 && !overPanelArea) {
            if (ImGui::IsMouseClicked(0)) { dragging = true; dragStart = mouse; startX = posX; startY = posY; }
        }

        // Handle corner resizing
        if (resizingCorner && ImGui::IsMouseDown(0)) {
            ImVec2 delta(mouse.x - dragStart.x, mouse.y - dragStart.y);
            int dx = (int)(delta.x / scaleX);
            int dy = (int)(delta.y / scaleY);

            int tmpW = startW, tmpH = startH, tmpX = startX, tmpY = startY;
            switch (activeCorner) {
                case 0: tmpX = std::clamp(startX + dx, 0, startX + startW - 1); tmpY = std::clamp(startY + dy, 0, startY + startH - 1); tmpW = (startX + startW) - tmpX; tmpH = (startY + startH) - tmpY; break;
                case 1: tmpY = std::clamp(startY + dy, 0, startY + startH - 1); tmpW = std::clamp(startW + dx, 1, baseImage.width - startX); tmpH = (startY + startH) - tmpY; break;
                case 2: tmpW = std::clamp(startW + dx, 1, baseImage.width - startX); tmpH = std::clamp(startH + dy, 1, baseImage.height - startY); break;
                case 3: tmpX = std::clamp(startX + dx, 0, startX + startW - 1); tmpW = (startX + startW) - tmpX; tmpH = std::clamp(startH + dy, 1, baseImage.height - startY); break;
            }
            if (keepAspect) {
                float asp = (float)overlayImage.width / std::max(1, overlayImage.height);
                if (fabsf((float)dx) > fabsf((float)dy)) { tmpW = std::max(1, tmpW); tmpH = std::max(1, (int)(tmpW / asp)); }
                else { tmpH = std::max(1, tmpH); tmpW = std::max(1, (int)(tmpH * asp)); }
            }
            posX = std::clamp(tmpX, 0, std::max(0, baseImage.width - tmpW));
            posY = std::clamp(tmpY, 0, std::max(0, baseImage.height - tmpH));
            overW = std::max(1, std::min(tmpW, baseImage.width - posX));
            overH = std::max(1, std::min(tmpH, baseImage.height - posY));
        }

        // Handle edge resizing (ignores aspect keeping for precise axis control)
        if (resizingEdge && ImGui::IsMouseDown(0)) {
            ImVec2 delta(mouse.x - dragStart.x, mouse.y - dragStart.y);
            int dx = (int)(delta.x / scaleX);
            int dy = (int)(delta.y / scaleY);
            int tmpX = startX, tmpY = startY, tmpW = startW, tmpH = startH;
            switch (activeEdge) {
                case 0: // left
                    tmpX = std::clamp(startX + dx, 0, startX + startW - 1);
                    tmpW = (startX + startW) - tmpX;
                    break;
                case 1: // top
                    tmpY = std::clamp(startY + dy, 0, startY + startH - 1);
                    tmpH = (startY + startH) - tmpY;
                    break;
                case 2: // right
                    tmpW = std::clamp(startW + dx, 1, baseImage.width - startX);
                    break;
                case 3: // bottom
                    tmpH = std::clamp(startH + dy, 1, baseImage.height - startY);
                    break;
            }
            posX = std::clamp(tmpX, 0, std::max(0, baseImage.width - tmpW));
            posY = std::clamp(tmpY, 0, std::max(0, baseImage.height - tmpH));
            overW = std::max(1, std::min(tmpW, baseImage.width - posX));
            overH = std::max(1, std::min(tmpH, baseImage.height - posY));
        }

        // Dragging move
        if (dragging && ImGui::IsMouseDown(0)) {
            ImVec2 delta(mouse.x - dragStart.x, mouse.y - dragStart.y);
            int dx = (int)(delta.x / scaleX);
            int dy = (int)(delta.y / scaleY);
            posX = std::clamp(startX + dx, 0, std::max(0, baseImage.width - overW));
            posY = std::clamp(startY + dy, 0, std::max(0, baseImage.height - overH));
        }

        // Release
        if (!ImGui::IsMouseDown(0)) {
            dragging = false; resizingCorner = false; resizingEdge = false; activeCorner = activeEdge = -1;
        }

        ImGui::EndPopup();
    }

    void applyRotate(bool &show, bool &textureNeedsUpdate) {
        static int currentItem = 0;
        const char* items[] = {"90", "180", "270"};
        int values[] = {90, 180, 270};

        static Image originalImage;
        static bool init = false;
        if(show){
            if (!BeginParamsUI("Rotate Parameters", &show)) return;

            if(!init){
                originalImage = processor.getCurrentImage();
                init = true;
            }

            ImGui::Text("Angle:");
            ImGui::SameLine();
            bool changed = false;
            if(ImGui::Combo("Angle", &currentItem, items, IM_ARRAYSIZE(items))) changed = true; // fixed

            if(changed){
                processor.setImage(originalImage);
                RotateFilter filter(values[currentItem]); 
                processor.applyFilter(filter);
                textureNeedsUpdate = true;
            }
            ImGui::Separator();

            if(ImGui::Button("Apply")){
                RotateFilter filter(values[currentItem]); 
                processor.setImage(originalImage);
                processor.applyFilter(filter);
                std::cout << "Rotated by: " << values[currentItem] << std::endl;
                show = false;
                init = false;
                textureNeedsUpdate = true;
            }

            ImGui::SameLine();
            if(ImGui::Button("Cancel")){
                processor.setImage(originalImage);
                show = false;
                init = false;
                textureNeedsUpdate = true;
            }

            EndParamsUI();
        }else init = false;
    }

    void applyOutline(bool &show, bool &textureNeedsUpdate) {
        if(show){
            OutlineFilter filter;
            processor.applyFilter(filter);
            std::cout << "Applied Outline Filter\n";
            
            show = false;
            textureNeedsUpdate = true;
        }
    }
    void applyPurple(bool &show, bool &textureNeedsUpdate) {
        static float factor = 0.0f;
        static Image originalImage;
        static bool init = false;
        if (show) {
            if (!BeginParamsUI("Purple Parameters", &show)) return;

            if(!init){
                originalImage = processor.getCurrentImage();
                init = true;
            }


            ImGui::Text("Factor:");
            ImGui::SameLine();
            bool changed = false;
            if(ImGui::SliderFloat("##FactorSlider", &factor, 0.0f, 3.0f, "%.2f"))changed = true;

            
            ImGui::SameLine();
            if(ImGui::InputFloat("##FactorInput", &factor, 0.0f, 0.0f, "%.2f"))changed = true;

            if(changed){
                processor.setImage(originalImage);
                PurpleFilter filter(factor);
                processor.applyFilter(filter);
                textureNeedsUpdate = true;
                
            }
            ImGui::Separator();

            if (ImGui::Button("Apply")) {
                PurpleFilter filter(factor);
                processor.setImage(originalImage);
                processor.applyFilter(filter);
                std::cout << "Purple filter applied with factor: " << factor << std::endl;
                show = false;
                init = false;
                textureNeedsUpdate = true;
            }

            ImGui::SameLine();
            if (ImGui::Button("Cancel")) {
                processor.setImage(originalImage);
                show = false;
                init = false;
                textureNeedsUpdate = true;
            }

            EndParamsUI();
        }else init = false;
    }

    void applyInfrared(bool &show, bool &textureNeedsUpdate) {
        if(show){
            InfraredFilter filter;
            processor.applyFilter(filter);
            std::cout << "Applied Infrared Filter\n";
            
            show = false;
            textureNeedsUpdate = true;
        }
    }

    void applyWave(bool &show, bool &textureNeedsUpdate) {
        static float amplitude, wavelength;
        static Image originalImage;
        static bool init = false;
        if(show){
            if (!BeginParamsUI("Wave Parameters", &show)) return;

            if(!init){
                originalImage = processor.getCurrentImage();
                init = true;
            }

            bool changed = false;
            
            ImGui::Text("Amplitude:");
            ImGui::SameLine();
            if(ImGui::InputFloat("##Amplitude", &amplitude, 0, 0))changed = true;
            
            ImGui::Text("Wavelength:");
            ImGui::SameLine();
            if(ImGui::InputFloat("##Wavelength", &wavelength, 0, 0))changed = true;

            if(changed){
                processor.setImage(originalImage);
                WaveFilter filter(amplitude, wavelength);
                processor.applyFilter(filter);
                textureNeedsUpdate = true;
            }
            
            ImGui::Separator();
            
            if(ImGui::Button("Apply")){
                WaveFilter filter(amplitude, wavelength);
                processor.setImage(originalImage);
                processor.applyFilter(filter);
                std::cout << "Wave filter applied with Parameters: " << amplitude << " " << wavelength << std::endl;
                show = false;
                init = false;
                textureNeedsUpdate = true;
            }

            ImGui::SameLine();
            if(ImGui::Button("Cancel")){
                processor.setImage(originalImage);
                show = false;
                init = false;
                textureNeedsUpdate = true;
            }

            EndParamsUI();
        }else init = false;
    }

    
    void applyOilPainting(bool &show, bool &textureNeedsUpdate) {
        
        static int currentItem = 0;
        const char* items[] = {"Low", "Medium", "High"};
        int values[] = {10, 20, 40};

        static Image originalImage;
        static bool init = false;
        if(show){
            if (!BeginParamsUI("Oil Painting Parameters", &show)) return;

            if(!init){
                originalImage = processor.getCurrentImage();
                init = true;
            }

            ImGui::Text("Intenisty:");
            ImGui::SameLine();
            bool changed = false;
            if(ImGui::Combo("##Intenisty", &currentItem, items, IM_ARRAYSIZE(items))) changed = true;

            if(changed){
                processor.setImage(originalImage);
                OilPaintingFilter filter(5, values[currentItem]); 
                processor.applyFilter(filter);
                textureNeedsUpdate = true;
            }
            ImGui::Separator();

            if(ImGui::Button("Apply")){
                OilPaintingFilter filter(5, values[currentItem]); 
                processor.setImage(originalImage);
                processor.applyFilter(filter);
                show = false;
                init = false;
                textureNeedsUpdate = true;
            }

            ImGui::SameLine();
            if(ImGui::Button("Cancel")){
                processor.setImage(originalImage);
                show = false;
                init = false;
                textureNeedsUpdate = true;
            }

            EndParamsUI();
        }else init = false;
    }

    void applyContrast(bool &show, bool &textureNeedsUpdate) {
        static float factor = 0.0f;
        static Image originalImage;
        static bool init = false;
        if (show) {
            if (!BeginParamsUI("Contrast Parameters", &show)) return;

            if(!init){
                originalImage = processor.getCurrentImage();
                init = true;
            }


            ImGui::Text("Factor:");
            ImGui::SameLine();
            bool changed = false;
            if(ImGui::SliderFloat("##FactorSlider", &factor, -3.0f, 3.0f, "%.2f"))changed = true;

            
            ImGui::SameLine();
            if(ImGui::InputFloat("##FactorInput", &factor, 0.0f, 0.0f, "%.2f"))changed = true;

            if(changed){
                processor.setImage(originalImage);
                ContrastFilter filter(factor);
                processor.applyFilter(filter);
                textureNeedsUpdate = true;
                
            }
            ImGui::Separator();

            if (ImGui::Button("Apply")) {
                ContrastFilter filter(factor);
                processor.setImage(originalImage);
                processor.applyFilter(filter);
                std::cout << "Contrast filter applied with factor: " << factor << std::endl;
                show = false;
                init = false;
                textureNeedsUpdate = true;
            }

            ImGui::SameLine();
            if (ImGui::Button("Cancel")) {
                processor.setImage(originalImage);
                show = false;
                init = false;
                textureNeedsUpdate = true;
            }

            EndParamsUI();
        }else init = false;
    }

    void applyRetro(bool &show, bool &textureNeedsUpdate) {
        if(show){
            RetroFilter filter;
            processor.applyFilter(filter);
            std::cout << "Applied Retro Filter\n";
            show = false;
            textureNeedsUpdate = true;
        }
    }

    void applySaturation(bool &show, bool &textureNeedsUpdate) {
        static float factor = 0.0f;
        static Image originalImage;
        static bool init = false;
        if (show) {
            if (!BeginParamsUI("Saturation Parameters", &show)) return;

            if(!init){
                originalImage = processor.getCurrentImage();
                init = true;
            }


            ImGui::Text("Factor:");
            ImGui::SameLine();
            bool changed = false;
            if(ImGui::SliderFloat("##FactorSlider", &factor, -3.0f, 3.0f, "%.2f"))changed = true;

            
            ImGui::SameLine();
            if(ImGui::InputFloat("##FactorInput", &factor, 0.0f, 0.0f, "%.2f"))changed = true;

            if(changed){
                processor.setImage(originalImage);
                SaturationFilter filter(factor);
                processor.applyFilter(filter);
                textureNeedsUpdate = true;
                
            }
            ImGui::Separator();

            if (ImGui::Button("Apply")) {
                SaturationFilter filter(factor);
                processor.setImage(originalImage);
                processor.applyFilter(filter);
                std::cout << "Saturation filter applied with factor: " << factor << std::endl;
                show = false;
                init = false;
                textureNeedsUpdate = true;
            }

            ImGui::SameLine();
            if (ImGui::Button("Cancel")) {
                processor.setImage(originalImage);
                show = false;
                init = false;
                textureNeedsUpdate = true;
            }

            EndParamsUI();
        }else init = false;
    }
    void applySkew(bool &show, bool &textureNeedsUpdate) {
        static float Angle = 0.0f;
        static Image originalImage;
        static bool init = false;
        if (show) {
            if (!BeginParamsUI("Skew Parameters", &show)) return;

            if(!init){
                originalImage = processor.getCurrentImage();
                init = true;
            }


            ImGui::Text("Angle:");
            ImGui::SameLine();
            bool changed = false;
            if(ImGui::SliderFloat("##AngleSlider", &Angle, -90.0f, 90.0f, "%.2f"))changed = true;

            
            ImGui::SameLine();
            if(ImGui::InputFloat("##AngleInput", &Angle, 0.0f, 0.0f, "%.2f"))changed = true;

            if(changed){
                processor.setImage(originalImage);
                SkewFilter filter(Angle);
                processor.applyFilter(filter);
                textureNeedsUpdate = true;
                
            }
            ImGui::Separator();

            if (ImGui::Button("Apply")) {
                SkewFilter filter(Angle);
                processor.setImage(originalImage);
                processor.applyFilter(filter);
                std::cout << "Skew filter applied with Angle: " << Angle << std::endl;
                show = false;
                init = false;
                textureNeedsUpdate = true;
            }

            ImGui::SameLine();
            if (ImGui::Button("Cancel")) {
                processor.setImage(originalImage);
                show = false;
                init = false;
                textureNeedsUpdate = true;
            }

            EndParamsUI();
        }else init = false;
    }
    void applyVignette(bool &show, bool &textureNeedsUpdate) {
        static float factor = 0.0f;
        static Image originalImage;
        static bool init = false;
        if (show) {
            if (!BeginParamsUI("Vignette Parameters", &show)) return;

            if(!init){
                originalImage = processor.getCurrentImage();
                init = true;
            }


            ImGui::Text("Factor:");
            ImGui::SameLine();
            bool changed = false;
            if(ImGui::SliderFloat("##FactorSlider", &factor, -3.0f, 3.0f, "%.2f"))changed = true;

            
            ImGui::SameLine();
            if(ImGui::InputFloat("##FactorInput", &factor, 0.0f, 0.0f, "%.2f"))changed = true;

            if(changed){
                processor.setImage(originalImage);
                VigentteFilter filter(factor);
                processor.applyFilter(filter);
                textureNeedsUpdate = true;
                
            }
            ImGui::Separator();

            if (ImGui::Button("Apply")) {
                VigentteFilter filter(factor);
                processor.setImage(originalImage);
                processor.applyFilter(filter);
                std::cout << "Vignette filter applied with factor: " << factor << std::endl;
                show = false;
                init = false;
                textureNeedsUpdate = true;
            }

            ImGui::SameLine();
            if (ImGui::Button("Cancel")) {
                processor.setImage(originalImage);
                show = false;
                init = false;
                textureNeedsUpdate = true;
            }

            EndParamsUI();
        }else init = false;
    }
    void applyWarmth(bool &show, bool &textureNeedsUpdate) {
        static float factor = 0.0f;
        static Image originalImage;
        static bool init = false;
        if (show) {
            if (!BeginParamsUI("Warmth Parameters", &show)) return;

            if(!init){
                originalImage = processor.getCurrentImage();
                init = true;
            }


            ImGui::Text("Factor:");
            ImGui::SameLine();
            bool changed = false;
            if(ImGui::SliderFloat("##FactorSlider", &factor, -3.0f, 3.0f, "%.2f"))changed = true;

            
            ImGui::SameLine();
            if(ImGui::InputFloat("##FactorInput", &factor, 0.0f, 0.0f, "%.2f"))changed = true;

            if(changed){
                processor.setImage(originalImage);
                WarmthFilter filter(factor);
                processor.applyFilter(filter);
                textureNeedsUpdate = true;
                
            }
            ImGui::Separator();

            if (ImGui::Button("Apply")) {
                WarmthFilter filter(factor);
                processor.setImage(originalImage);
                processor.applyFilter(filter);
                std::cout << "Warmth filter applied with factor: " << factor << std::endl;
                show = false;
                init = false;
                textureNeedsUpdate = true;
            }

            ImGui::SameLine();
            if (ImGui::Button("Cancel")) {
                processor.setImage(originalImage);
                show = false;
                init = false;
                textureNeedsUpdate = true;
            }

            EndParamsUI();
        }else init = false;
    }

private:
    ImageProcessor &processor;    
};