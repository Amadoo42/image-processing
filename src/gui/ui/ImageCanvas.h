#pragma once
#include "imgui.h"
#include "../GuiState.h"
#include "../CompareView.h"
#include "../LoadTexture.h"

static void drawImageCanvas(ImageProcessor &processor, float width) {
    ImGui::BeginChild("ImageCanvas", ImVec2(width, 0), true);
    const Image& currentImage = processor.getCurrentImage();
    gLastCanvasAvail = ImGui::GetContentRegionAvail();
    if(currentImage.width > 0 && currentImage.height > 0) {
        if(textureNeedsUpdate) {
            if(currentTextureID != 0) glDeleteTextures(1, &currentTextureID);
            currentTextureID = loadTexture(currentImage);
            textureNeedsUpdate = false;
        }
        if(!compareMode) {
            ImGuiIO& io = ImGui::GetIO();
            if(ImGui::IsWindowHovered() && io.MouseWheel != 0.0f) {
                zoom_level += io.MouseWheel * 0.1f;
                zoom_level = std::clamp(zoom_level, 0.1f, 10.0f);
            }
            if(ImGui::IsWindowHovered() && ImGui::IsMouseDragging(ImGuiMouseButton_Right)) {
                pan_offset.x += io.MouseDelta.x;
                pan_offset.y += io.MouseDelta.y;
            }
            ImVec2 zoomed_size = ImVec2(currentImage.width * zoom_level, currentImage.height * zoom_level);
            ImVec2 window_size = ImGui::GetContentRegionAvail();
            ImVec2 base_pos = ImVec2(
                (window_size.x - zoomed_size.x) * 0.5f,
                (window_size.y - zoomed_size.y) * 0.5f
            );
            float minX = -zoomed_size.x * 0.5f;
            float maxX = window_size.x - zoomed_size.x * 0.5f;
            float minY = -zoomed_size.y * 0.5f;
            float maxY = window_size.y - zoomed_size.y * 0.5f;
            pan_offset.x = std::clamp(pan_offset.x, minX - base_pos.x, maxX - base_pos.x);
            pan_offset.y = std::clamp(pan_offset.y, minY - base_pos.y, maxY - base_pos.y);
            ImVec2 image_pos = ImVec2(base_pos.x + pan_offset.x, base_pos.y + pan_offset.y);
            ImGui::SetCursorPos(image_pos);
            ImGui::Image((void*)(intptr_t)currentTextureID, zoomed_size);

            ImVec2 itemMin = ImGui::GetItemRectMin();
            ImVec2 itemMax = ImGui::GetItemRectMax();
            bool overImage = ImGui::IsItemHovered();
            auto screenToPixel = [&](ImVec2 screen)->ImVec2{
                float ix = (screen.x - itemMin.x) / std::max(0.0001f, zoom_level);
                float iy = (screen.y - itemMin.y) / std::max(0.0001f, zoom_level);
                ix = std::floor(ix); iy = std::floor(iy);
                ix = std::clamp(ix, 0.0f, (float)currentImage.width - 1.0f);
                iy = std::clamp(iy, 0.0f, (float)currentImage.height - 1.0f);
                return ImVec2(ix, iy);
            };

            if (gSelectionTool == SelectionToolMode::Rectangle && overImage) {
                if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
                    gHasActiveRectDrag = true;
                    gRectDragStart = screenToPixel(io.MousePos);
                    gRectDragEnd = gRectDragStart;
                }
                if (gHasActiveRectDrag && ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
                    gRectDragEnd = screenToPixel(io.MousePos);
                }
                if (gHasActiveRectDrag && ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
                    ImVec2 a = gRectDragStart; ImVec2 b = gRectDragEnd;
                    int rx = (int)std::floor(std::min(a.x, b.x));
                    int ry = (int)std::floor(std::min(a.y, b.y));
                    int rw = (int)std::abs(b.x - a.x) + 1;
                    int rh = (int)std::abs(b.y - a.y) + 1;
                    processor.setRectSelection(rx, ry, rw, rh);
                    gHasActiveRectDrag = false;
                    statusBarMessage = "Rectangle selected";
                }
            }

            if (gSelectionTool == SelectionToolMode::MagicWand && overImage) {
                if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
                    ImVec2 p = screenToPixel(io.MousePos);
                    processor.setMagicWandSelection((int)p.x, (int)p.y, std::max(0, gMagicWandTolerance));
                    statusBarMessage = "Magic wand selection updated";
                }
            }

            ImDrawList* dl = ImGui::GetWindowDrawList();
            if (gSelectionTool == SelectionToolMode::Rectangle && gHasActiveRectDrag) {
                ImVec2 a = gRectDragStart; ImVec2 b = gRectDragEnd;
                ImVec2 sa = ImVec2(itemMin.x + a.x * zoom_level, itemMin.y + a.y * zoom_level);
                ImVec2 sb = ImVec2(itemMin.x + b.x * zoom_level, itemMin.y + b.y * zoom_level);
                ImVec2 rmin(std::min(sa.x, sb.x), std::min(sa.y, sb.y));
                ImVec2 rmax(std::max(sa.x, sb.x), std::max(sa.y, sb.y));
                dl->AddRect(rmin, rmax, IM_COL32(0, 255, 255, 255), 0, 0, 2.0f);
            }

            if (processor.hasSelection()) {
                const auto &mask = processor.getSelectionMask();
                int w = currentImage.width, h = currentImage.height;
                
                if (gSelectionTool == SelectionToolMode::MagicWand) {
                    std::vector<ImVec2> outlinePoints;
                    for (int y = 0; y < h; ++y) {
                        for (int x = 0; x < w; ++x) {
                            if (mask[y][x]) {
                                bool isEdge = false;
                                for (int dy = -1; dy <= 1; ++dy) {
                                    for (int dx = -1; dx <= 1; ++dx) {
                                        int nx = x + dx;
                                        int ny = y + dy;
                                        if (nx >= 0 && nx < w && ny >= 0 && ny < h) {
                                            if (!mask[ny][nx]) { isEdge = true; break; }
                                        } else { isEdge = true; break; }
                                    }
                                    if (isEdge) break;
                                }
                                if (isEdge) {
                                    ImVec2 screenPos(itemMin.x + x * zoom_level, itemMin.y + y * zoom_level);
                                    outlinePoints.push_back(screenPos);
                                }
                            }
                        }
                    }
                    for (size_t i = 0; i < outlinePoints.size(); ++i) {
                        dl->AddCircleFilled(outlinePoints[i], 1.0f, IM_COL32(255, 255, 0, 255));
                    }
                } else {
                    int minx = w, miny = h, maxx = -1, maxy = -1;
                    for (int y = 0; y < h; ++y) {
                        for (int x = 0; x < w; ++x) {
                            if (mask[y][x]) {
                                if (x < minx) minx = x;
                                if (y < miny) miny = y;
                                if (x > maxx) maxx = x;
                                if (y > maxy) maxy = y;
                            }
                        }
                    }
                    if (maxx >= minx && maxy >= miny) {
                        ImVec2 smin(itemMin.x + minx * zoom_level, itemMin.y + miny * zoom_level);
                        ImVec2 smax(itemMin.x + (maxx + 1) * zoom_level, itemMin.y + (maxy + 1) * zoom_level);
                        dl->AddRect(smin, smax, IM_COL32(255, 255, 0, 255), 0, 0, 2.0f);
                    }
                }
            }
        } else {
            renderCompareView(processor, zoom_level, pan_offset);
        }
    }
    ImGui::EndChild();
}
