#pragma once
#include "helpers/Image_Class.h"
#include "helpers/apply_filter.h"

void displayMenu(Image &image, string &dir) {
    vector<string> filters{"Black & White", "Darken & Lighten", "Add Frame", "Grayscale", "Flip", "Invert", "Merge",
        "Resize", "Rotate", "Blur", "Crop", "Outline", "Purple"};
    
    int input;
    do {
        cout << "\nMenu:\n";
        cout << "1. Load a new image\n";
        cout << "2. Save current image\n";

        // Number of filters
        int filterCnt = filters.size();

        for(int i = 0; i < filterCnt; i++) cout << i + 3 << ". Apply " << filters[i] << " filter\n";

        cout << filters.size() + 3 << ". To exit program\n";
        cin >> input;

        // Load a new image
        if(input == 1) {
            cout << "Enter the filename of the new image: ";
            cin >> dir;
            image.loadNewImage(dir);
            cout << "Image loaded successfully!" << endl;
        }
        // Save the current image
        else if(input == 2) {
            cout << "Enter the filename to save the current image (with extension .jpg, .bmp, .png): ";
            cin >> dir;
            image.saveImage(dir);
            cout << "Image saved successfully!" << endl;
        }

        // Apply Filters
        else if(input >= 3 && input < 3 + filterCnt) {
            Filter filter;
            switch(input) {
                case 3: {
                    filter.blackandwhite(image);
                    break;
                }
                    
                case 4: {
                    int choice;
                    cout << "1. Darken\n2. Lighten\nChoose an option: ";
                    cin >> choice;
                    if(choice == 1) filter.darken(image);
                    else if(choice == 2) filter.lighten(image);
                    else cout << "Invalid choice. Returning to main menu." << endl;
                    break;
                }
                    
                case 5: { 
                    string frame_image_directory;
                    cout << "Enter the filename of the frame image: ";
                    cin >> frame_image_directory;
                    Image frame_image(frame_image_directory);
                    filter.frame(image, frame_image);
                    break;
                }
                    
                case 6: {
                    filter.gray(image);
                    break;
                }
                    
                case 7: {
                    int choice;
                    cout << "1. Horizontal Flip\n2. Vertical Flip\nChoose an option: ";
                    cin >> choice;
                    if(choice == 1) filter.h_flip(image);
                    else if(choice == 2) filter.v_flip(image);
                    else cout << "Invalid choice. Returning to main menu." << endl;
                    break;
                }
                    
                case 8: {
                    filter.invert(image);
                    break;
                }
                    
                case 9: {
                    string merge_image_directory;
                    cout << "Enter the filename of the image to merge with: ";  
                    cin >> merge_image_directory;
                    Image merge_image(merge_image_directory);
                    filter.merge(image, merge_image);
                    break;
                }
                    
                case 10: {
                    int choice;
                    cout << "1. By Ratio\n2. By Dimensions\nChoose an option: ";
                    cin >> choice;
                    if(choice == 1) {
                        double ratioX, ratioY;
                        cout << "Enter the ratio for width (e.g., 0.5 for half): ";
                        cin >> ratioX >> ratioY;
                        filter.resize(image, ratioX, ratioY);
                    }
                    else if(choice == 2) {
                        int newWidth, newHeight;
                        cout << "Enter the new width and height: ";
                        cin >> newWidth >> newHeight;
                        filter.resize(image, newWidth, newHeight);
                    }
                    else cout << "Invalid choice. Returning to main menu." << endl;
                    break;
                }
                    
                case 11: {
                    int angle;
                    cout << "Enter the angle to rotate (90, 180, 270): ";
                    cin >> angle;
                    if(angle == 90 || angle == 180 || angle == 270) filter.rotate(image, angle);
                    else cout << "Invalid angle. Returning to main menu." << endl;
                    break;
                }
                    
                case 12: {
                    filter.blur(image);
                    break;
                }
                    
                case 13: {
                    int x1, y1, w, h;
                    cout << "Enter the top-left x and y coordinates, width and height for cropping: ";
                    cin >> x1 >> y1 >> w >> h; 
                    filter.crop(image, x1, y1, w, h);
                    break;
                }
                    
                case 14: {
                    filter.edge(image);
                    break;
                }

                case 15: {
                    filter.purple(image);
                    break;
                }
            }
        }

        // Exit the program
        else if(input == filters.size() + 3) {
            cout << "Exiting the program. Goodbye!" << endl;
        }
        // Invalid option
        else {
            cout << "Invalid option. Please try again." << endl;
        }

    } while(input != filters.size() + 3);
}