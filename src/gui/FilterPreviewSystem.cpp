#include "FilterPreviewSystem.h"
#include "LoadTexture.h"
#include "../filters/GrayscaleFilter.h"
#include "../filters/InvertFilter.h"
#include "../filters/BlackAndWhiteFilter.h"
#include "../filters/BlurFilter.h"
#include "../filters/ContrastFilter.h"
#include "../filters/SaturationFilter.h"
#include "../filters/HorizontalFlipFilter.h"
#include "../filters/VerticalFlipFilter.h"
#include "../filters/RotateFilter.h"
#include "../filters/CropFilter.h"
#include "../filters/ResizeFilter.h"
#include "../filters/SkewFilter.h"
#include "../filters/MergeFilter.h"
#include "../filters/FrameFilter.h"
#include "../filters/OutlineFilter.h"
#include "../filters/PurpleFilter.h"
#include "../filters/InfraredFilter.h"
#include "../filters/WaveFilter.h"
#include "../filters/OilPaintingFilter.h"
#include "../filters/RetroFilter.h"
#include "../filters/VigentteFilter.h"
#include "../filters/WarmthFilter.h"
#include "../filters/DarkenFilter.h"
#include "../filters/LightenFilter.h"
#include <chrono>
#include <algorithm>
#include <iostream>

// Forward declare the helper classes
class PreviewTextureManager {
public:
    static GLuint createTexture(const Image& image) {
        return loadTexture(image);
    }
    
    static void deleteTexture(GLuint textureID) {
        if (textureID != 0) {
            glDeleteTextures(1, &textureID);
        }
    }
};

class ThumbnailGenerator {
public:
    static Image createThumbnail(const Image& source, int targetWidth, int targetHeight) {
        if (source.width == 0 || source.height == 0) {
            return Image();
        }
        
        // Calculate scaling factor to maintain aspect ratio
        float scaleX = static_cast<float>(targetWidth) / source.width;
        float scaleY = static_cast<float>(targetHeight) / source.height;
        float scale = std::min(scaleX, scaleY);
        
        int newWidth = static_cast<int>(source.width * scale);
        int newHeight = static_cast<int>(source.height * scale);
        
        // Ensure minimum size
        newWidth = std::max(1, newWidth);
        newHeight = std::max(1, newHeight);
        
        Image thumbnail;
        thumbnail.width = newWidth;
        thumbnail.height = newHeight;
        thumbnail.channels = source.channels;
        thumbnail.imageData = new unsigned char[newWidth * newHeight * source.channels];
        
        // Simple nearest-neighbor scaling
        for (int y = 0; y < newHeight; ++y) {
            for (int x = 0; x < newWidth; ++x) {
                int srcX = static_cast<int>(x / scale);
                int srcY = static_cast<int>(y / scale);
                
                // Clamp to source image bounds
                srcX = std::min(srcX, source.width - 1);
                srcY = std::min(srcY, source.height - 1);
                
                for (int c = 0; c < source.channels; ++c) {
                    thumbnail(x, y, c) = source(srcX, srcY, c);
                }
            }
        }
        
        return thumbnail;
    }
};

class FilterFactory {
public:
    static std::unique_ptr<Filter> createFilter(FilterType filterType) {
        switch (filterType) {
            case FilterType::Grayscale:
                return std::make_unique<GrayscaleFilter>();
            case FilterType::Invert:
                return std::make_unique<InvertFilter>();
            case FilterType::BlackAndWhite:
                return std::make_unique<BlackAndWhiteFilter>();
            case FilterType::Blur:
                return std::make_unique<BlurFilter>();
            case FilterType::Contrast:
                return std::make_unique<ContrastFilter>();
            case FilterType::Saturation:
                return std::make_unique<SaturationFilter>();
            case FilterType::Brightness:
                return std::make_unique<LightenFilter>();
            case FilterType::HorizontalFlip:
                return std::make_unique<HorizontalFlipFilter>();
            case FilterType::VerticalFlip:
                return std::make_unique<VerticalFlipFilter>();
            case FilterType::Rotate:
                return std::make_unique<RotateFilter>(90); // Default 90 degree rotation
            case FilterType::Crop:
                return std::make_unique<CropFilter>(0, 0, 100, 100); // Default crop
            case FilterType::Resize:
                return std::make_unique<ResizeFilter>(100, 100); // Default resize
            case FilterType::Skew:
                return std::make_unique<SkewFilter>(10.0); // Default 10 degree skew
            case FilterType::Merge:
                return nullptr; // Skip merge filter as it requires another image
            case FilterType::Frame:
                return nullptr; // Skip frame filter as it requires frame image
            case FilterType::Outline:
                return std::make_unique<OutlineFilter>();
            case FilterType::Purple:
                return std::make_unique<PurpleFilter>(0.5f); // Default 50% purple
            case FilterType::Infrared:
                return std::make_unique<InfraredFilter>();
            case FilterType::Wave:
                return std::make_unique<WaveFilter>(10.0f, 20.0f); // Default wave params
            case FilterType::OilPainting:
                return std::make_unique<OilPaintingFilter>();
            case FilterType::Retro:
                return std::make_unique<RetroFilter>();
            case FilterType::Vignette:
                return std::make_unique<VigentteFilter>();
            case FilterType::Warmth:
                return std::make_unique<WarmthFilter>();
            default:
                return nullptr;
        }
    }
};

