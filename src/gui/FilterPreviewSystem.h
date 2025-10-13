#pragma once

#include "../core/ImageProcessor.h"
#include "../filters/Filter.h"
#include "FilterDefs.h"
#include <GL/gl.h>
#include "imgui.h"
#include <unordered_map>
#include <memory>
#include <thread>
#include <mutex>
#include <atomic>
#include <future>
#include <vector>

// Forward declarations
class PreviewTextureManager;
class ThumbnailGenerator;
class FilterFactory;

struct FilterPreview {
    GLuint textureID;
    Image thumbnail;
    bool isValid;
    bool isGenerating;
    
    FilterPreview() : textureID(0), isValid(false), isGenerating(false) {}
    
    ~FilterPreview() {
        if (textureID != 0) {
            glDeleteTextures(1, &textureID);
        }
    }
    
    // Disable copy constructor and assignment operator
    FilterPreview(const FilterPreview&) = delete;
    FilterPreview& operator=(const FilterPreview&) = delete;
    
    // Enable move constructor and assignment operator
    FilterPreview(FilterPreview&& other) noexcept 
        : textureID(other.textureID), thumbnail(std::move(other.thumbnail)), 
          isValid(other.isValid), isGenerating(other.isGenerating) {
        other.textureID = 0;
        other.isValid = false;
        other.isGenerating = false;
    }
    
    FilterPreview& operator=(FilterPreview&& other) noexcept {
        if (this != &other) {
            if (textureID != 0) {
                glDeleteTextures(1, &textureID);
            }
            textureID = other.textureID;
            thumbnail = std::move(other.thumbnail);
            isValid = other.isValid;
            isGenerating = other.isGenerating;
            
            other.textureID = 0;
            other.isValid = false;
            other.isGenerating = false;
        }
        return *this;
    }
};

class FilterPreviewSystem {
private:
    static constexpr int THUMBNAIL_WIDTH = 200;
    static constexpr int THUMBNAIL_HEIGHT = 150;
    static constexpr int MAX_PREVIEW_CACHE_SIZE = 50;
    
    std::unordered_map<FilterType, std::unique_ptr<FilterPreview>> previews;
    std::unique_ptr<PreviewTextureManager> textureManager;
    std::unique_ptr<ThumbnailGenerator> thumbnailGenerator;
    std::unique_ptr<FilterFactory> filterFactory;
    
    // Threading support
    std::vector<std::future<void>> activeTasks;
    std::mutex previewMutex;
    std::atomic<bool> isGeneratingPreviews;
    
    // Current image reference for preview generation
    Image currentImage;
    bool imageChanged;
    bool previewsGenerated;
    
    // Performance tracking
    std::chrono::steady_clock::time_point lastUpdateTime;
    static constexpr int UPDATE_THROTTLE_MS = 100; // Minimum time between updates
    
public:
    FilterPreviewSystem();
    ~FilterPreviewSystem();
    
    // Main interface
    void updateImage(const Image& image);
    void generateAllPreviews();
    void generatePreview(FilterType filterType);
    FilterPreview* getPreview(FilterType filterType);
    
    // UI helpers
    void renderPreviewGrid(ImVec2 startPos, ImVec2 itemSize, int itemsPerRow = 3);
    void renderPreviewItem(FilterType filterType, ImVec2 pos, ImVec2 size);
    
    // Performance and cleanup
    void clearPreviews();
    void cleanup();
    bool isPreviewReady(FilterType filterType) const;
    bool isAnyPreviewGenerating() const;
    
    // Configuration
    void setThumbnailSize(int width, int height);
    ImVec2 getThumbnailSize() const { return ImVec2(THUMBNAIL_WIDTH, THUMBNAIL_HEIGHT); }
    
private:
    void generatePreviewAsync(FilterType filterType);
    void cleanupCompletedTasks();
    bool shouldUpdatePreviews() const;
    void invalidateAllPreviews();
};