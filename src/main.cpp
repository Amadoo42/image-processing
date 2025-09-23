/*
This file contains the main functions for the image processing project, which is mainly the menu stuff.
TODO: Wtf is TGA
TODO: import images from "Sample Images" folder
*/

#include <bits/stdc++.h>
using namespace std;
#include "helpers/Image_Class.h"
#include "filters/blackandwhite.h"
#include "filters/darken_lighten.h"
#include "filters/frame.h"
#include "filters/gray.h"
#include "filters/h_flip.h"
#include "filters/v_flip.h"
#include "filters/invert.h"
#include "filters/merge.h"
#include "filters/resize.h"
#include "filters/rotate.h"
#include "filters/blur.h"
#include "filters/crop.h"
// #include "filters/outline.h"

int main() {
    string filename;
    // Get the filename from the user
    cout << "Enter the filename of the image: ";
    cin >> filename;
    Image image(filename);
    cout << "Image loaded successfully!" << endl;

    // Main menu loop
    int input;
    do {
        cout << "\nMenu:\n";
        cout << "1. Load a new image\n";
        string filters[] = {"Black & White", "Darken & Lighten", "Add Frame", "Grayscale", "Flip", "Invert", "Merge",
            "Resize", "Rotate", "Blur", "Crop", "Outline"};

        // Number of filters
        int filterCnt = 12;

        for(int i = 0; i < filterCnt; i++) cout << i + 3 << ". Apply " << filters[i] << " filter\n";

        cin >> input;

        // Load a new image
        if(input == 1) {
            cout << "Enter the filename of the new image: ";
            cin >> filename;
            image.loadNewImage(filename);
            cout << "Image loaded successfully!" << endl;
        }
        // Save the current image
        else if(input == 2) {
            cout << "Enter the filename to save the current image (with extension .jpg, .bmp, .png): ";
            cin >> filename;
            image.saveImage(filename);
            cout << "Image saved successfully!" << endl;
        }
        else if(input >= 3 && input <= filterCnt + 2) {
            switch(input) {
                case 3:
                    blackandwhite(image);
                    break;
                case 4:
                    {
                        int choice;
                        cout << "1. Darken\n2. Lighten\nChoose an option: ";
                        cin >> choice;
                        if(choice == 1) darken(image);
                        else if(choice == 2) lighten(image);
                        else cout << "Invalid choice. Returning to main menu." << endl;
                    }
                    break;
                case 5:
                    string frame_image_directory;
                    cout << "Enter the filename of the frame image: ";
                    cin >> frame_image_directory;
                    Image frame_image(frame_image_directory);
                    frame(image, frame_image);
                    break;
                case 6:
                    gray(image);
                    break;
                case 7:
                    {
                        int choice;
                        cout << "1. Horizontal Flip\n2. Vertical Flip\nChoose an option: ";
                        cin >> choice;
                        if(choice == 1) h_flip(image);
                        else if(choice == 2) v_flip(image);
                        else cout << "Invalid choice. Returning to main menu." << endl;
                    }
                    break;
                case 8:
                    invert(image);
                    break;
                case 9:
                    {
                        string merge_image_directory;
                        cout << "Enter the filename of the image to merge with: ";  
                        cin >> merge_image_directory;
                        Image merge_image(merge_image_directory);
                        merge(image, merge_image);
                    }
                    break;
                case 10:
                    {
                        int choice;
                        cout << "1. By Ratio\n2. By Dimensions\nChoose an option: ";
                        cin >> choice;
                        if(choice == 1) {
                            double ratioX, ratioY;
                            cout << "Enter the ratio for width (e.g., 0.5 for half): ";
                            cin >> ratioX >> ratioY;
                            resize(image, ratioX, ratioY);
                        }
                        else if(choice == 2) {
                            int newWidth, newHeight;
                            cout << "Enter the new width and height: ";
                            cin >> newWidth >> newHeight;
                            resize(image, newWidth, newHeight);
                        }
                        else cout << "Invalid choice. Returning to main menu." << endl;
                    }
                    break;
                case 11:
                    {
                        int angle;
                        cout << "Enter the angle to rotate (90, 180, 270): ";
                        cin >> angle;
                        if(angle == 90 || angle == 180 || angle == 270) rotate(image, angle);
                        else cout << "Invalid angle. Returning to main menu." << endl;
                    }
                    break;
                case 12:
                    blur(image);
                    break;
                case 13:
                    {
                        int x1, y1, w, h;
                        cout << "Enter the top-left x and y coordinates, width and height for cropping: ";
                        cin >> x1 >> y1 >> w >> h; 
                        crop(image, x1, y1, w, h);
                    }
                    break;
                case 14:
                    // outline(image);
                    break;
            }
            cout << filters[input - 3] << " filter applied successfully!" << endl;
        }
        // Exit the program
        else if(input == 15) {
            cout << "Exiting the program. Goodbye!" << endl;
        }
        // Invalid option
        else {
            cout << "Invalid option. Please try again." << endl;
        }

    } while(input != 15);

    return 0;
}