FilterPreviewSystem::FilterPreviewSystem() 
    : textureManager(std::make_unique<PreviewTextureManager>())
    , thumbnailGenerator(std::make_unique<ThumbnailGenerator>())
    , filterFactory(std::make_unique<FilterFactory>())
    , isGeneratingPreviews(false)
    , imageChanged(false)
    , lastUpdateTime(std::chrono::steady_clock::now()) {
}

FilterPreviewSystem::~FilterPreviewSystem() {
    cleanup();
}

void FilterPreviewSystem::updateImage(const Image& image) {
    std::lock_guard<std::mutex> lock(previewMutex);
    
    // Check if image actually changed
    if (currentImage.width != image.width || 
        currentImage.height != image.height ||
        currentImage.channels != image.channels ||
        currentImage.imageData != image.imageData) {
        
        currentImage = image;
        imageChanged = true;
        invalidateAllPreviews();
    }
}

void FilterPreviewSystem::generateAllPreviews() {
    if (!shouldUpdatePreviews() || currentImage.width == 0 || currentImage.height == 0) {
        return;
    }
    
    std::lock_guard<std::mutex> lock(previewMutex);
    
    if (isGeneratingPreviews) {
        return;
    }
    
    isGeneratingPreviews = true;
    lastUpdateTime = std::chrono::steady_clock::now();
    
    // Generate previews for all available filter types
    std::vector<FilterType> filterTypes = {
        FilterType::Grayscale, FilterType::Invert, FilterType::BlackAndWhite,
        FilterType::Blur, FilterType::Contrast, FilterType::Saturation,
        FilterType::Brightness, FilterType::HorizontalFlip, FilterType::VerticalFlip,
        FilterType::Rotate, FilterType::Crop, FilterType::Resize,
        FilterType::Skew, FilterType::Merge, FilterType::Frame,
        FilterType::Outline, FilterType::Purple, FilterType::Infrared,
        FilterType::Wave, FilterType::OilPainting, FilterType::Retro,
        FilterType::Vignette, FilterType::Warmth
    };
    
    for (FilterType filterType : filterTypes) {
        generatePreviewAsync(filterType);
    }
}

void FilterPreviewSystem::generatePreview(FilterType filterType) {
    if (currentImage.width == 0 || currentImage.height == 0) {
        return;
    }
    
    std::lock_guard<std::mutex> lock(previewMutex);
    
    // Check if preview already exists and is valid
    auto it = previews.find(filterType);
    if (it != previews.end() && it->second->isValid && !it->second->isGenerating) {
        return;
    }
    
    generatePreviewAsync(filterType);
}

void FilterPreviewSystem::generatePreviewAsync(FilterType filterType) {
    // Create or get preview object
    if (previews.find(filterType) == previews.end()) {
        previews[filterType] = std::make_unique<FilterPreview>();
    }
    
    auto& preview = previews[filterType];
    preview->isGenerating = true;
    preview->isValid = false;
    
    // Launch async task
    auto future = std::async(std::launch::async, [this, filterType]() {
        try {
            // Create thumbnail
            Image thumbnail = ThumbnailGenerator::createThumbnail(
                currentImage, THUMBNAIL_WIDTH, THUMBNAIL_HEIGHT);
            
            if (thumbnail.width == 0 || thumbnail.height == 0) {
                return;
            }
            
            // Create a copy for filter application
            Image filteredThumbnail = thumbnail;
            
            // Apply filter
            auto filter = FilterFactory::createFilter(filterType);
            if (filter) {
                filter->apply(filteredThumbnail);
            }
            
            // Update preview in main thread
            std::lock_guard<std::mutex> lock(previewMutex);
            auto it = previews.find(filterType);
            if (it != previews.end()) {
                auto& preview = it->second;
                
                // Clean up old texture
                if (preview->textureID != 0) {
                    glDeleteTextures(1, &preview->textureID);
                }
                
                // Create new texture
                preview->textureID = PreviewTextureManager::createTexture(filteredThumbnail);
                preview->thumbnail = std::move(filteredThumbnail);
                preview->isValid = true;
                preview->isGenerating = false;
            }
        } catch (const std::exception& e) {
            std::cerr << "Error generating preview for filter " << static_cast<int>(filterType) 
                      << ": " << e.what() << std::endl;
            
            std::lock_guard<std::mutex> lock(previewMutex);
            auto it = previews.find(filterType);
            if (it != previews.end()) {
                it->second->isGenerating = false;
                it->second->isValid = false;
            }
        }
    });
    
    activeTasks.push_back(std::move(future));
}

