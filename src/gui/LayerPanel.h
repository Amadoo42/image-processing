#pragma once

#include "imgui.h"
#include <vector>
#include <string>
#include <algorithm>
#include <cstdio>
#include "../core/ImageProcessor.h"

class LayerPanel {
public:
    // Returns true if any change affecting the composite occurred
    bool render(ImageProcessor &processor) {
        bool changed = false;
        auto &layers = processor.getLayersMutable();
        int active = processor.getActiveLayerIndex();

        // Header actions
        if (ImGui::Button("Add New")) {
            const Image &base = processor.getCompositeImage();
            Image blank;
            if (base.width > 0 && base.height > 0) {
                blank = Image(base.width, base.height);
                // leave black; start hidden so it doesn't occlude
            }
            Layer nl(blank, defaultLayerName((int)layers.size()));
            nl.isVisible = false;
            layers.push_back(nl);
            processor.setActiveLayer((int)layers.size() - 1);
            processor.invalidateComposite();
            changed = true;
        }
        ImGui::SameLine();
        bool canAct = active >= 0 && active < (int)layers.size();
        if (!canAct) ImGui::BeginDisabled();
        if (ImGui::Button("Duplicate")) {
            Layer dup = layers[active];
            dup.name += " Copy";
            layers.insert(layers.begin() + active + 1, dup);
            processor.setActiveLayer(active + 1);
            processor.invalidateComposite();
            changed = true;
        }
        ImGui::SameLine();
        if (ImGui::Button("Delete")) {
            processor.deleteLayer(active);
            processor.invalidateComposite();
            changed = true;
        }
        if (!canAct) ImGui::EndDisabled();

        ImGui::Separator();
        ImGui::TextUnformatted("Layers");
        ImGui::BeginChild("##layer_list", ImVec2(0, 260), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);

        // Display top-most first
        for (int i = (int)layers.size() - 1; i >= 0; --i) {
            Layer &L = layers[i];
            ImGui::PushID(i);
            // Selection
            bool selected = (i == active);
            if (ImGui::Selectable(L.name.c_str(), selected)) {
                processor.setActiveLayer(i);
                changed = true; // affects which layer receives edits
            }
            // Drag & Drop reordering
            if (ImGui::BeginDragDropSource()) {
                int idx = i; ImGui::SetDragDropPayload("LAYER_IDX", &idx, sizeof(int));
                ImGui::Text("Move %s", L.name.c_str());
                ImGui::EndDragDropSource();
            }
            if (ImGui::BeginDragDropTarget()) {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("LAYER_IDX")) {
                    int from = *(const int*)payload->Data;
                    int to = i;
                    processor.moveLayer(from, to);
                    processor.invalidateComposite();
                    changed = true;
                }
                ImGui::EndDragDropTarget();
            }

            // Inline controls
            ImGui::Indent(12.0f);
            ImGui::Checkbox("Visible", &L.isVisible);
            ImGui::SameLine();
            ImGui::SetNextItemWidth(120.0f);
            if (ImGui::SliderFloat("Opacity", &L.opacity, 0.0f, 1.0f, "%.2f")) {
                L.opacity = std::clamp(L.opacity, 0.0f, 1.0f);
                processor.invalidateComposite();
                changed = true;
            }
            // Rename
            static char nameBuf[128];
            nameBuf[0] = '\0';
            // show input on active for brevity
            if (i == active) {
                std::snprintf(nameBuf, sizeof(nameBuf), "%s", L.name.c_str());
                ImGui::SetNextItemWidth(180.0f);
                if (ImGui::InputText("Name", nameBuf, IM_ARRAYSIZE(nameBuf))) {
                    L.name = nameBuf;
                }
            }
            // Up/Down
            if (ImGui::ArrowButton("Up", ImGuiDir_Up)) { processor.moveLayer(i, std::min(i + 1, (int)layers.size()-1)); processor.invalidateComposite(); changed = true; }
            ImGui::SameLine();
            if (ImGui::ArrowButton("Down", ImGuiDir_Down)) { processor.moveLayer(i, std::max(i - 1, 0)); processor.invalidateComposite(); changed = true; }

            ImGui::Unindent(12.0f);
            ImGui::Separator();
            ImGui::PopID();
        }
        ImGui::EndChild();
        return changed;
    }

private:
    static std::string defaultLayerName(int idx) {
        char buf[32]; std::snprintf(buf, sizeof(buf), "Layer %d", idx + 1);
        return std::string(buf);
    }
};
