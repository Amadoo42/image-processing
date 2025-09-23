/*
This file contains the main functions for the image processing project, which is mainly the menu stuff.
TODO: Wtf is TGA
TODO: import images from "Sample Images" folder
*/

#include <bits/stdc++.h>
using namespace std;
#include "helpers/Image_Class.h"

int main() {
    string filename;
    // Get the filename from the user
    cout << "Enter the filename of the image: ";
    cin >> filename;
    Image image(filename);
    cout << "Image loaded successfully!" << endl;

    // Main menu loop
    string input;
    do {
        cout << "\nMenu:\n";
        cout << "1. Load a new image\n";
        int cnt=2;
        string filters[] = {"Black&White", "Darken&lighten", "Add frame", "Grayscale", "Flip", "Invert", "Merge",
            "Resize", "Rotate", "Blur", "Crop", "Image Edge"};
        for(auto filter:filters){
            cout<<cnt++<<" "<<". "<<filter<<endl;
        }
        cout << cnt++<<". Save the current image\n";
        cout << cnt++<<". Exit\n";
        cout << "Choose an option: (1-"<<cnt<<"): " << endl;
        cin >> input;

        // Load a new image
        if(input == "1") {
            cout << "Enter the filename of the new image: ";
            cin >> filename;
            image.loadNewImage(filename);
            cout << "Image loaded successfully!" << endl;
        }
        else if(input == "2"){
            blackandwhite(image);
            cout<< "successful" <<endl;
        }
        else if(input == "3"){
            
        }
        // Save the current image
        else if(input == "2") {
            cout << "Enter the filename to save the current image (with extension .jpg, .bmp, .png): ";
            cin >> filename;
            image.saveImage(filename);
            cout << "Image saved successfully!" << endl;
        }
        // Exit the program
        else if(input == cnt) {
            cout << "Exiting the program. Goodbye!" << endl;
        }
        // Invalid option
        else {
            cout << "Invalid option. Please try again." << endl;
        }

    } while(input != cnt);

    return 0;
}