FilterPreview* FilterPreviewSystem::getPreview(FilterType filterType) {
    std::lock_guard<std::mutex> lock(previewMutex);
    
    auto it = previews.find(filterType);
    if (it != previews.end() && it->second->isValid) {
        return it->second.get();
    }
    
    return nullptr;
}

void FilterPreviewSystem::renderPreviewGrid(ImVec2 startPos, ImVec2 itemSize, int itemsPerRow) {
    ImVec2 currentPos = startPos;
    int itemCount = 0;
    
    std::lock_guard<std::mutex> lock(previewMutex);
    
    for (auto& [filterType, preview] : previews) {
        if (preview->isValid) {
            renderPreviewItem(filterType, currentPos, itemSize);
            
            itemCount++;
            if (itemCount % itemsPerRow == 0) {
                currentPos.x = startPos.x;
                currentPos.y += itemSize.y + 10; // 10px spacing
            } else {
                currentPos.x += itemSize.x + 10; // 10px spacing
            }
        }
    }
}

void FilterPreviewSystem::renderPreviewItem(FilterType filterType, ImVec2 pos, ImVec2 size) {
    auto preview = getPreview(filterType);
    if (!preview || !preview->isValid) {
        // Draw placeholder
        ImGui::SetCursorPos(pos);
        ImGui::Button("Loading...", size);
        return;
    }
    
    ImGui::SetCursorPos(pos);
    
    // Create a button with the preview image
    ImGui::PushID(static_cast<int>(filterType));
    
    if (ImGui::ImageButton("##preview", (void*)(intptr_t)preview->textureID, size)) {
        // Filter selected - this could trigger filter application
        // For now, we'll just show the filter name
        std::cout << "Selected filter: " << filterTypeName(filterType) << std::endl;
    }
    
    // Show filter name below the preview
    ImVec2 textPos = ImVec2(pos.x, pos.y + size.y + 2);
    ImGui::SetCursorPos(textPos);
    ImGui::Text("%s", filterTypeName(filterType));
    
    ImGui::PopID();
}

void FilterPreviewSystem::clearPreviews() {
    std::lock_guard<std::mutex> lock(previewMutex);
    previews.clear();
}

void FilterPreviewSystem::cleanup() {
    // Wait for all active tasks to complete
    for (auto& future : activeTasks) {
        if (future.valid()) {
            future.wait();
        }
    }
    activeTasks.clear();
    
    std::lock_guard<std::mutex> lock(previewMutex);
    previews.clear();
    isGeneratingPreviews = false;
}

bool FilterPreviewSystem::isPreviewReady(FilterType filterType) const {
    std::lock_guard<std::mutex> lock(const_cast<std::mutex&>(previewMutex));
    
    auto it = previews.find(filterType);
    return it != previews.end() && it->second->isValid && !it->second->isGenerating;
}

bool FilterPreviewSystem::isAnyPreviewGenerating() const {
    std::lock_guard<std::mutex> lock(const_cast<std::mutex&>(previewMutex));
    
    for (const auto& [filterType, preview] : previews) {
        if (preview->isGenerating) {
            return true;
        }
    }
    
    return isGeneratingPreviews;
}

void FilterPreviewSystem::setThumbnailSize(int width, int height) {
    // This would require regenerating all previews
    // For now, we'll keep the static size
}

void FilterPreviewSystem::cleanupCompletedTasks() {
    activeTasks.erase(
        std::remove_if(activeTasks.begin(), activeTasks.end(),
            [](const std::future<void>& future) {
                return future.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
            }),
        activeTasks.end());
}

bool FilterPreviewSystem::shouldUpdatePreviews() const {
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastUpdateTime);
    return elapsed.count() >= UPDATE_THROTTLE_MS && imageChanged;
}

void FilterPreviewSystem::invalidateAllPreviews() {
    for (auto& [filterType, preview] : previews) {
        preview->isValid = false;
        if (preview->textureID != 0) {
            glDeleteTextures(1, &preview->textureID);
            preview->textureID = 0;
        }
    }
}