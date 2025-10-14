#pragma once

#include "imgui.h"
#include <vector>
#include <map>
#include <mutex>
#include <string>
#include <algorithm>

#include "FilterDefs.h"
#include "LoadTexture.h"
#include "../core/ImageProcessor.h"

// Filters
#include "../filters/GrayscaleFilter.h"
#include "../filters/InvertFilter.h"
#include "../filters/BlackAndWhiteFilter.h"
#include "../filters/BlurFilter.h"
#include "../filters/DarkenFilter.h"
#include "../filters/ContrastFilter.h"
#include "../filters/SaturationFilter.h"
#include "../filters/OutlineFilter.h"
#include "../filters/PurpleFilter.h"
#include "../filters/InfraredFilter.h"
#include "../filters/WaveFilter.h"
#include "../filters/OilPaintingFilter.h"
#include "../filters/RetroFilter.h"
#include "../filters/VigentteFilter.h"
#include "../filters/WarmthFilter.h"
#include "../filters/ResizeFilter.h"

// Small cache that builds thumbnail previews for supported visual filters.
// Generation is synchronous and only runs when invalidated.
class FilterPreviewCache {
public:
    FilterPreviewCache() = default;
    ~FilterPreviewCache() { releaseTextures(); }

    // Filters we will preview (exclude non-visual/editing tools like Resize/Crop/Merge/Skew/Frame)
    static bool isPreviewable(FilterType t) {
        switch (t) {
            case FilterType::Resize:
            case FilterType::Crop:
            case FilterType::Skew:
            case FilterType::Merge:
            case FilterType::Frame:
                return false;
            default: return true;
        }
    }

    void invalidate() { cacheValid = false; }

    // Build previews if needed for the provided filter list
    void ensureBuilt(const Image &src, const std::vector<FilterType> &filters) {
        if (src.width <= 0 || src.height <= 0) return;

        size_t sig = signature(src);
        if (!cacheValid || sig != lastSignature) {
            // Rebuild base thumbnail for new source image
            releaseTextures();
            previews.clear();
            baseThumb = makeBaseThumb(src);
            cacheValid = true;
            lastSignature = sig;
        }

        // Ensure entries exist for requested filters
        for (FilterType t : filters) {
            if (!isPreviewable(t)) continue;
            if (previews.find(t) != previews.end()) continue;

            PreviewEntry entry;
            entry.type = t;
            entry.name = filterTypeName(t);
            entry.texture = 0;
            entry.built = false;

            Image thumbCopy = baseThumb;
            applyPreviewFilter(t, thumbCopy);
            entry.image = thumbCopy;
            entry.built = true;
            previews[t] = std::move(entry);
        }
    }

    // Lazily upload GL texture for a filter preview when needed
    GLuint getTexture(FilterType t) {
        auto it = previews.find(t);
        if (it == previews.end()) return 0;
        PreviewEntry &e = it->second;
        if (!e.built) return 0;
        if (e.texture == 0 && e.image.width > 0 && e.image.height > 0) {
            e.texture = loadTexture(e.image);
        }
        return e.texture;
    }

private:
    struct PreviewEntry {
        FilterType type;
        std::string name;
        Image image;
        GLuint texture = 0;
        bool built = false;
    };

    std::map<FilterType, PreviewEntry> previews;
    Image baseThumb;
    bool cacheValid = false;
    size_t lastSignature = 0;

    static size_t signature(const Image &img) {
        // Cheap content-sensitive signature: width/height plus a hash of a few hundred bytes
        // sampled from the buffer so in-place edits are detected.
        size_t h = (static_cast<size_t>(img.width) << 32) ^ static_cast<size_t>(img.height);
        const unsigned char* data = img.imageData;
        if (!data || img.width <= 0 || img.height <= 0) return h;
        const size_t total = static_cast<size_t>(img.width) * static_cast<size_t>(img.height) * 3;
        const size_t step = std::max<size_t>(1, total / 257);
        for (size_t i = 0; i < total; i += step) {
            h = (h * 1315423911u) ^ data[i];
        }
        return h;
    }

    static Image makeBaseThumb(const Image &src) {
        const int maxLongEdge = 256; // small for speed
        if (src.width <= 0 || src.height <= 0) return Image();
        int newW = src.width;
        int newH = src.height;
        if (src.width >= src.height) {
            if (src.width > maxLongEdge) {
                newW = maxLongEdge;
                newH = std::max(1, src.height * maxLongEdge / src.width);
            }
        } else {
            if (src.height > maxLongEdge) {
                newH = maxLongEdge;
                newW = std::max(1, src.width * maxLongEdge / src.height);
            }
        }
        Image resized = src;
        if (newW != src.width || newH != src.height) {
            ResizeFilter rf(newW, newH);
            rf.apply(resized);
        }
        return resized;
    }

