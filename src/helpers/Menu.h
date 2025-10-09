#pragma once

#include "ImageProcessor.h"
#include "../filters/Filter.h"
#include "../filters/BlackAndWhiteFilter.h"
#include "../filters/BlurFilter.h"
#include "../filters/CropFilter.h"
#include "../filters/DarkenFilter.h"
#include "../filters/LightenFilter.h"
#include "../filters/FrameFilter.h"
#include "../filters/GrayscaleFilter.h"
#include "../filters/HorizontalFlipFilter.h"
#include "../filters/VerticalFlipFilter.h"
#include "../filters/MergeFilter.h"
#include "../filters/ResizeFilter.h"
#include "../filters/RotateFilter.h"
#include "../filters/InvertFilter.h"
#include "../filters/OutlineFilter.h"
#include "../filters/Purple.h"
#include "../filters/Infrared.h"
#include "../filters/Wave.h"
#include "../filters/oilpainting.h"
#include "../filters/retro.h"

// #TODO: Lots of error handling here

class Menu {
    private:
    ImageProcessor processor;
    string filename;

    void displayMainMenu() {
        cout << "\n=== Image Processing Menu ===" << endl;
        cout << "1. Load a New Image" << endl;
        cout << "2. Save Current Image" << endl;
        cout << "3. Apply Filter" << endl;
        cout << "4. Undo" << endl;
        cout << "5. Redo" << endl;
        cout << "0. Exit" << endl;
        cout << "Choice: ";
    }

    void displayFilterMenu() {
        cout << "\n=== Select Filter ===" << endl;
        cout << "1. Black & White" << endl;
        cout << "2. Darken & Lighten" << endl;
        cout << "3. Add Frame" << endl;
        cout << "4. Grayscale" << endl;
        cout << "5. Flip" << endl;
        cout << "6. Invert" << endl;
        cout << "7. Merge" << endl;
        cout << "8. Resize" << endl;
        cout << "9. Rotate" << endl;
        cout << "10. Blur" << endl;
        cout << "11. Crop" << endl;
        cout << "12. Outline" << endl;
        cout << "13. Purple" << endl;
        cout << "14. Infrared" << endl;
        cout << "15. Wave" << endl;
        cout << "16. Oil Painting" << endl;
        cout << "17. retro TV" << endl;
        cout << "0. Back to Main Menu" << endl;
        cout << "Choice: ";
    }

    void handleLoadImage() {
        cout << "Enter image filename: ";
        cin >> filename;
        processor.loadImage(filename);
        cout << "Image loaded successfully!\n";
    }

    void handleSaveImage() {
        int choice;
        cout << "Save in same directory?\n";
        cout << "1. Yes\n";
        cout << "2. No\n";
        cout << "Choice: ";
        cin >> choice;
        while(choice != 1 && choice != 2){
            cout << "Invalid Option. Please try again\n";
            cout << "Choice: ";
            cin >> choice;
        }
        if(choice == 1){
            processor.saveImage(filename);
            cout << "Image saved successfully!\n";
        }
        else{
            cout << "Enter the filename to save the current image (with extension .jpg, .bmp, .png): ";
            cin >> filename;
            if (processor.saveImage(filename)) cout << "Image saved successfully!\n";
            else cout << "Failed to save image. Please check the filename and try again.\n";
        }
    }

    void applyBlackAndWhite() {
        BlackAndWhiteFilter filter;
        processor.applyFilter(filter);
        cout << "Filter applied successfully!\n";
    }

    void applyDarkenAndLighten() {
        int choice;
        cout << "1. Darken\n2. Lighten\nChoose an option: ";
        cin >> choice;
        while(choice != 1 && choice != 2){
            cout << "Invalid Option. Please try again\n";
            cout << "Choice: ";
            cin >> choice;
        }
        if(choice == 1){
            DarkenFilter filter;
            processor.applyFilter(filter);
            cout << "Filter applied successfully!\n";
        }
        else if(choice == 2){
            LightenFilter filter;
            processor.applyFilter(filter);
            cout << "Filter applied successfully!\n";
        }
    }

    void applyFrame() {
        string frame_image_directory;
        int choice;
        cout << "1. Choose a frame\n";
        cout << "2. Load your own frame\n";
        cin >> choice;
        while(choice != 1 && choice != 2){
            cout << "Invalid Option. Please try again\n";
            cout << "Choice: ";
            cin >> choice;
        }
        if(choice == 1){
            cout << "1. Normal frame\n";
            cout << "2. Fancy frame!\n";
            cout << "Choice: ";
            cin >> choice;
            while(choice != 1 && choice != 2){
                cout << "Invalid Option. Please try again\n";
                cout << "Choice: ";
                cin >> choice;
            }
            if(choice == 1)frame_image_directory = "images/normal.jpg";// put image path here
            else frame_image_directory = "images/fancy.jpg";// put image path here
        }
        else{
            cout << "Enter the filename of the frame image: ";
            cin >> frame_image_directory;
        }
        Image frame_image(frame_image_directory);
        FrameFilter filter(frame_image);
        processor.applyFilter(filter);
        cout << "Filter applied successfully!\n";
    }

    void applyGrayscale() {
        GrayscaleFilter filter;
        processor.applyFilter(filter);
        cout << "Filter applied successfully!\n";
    }
    
