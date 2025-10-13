#include <iostream>
#include <memory>
#include "src/gui/FilterPreviewSystem.h"
#include "src/gui/FilterPreviewPanel.h"
#include "src/core/ImageProcessor.h"

int main() {
    std::cout << "Testing Filter Preview System..." << std::endl;
    
    try {
        // Test FilterPreviewSystem creation
        auto previewSystem = std::make_unique<FilterPreviewSystem>();
        std::cout << "✓ FilterPreviewSystem created successfully" << std::endl;
        
        // Test FilterPreviewPanel creation
        auto previewPanel = std::make_unique<FilterPreviewPanel>();
        std::cout << "✓ FilterPreviewPanel created successfully" << std::endl;
        
        // Test ImageProcessor creation
        ImageProcessor processor;
        std::cout << "✓ ImageProcessor created successfully" << std::endl;
        
        // Test filter type names
        std::cout << "✓ Available filter types:" << std::endl;
        std::cout << "  - " << filterTypeName(FilterType::Grayscale) << std::endl;
        std::cout << "  - " << filterTypeName(FilterType::Blur) << std::endl;
        std::cout << "  - " << filterTypeName(FilterType::Contrast) << std::endl;
        std::cout << "  - " << filterTypeName(FilterType::Saturation) << std::endl;
        
        std::cout << "\n✓ All core components initialized successfully!" << std::endl;
        std::cout << "✓ Filter preview system is ready for use." << std::endl;
        
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "✗ Error: " << e.what() << std::endl;
        return 1;
    }
}