#pragma once

#include "../core/ImageProcessor.h"
#include "MemoryOperation.h"
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
        if(show){
            BlurFilter filter;
            processor.applyFilter(filter);
            std::cout << "Applied Blur Filter\n";
            
            show = false;
            textureNeedsUpdate = true;
        }
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
    // #TODO: overlay
    // #TODO: handle bounds
    void applyCrop(bool &show, bool &textureNeedsUpdate) {
        static int posX, posY, newWidth, newHeight;
        static Image originalImage;
        static bool init = false;
        if(show){
            ImGui::Begin("Crop Parameters", &show);

            if(!init){
                originalImage = processor.getCurrentImage();
                init = true;
            }

            bool changed = false;

            ImGui::Text("X:");
            ImGui::SameLine();
            if(ImGui::InputInt("##X", &posX, 0, 0))changed = true;

            ImGui::Text("Y:");
            ImGui::SameLine();
            if(ImGui::InputInt("##Y", &posY, 0, 0))changed = true;

            ImGui::Text("Width:");
            ImGui::SameLine();
            if(ImGui::InputInt("##Width", &newWidth, 0, 0))changed = true;
            
            ImGui::Text("Height:");
            ImGui::SameLine();
            if(ImGui::InputInt("##Height", &newHeight, 0, 0))changed = true;

            if(changed){
                processor.setImage(originalImage);
                CropFilter filter(posX, posY, newWidth, newHeight);
                processor.applyFilter(filter);
                textureNeedsUpdate = true;
            }
            
            ImGui::Separator();
            
            if(ImGui::Button("Apply")){
                CropFilter filter(posX, posY, newWidth, newHeight);
                processor.setImage(originalImage);
                processor.applyFilter(filter);
                std::cout << "Crop filter applied with Parameters: " << posX << " " << posY << " " << newWidth << " " << newHeight << std::endl;
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

            ImGui::End();
        }else init = false;
    }
    // #TODO: overlay
    void applyResize(bool &show, bool &textureNeedsUpdate) {
        static int newWidth, newHeight;
        static Image originalImage;
        static bool init = false;
        if(show){
            ImGui::Begin("Resize Parameters", &show);

            if(!init){
                originalImage = processor.getCurrentImage();
                init = true;
            }

            bool changed = false;
            
            ImGui::Text("Width:");
            ImGui::SameLine();
            if(ImGui::InputInt("##Width", &newWidth, 0, 0))changed = true;
            
            ImGui::Text("Height:");
            ImGui::SameLine();
            if(ImGui::InputInt("##Height", &newHeight, 0, 0))changed = true;

            if(changed){
                processor.setImage(originalImage);
                ResizeFilter filter(newWidth, newHeight);
                processor.applyFilter(filter);
                textureNeedsUpdate = true;
            }
            
            ImGui::Separator();
            
            if(ImGui::Button("Apply")){
                ResizeFilter filter(newWidth, newHeight);
                processor.setImage(originalImage);
                processor.applyFilter(filter);
                std::cout << "Resize filter applied with Parameters: " << newWidth << " " << newHeight << std::endl;
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

            ImGui::End();
        }else init = false;
    }
    void applyBrightness(bool &show, bool &textureNeedsUpdate) {
        static float factor = 1.0f;
        static Image originalImage;
        static bool init = false;
        if (show) {
            ImGui::Begin("Brightness Parameters", &show);

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

            ImGui::End();
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

    // #TODO: do this shit later
    void applyMerge(bool &show, bool &textureNeedsUpdate) {
        if(show){
            Image merge_image("assets/SampleImages/luffy.jpg");
            MergeFilter filter(merge_image); 
            processor.applyFilter(filter);
            std::cout << "Applied Merge Filter\n";
            
            show = false;
        }
    }
    void applyRotate(bool &show, bool &textureNeedsUpdate) {
    static int currentItem = 0;
    const char* items[] = {"90", "180", "270"};
    int values[] = {90, 180, 270};

    static Image originalImage;
    static bool init = false;
    if(show){
        ImGui::Begin("Rotate Parameters", &show);

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

        ImGui::End();
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
            ImGui::Begin("Purple Parameters", &show);

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

            ImGui::End();
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
            ImGui::Begin("Wave Parameters", &show);

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

            ImGui::End();
        }else init = false;
    }

    // #TODO: oil painting is fucked
    void applyOilPainting(bool &show, bool &textureNeedsUpdate) {
        static int radius, intensity;
        static Image originalImage;
        static bool init = false;
        if(show){
            ImGui::Begin("Oil Painting Parameters", &show);

            if(!init){
                originalImage = processor.getCurrentImage();
                init = true;
            }

            bool changed = false;
            
            ImGui::Text("Radius:");
            ImGui::SameLine();
            if(ImGui::InputInt("##Radius", &radius, 0, 0))changed = true;
            
            ImGui::Text("Intensity:");
            ImGui::SameLine();
            if(ImGui::InputInt("##Intensity", &intensity, 0, 0))changed = true;

            if(changed){
                processor.setImage(originalImage);
                ResizeFilter filter(radius, intensity);
                processor.applyFilter(filter);
                textureNeedsUpdate = true;
            }
            
            ImGui::Separator();
            
            if(ImGui::Button("Apply")){
                ResizeFilter filter(radius, intensity);
                processor.setImage(originalImage);
                processor.applyFilter(filter);
                std::cout << "Oil Painting filter applied with Parameters: " << radius << " " << intensity << std::endl;
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

            ImGui::End();
        }else init = false;
    }

    void applyContrast(bool &show, bool &textureNeedsUpdate) {
        static float factor = 0.0f;
        static Image originalImage;
        static bool init = false;
        if (show) {
            ImGui::Begin("Contrast Parameters", &show);

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

            ImGui::End();
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
            ImGui::Begin("Saturation Parameters", &show);

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

            ImGui::End();
        }else init = false;
    }
    void applySkew(bool &show, bool &textureNeedsUpdate) {
        static float Angle = 0.0f;
        static Image originalImage;
        static bool init = false;
        if (show) {
            ImGui::Begin("Skew Parameters", &show);

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

            ImGui::End();
        }else init = false;
    }
    void applyVignette(bool &show, bool &textureNeedsUpdate) {
        static float factor = 0.0f;
        static Image originalImage;
        static bool init = false;
        if (show) {
            ImGui::Begin("Vignette Parameters", &show);

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

            ImGui::End();
        }else init = false;
    }
    void applyWarmth(bool &show, bool &textureNeedsUpdate) {
        static float factor = 0.0f;
        static Image originalImage;
        static bool init = false;
        if (show) {
            ImGui::Begin("Warmth Parameters", &show);

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

            ImGui::End();
        }else init = false;
    }

private:
    ImageProcessor &processor;    
};