#pragma once
#include "FiltersMenu.h"
#include "MemoryOperation.h"
void renderGUI(ImageProcessor &processor) {
    ImGui::Begin("Image Processor");

    if(ImGui::Button("Load Image")) {
        // #TODO: Open a file dialog here
        // For now we'll load a hardcoded image.
        // processor.loadImage("assets/SampleImages/luffy.jpg");
        // std::cout << "Image loaded." << std::endl;
        bool stop = 0;
        while(!stop) {
        std::string selected = openFileDialog_Linux();
        bool manual_popup = 0;
        char manual_buf[1024] = "";

            if(!selected.empty()) {
                // todo : make sure the directory leads to an image
                // std::cout << "Selected : " << selected << '\n';
            std::cout << "loaded Image Successfully\n";
            processor.loadImage(selected);
            stop = 1;
        }else {
            std::cout << "try again\n";
        }
    }
        // if(manual_popup) {

        // }
    }

    ImGui::SameLine();
    if(ImGui::Button("Save Image")) {
        if(processor.saveImage("output.jpg")) std::cout << "Image saved to output.jpg" << std::endl;
        else std::cerr << "Failed to save image." << std::endl;
    }
    
    ImGui::Separator();

    filtersMenu(processor);

    ImGui::Separator();

    ImGui::Text("History:");
    if(ImGui::Button("Undo")) {
        if(processor.undo()) std::cout << "Undo successful." << std::endl;
        else std::cout << "Nothing to undo." << std::endl;
    }

    ImGui::SameLine();
    if(ImGui::Button("Redo")) {
        if(processor.redo()) std::cout << "Redo successful." << std::endl;
        else std::cout << "Nothing to redo." << std::endl;
    }

    ImGui::End();
}