    static void applyPreviewFilter(FilterType t, Image &img) {
        switch (t) {
            case FilterType::Grayscale: { GrayscaleFilter f; f.apply(img); break; }
            case FilterType::Invert: { InvertFilter f; f.apply(img); break; }
            case FilterType::BlackAndWhite: { BlackAndWhiteFilter f; f.apply(img); break; }
            case FilterType::Brightness: { DarkenFilter f(1.20f); f.apply(img); break; }
            case FilterType::Contrast: { ContrastFilter f(1.35); f.apply(img); break; }
            case FilterType::Saturation: { SaturationFilter f(1.35); f.apply(img); break; }
            case FilterType::Blur: { BlurFilter f(7, 1.6); f.apply(img); break; }
            case FilterType::Outline: { OutlineFilter f; f.apply(img); break; }
            case FilterType::Purple: { PurpleFilter f(1.0f); f.apply(img); break; }
            case FilterType::Infrared: { InfraredFilter f; f.apply(img); break; }
            case FilterType::Wave: { WaveFilter f(4.0f, 40.0f); f.apply(img); break; }
            case FilterType::OilPainting: { OilPaintingFilter f(4, 20); f.apply(img); break; }
            case FilterType::Retro: { RetroFilter f; f.apply(img); break; }
            case FilterType::Vignette: { VigentteFilter f(1.2); f.apply(img); break; }
            case FilterType::Warmth: { WarmthFilter f(1.0); f.apply(img); break; }
            // Not previewed / not applicable
            default: break;
        }
    }

    void releaseTextures() {
        for (auto &kv : previews) {
            if (kv.second.texture != 0) {
                glDeleteTextures(1, &kv.second.texture);
                kv.second.texture = 0;
            }
        }
    }
};

// Renders a grid of previews for the provided filters.
// Returns true if a filter tile was clicked (selectedFilter updated by reference).
inline bool renderFilterPreviewGrid(FilterPreviewCache &cache,
                                    ImageProcessor &processor,
                                    const std::vector<FilterType> &filters,
                                    FilterType &selectedFilter,
                                    bool invalidateCache,
                                    const char* tableIdSuffix,
                                    int columns = 3,
                                    ImVec2 thumbSize = ImVec2(120, 80),
                                    const Image* srcOverride = nullptr)
{
    if (invalidateCache) cache.invalidate();
    const Image &src = srcOverride ? *srcOverride : processor.getCurrentImage();
    cache.ensureBuilt(src, filters);

    bool clicked = false;
    ImGuiTableFlags flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_BordersInnerV;
    std::string tableId = std::string("FilterPreviewTable_") + tableIdSuffix;
    if (ImGui::BeginTable(tableId.c_str(), columns, flags)) {
        int c = 0;
        for (FilterType t : filters) {
            if (!FilterPreviewCache::isPreviewable(t)) continue;
            if (c % columns == 0) ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(c % columns);
            ImGui::BeginGroup();

            ImGui::PushID(static_cast<int>(t));
            GLuint tex = cache.getTexture(t);
            if (tex != 0) {
                if (ImGui::ImageButton("##prev", (void*)(intptr_t)tex, thumbSize)) {
                    selectedFilter = t;
                    clicked = true;
                }
            } else {
                // Unique placeholder ID prevents conflicts when multiple are visible
                ImGui::Button("##generating", thumbSize);
                ImVec2 p = ImGui::GetItemRectMin();
                ImVec2 q = ImGui::GetItemRectMax();
                ImVec2 center = ImVec2((p.x+q.x)*0.5f, (p.y+q.y)*0.5f);
                ImDrawList* dl = ImGui::GetWindowDrawList();
                const char* txt = "Generating...";
                ImVec2 sz = ImGui::CalcTextSize(txt);
                dl->AddText(ImVec2(center.x - sz.x*0.5f, center.y - sz.y*0.5f), IM_COL32(255,255,255,200), txt);
            }

            ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + thumbSize.x);
            ImGui::TextUnformatted(filterTypeName(t));
            ImGui::PopTextWrapPos();
            ImGui::PopID();
            ImGui::EndGroup();

            ++c;
        }
        ImGui::EndTable();
    }
    return clicked;
}
