#pragma once
#include "../core/ImageProcessor.h"
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

class FilterParameters {
public:
    FilterParameters(ImageProcessor &processor) : processor(processor) {};

    void applyGrayscale() {
        GrayscaleFilter filter;
        processor.applyFilter(filter);
        std::cout << "Applied Grayscale Filter\n";
    }
    void applyInvert() {
        InvertFilter filter;
        processor.applyFilter(filter);
        std::cout << "Applied Invert Filter\n";
    }
    void applyBlur() {
        BlurFilter filter;
        processor.applyFilter(filter);
        std::cout << "Applied Blur Filter\n";
    }
    void applyBlackAndWhite() {
        BlackAndWhiteFilter filter;
        processor.applyFilter(filter);
        std::cout << "Applied Black and White Filter\n";
    }
    void applyCrop() {
        CropFilter filter(50, 50, 200, 200); 
        processor.applyFilter(filter);
        std::cout << "Applied Crop Filter\n";
    }
    void applyResize() {
        ResizeFilter filter(400, 400); 
        processor.applyFilter(filter);
        std::cout << "Applied Resize Filter\n";
    }
    void applyDarken() {
        DarkenFilter filter; 
        processor.applyFilter(filter);
        std::cout << "Applied Darken Filter\n";
    }
    void applyLighten() {
        LightenFilter filter; 
        processor.applyFilter(filter);
        std::cout << "Applied Lighten Filter\n";
    }
    void applyFrame() {
        Image frame_image("assets/fancy.png");
        FrameFilter filter(frame_image);
        processor.applyFilter(filter);
        std::cout << "Applied Frame Filter\n";
    }
    void applyHorizontalFlip() {
        HorizontalFlipFilter filter;
        processor.applyFilter(filter);
        std::cout << "Applied Horizontal Flip Filter\n";
    }
    void applyVerticalFlip() {
        VerticalFlipFilter filter;
        processor.applyFilter(filter);
        std::cout << "Applied Vertical Flip Filter\n";
    }
    void applyMerge() {
        Image merge_image("assets/SampleImages/luffy.jpg");
        MergeFilter filter(merge_image); 
        processor.applyFilter(filter);
        std::cout << "Applied Merge Filter\n";
    }
    void applyRotate() {
        RotateFilter filter(90); 
        processor.applyFilter(filter);
        std::cout << "Applied Rotate Filter\n";
    }
    void applyOutline() {
        OutlineFilter filter;
        processor.applyFilter(filter);
        std::cout << "Applied Outline Filter\n";
    }
    void applyPurple() {
        PurpleFilter filter;
        processor.applyFilter(filter);
        std::cout << "Applied Purple Filter\n";
    }
    void applyInfrared() {
        InfraredFilter filter;
        processor.applyFilter(filter);
        std::cout << "Applied Infrared Filter\n";
    }
    void applyWave() {
        WaveFilter filter; 
        processor.applyFilter(filter);
        std::cout << "Applied Wave Filter\n";
    }
    void applyOilPainting() {
        OilPaintingFilter filter; 
        processor.applyFilter(filter);
        std::cout << "Applied Oil Painting Filter\n";
    }
    void applyContrast() {
        ContrastFilter filter(1.5); 
        processor.applyFilter(filter);
        std::cout << "Applied Contrast Filter\n";
    }
    void applyRetro() {
        RetroFilter filter;
        processor.applyFilter(filter);
        std::cout << "Applied Retro Filter\n";
    }
    void applySaturation() {
        SaturationFilter filter(1.5); 
        processor.applyFilter(filter);
        std::cout << "Applied Saturation Filter\n";
    }
    void applySkew() {
        SkewFilter filter(40); 
        processor.applyFilter(filter);
        std::cout << "Applied Skew Filter\n";
    }
    void applyVignette() {
        VigentteFilter filter(0.5); 
        processor.applyFilter(filter);
        std::cout << "Applied Vignette Filter\n";
    }
    void applyWarmth(bool &showWarmthWindow) {
        static float factor = 1.0f;
        static Image originalImage;
        static bool init = false;
        if (showWarmthWindow) {
            ImGui::Begin("Warmth Parameters", &showWarmthWindow);

            if(!init){
                originalImage = processor.getCurrentImage();
                init = true;
            }


            ImGui::Text("Adjust warmth factor:");
            ImGui::SameLine();
            bool changed = false;
            if(ImGui::SliderFloat("##FactorSlider", &factor, 0.1f, 5.0f, "%.2f"))changed = true;

            
            ImGui::SameLine();
            if(ImGui::InputFloat("##FactorInput", &factor, 0.0f, 0.0f, "%.2f"))changed = true;

            if(changed){
                processor.setImage(originalImage);
                WarmthFilter filter(factor);
                processor.applyFilter(filter);
            }
            ImGui::Separator();

            if (ImGui::Button("Apply")) {
                WarmthFilter filter(factor);
                processor.setImage(originalImage);
                processor.applyFilter(filter);
                std::cout << "Warmth filter applied with factor: " << factor << std::endl;
                showWarmthWindow = false;
                init = false;
            }

            ImGui::SameLine();
            if (ImGui::Button("Cancel")) {
                processor.setImage(originalImage);
                showWarmthWindow = false;
                init = false;
            }

            ImGui::End();
        }else init = false;
    }

private:
    ImageProcessor &processor;    
};