    void applyFlip() {
        int choice;
        cout << "1. Horizontal Flip\n2. Vertical Flip\nChoose an option: ";
        cin >> choice;
        while(choice != 1 && choice != 2){
            cout << "Invalid Option. Please try again\n";
            cout << "Choice: ";
            cin >> choice;
        }
        if(choice == 1){
            HorizontalFlipFilter filter;
            processor.applyFilter(filter);
            cout << "Filter applied successfully!\n";
        }
        else if(choice == 2){
            VerticalFlipFilter filter;
            processor.applyFilter(filter);
            cout << "Filter applied successfully!\n";
        }
    }

    void applyInvert() {
        InvertFilter filter;
        processor.applyFilter(filter);
        cout << "Filter applied successfully!\n";
    }

    void applyMerge() {
        string merge_image_directory;
        cout << "Enter the filename of the image to merge with: ";  
        cin >> merge_image_directory;
        Image merge_image(merge_image_directory);
        MergeFilter filter(merge_image);
        processor.applyFilter(filter);
        cout << "Filter applied successfully!\n";
    }

    void applyResize() {
        int choice;
        cout << "1. By Ratio\n2. By Dimensions\nChoose an option: ";
        cin >> choice;
        while(choice != 1 && choice != 2){
            cout << "Invalid Option. Please try again\n";
            cout << "Choice: ";
            cin >> choice;
        }
        if(choice == 1) {
            double ratioX, ratioY;
            cout << "Enter the ratio for width (e.g., 0.5 for half): ";
            cin >> ratioX;
            cout << "Enter the ratio for height: ";
            cin >> ratioY;
            ResizeFilter filter(ratioX, ratioY);
            processor.applyFilter(filter);
            cout << "Filter applied successfully!\n";
        }
        else if(choice == 2) {
            int newWidth, newHeight;
            cout << "Enter the new width: ";
            cin >> newWidth;
            cout << "Enter the new height: ";
            cin >> newHeight;
            ResizeFilter filter(newWidth, newHeight);
            processor.applyFilter(filter);
            cout << "Filter applied successfully!\n";
        }
    }

    void applyRotate() {
        int angle;
        cout << "Enter rotation angle (90, 180, 270): ";
        cin >> angle;
        while(angle != 90 && angle != 180 && angle != 270){
            cout << "Invalid angle. Please try again\n";
            cout << "Enter rotation angle (90, 180, 270): ";
            cin >> angle;
        }
        if(angle == 90 || angle == 180 || angle == 270) {
            RotateFilter filter(angle);
            processor.applyFilter(filter);
            cout << "Filter applied successfully!\n";
        }
    }

    void applyBlur() {
        BlurFilter filter;
        processor.applyFilter(filter);
        cout << "Filter applied successfully!\n";
    }

    void applyCrop() {
        int x1, y1, w, h;
        const Image &image = processor.getCurrentImage();
        cout << "Enter the top-left x and y coordinates, width and height for cropping: ";
        cin >> x1 >> y1 >> w >> h; 
        if(x1 < 0 || y1 < 0 || w < 0 || h < 0 || x1 + w >= image.width || y1 + h >= image.height){
            cout << "Invalid input. Returning to main menu\n";
        }
        else{
            CropFilter filter(x1, y1, w, h);
            processor.applyFilter(filter);
            cout << "Filter applied successfully!\n";
        } 
    }

    void applyOutline() {
        OutlineFilter filter;
        processor.applyFilter(filter);
        cout << "Filter applied successfully!\n";
    }

    void applyPurple() {
        Purple filter;
        processor.applyFilter(filter);
        cout << "Purple filter applied successfully!\n";
    }

    void applyInfrared() {
        Infrared filter;
        processor.applyFilter(filter);
        cout << "Infrared filter applied successfully!\n";
    }

    void applyWave() {
        Wave filter;
        processor.applyFilter(filter);
        cout << "Wave filter applied successfully!\n";
    }
    void applyOil() {
        oilpainting filter;
        processor.applyFilter(filter);
        cout << "Oil filter applied successfully\n";
    }
    void applyRetro() {
        retro filter;
        processor.applyFilter(filter);
        cout << "Retro TV applied successfully\n";
    }
    void handleApplyFilter() {
        int choice;
        displayFilterMenu();
        cin >> choice;
        switch(choice) {
            case 1: applyBlackAndWhite(); break;
            case 2: applyDarkenAndLighten(); break;
            case 3: applyFrame(); break;
            case 4: applyGrayscale(); break;
            case 5: applyFlip(); break;
            case 6: applyInvert(); break;
            case 7: applyMerge(); break;
            case 8: applyResize(); break;
            case 9: applyRotate(); break;
            case 10: applyBlur(); break;
            case 11: applyCrop(); break;
            case 12: applyOutline(); break;
            case 13: applyPurple(); break;
            case 14: applyInfrared(); break;
            case 15: applyWave(); break;
            case 16: applyOil(); break;
            case 17: applyRetro(); break;
            case 0: return; // Back to main menu
            default: cout << "Invalid choice. Returning to main menu." << endl; break;
        }
    }

public:
    void run() {
        int choice;
        while(true) {
            displayMainMenu();
            cin >> choice;
            switch(choice) {
                case 1: handleLoadImage(); break;
                case 2: handleSaveImage(); break;
                case 3: handleApplyFilter(); break;
                case 4: 
                    if(processor.undo()) cout << "Undo successful!\n";
                    else cout << "Nothing to undo.\n";
                    break;
                case 5: 
                    if(processor.redo()) cout << "Redo successful!\n";
                    else cout << "Nothing to redo.\n";
                    break;
                case 0: 
                    cout << "Exiting the program. Goodbye!" << endl;
                    return;
                default: cout << "Invalid choice. Please try again." << endl; break;
            }
        }
    }

};