/*
This file contains the main functions for the image processing project, which is mainly the menu stuff.
TODO: Wtf is TGA
TODO: import images from "Sample Images" folder
TODO: Save images to the same folder as the original image
TODO: Adjust frame options in menu
TODO: Find better menu for merge
TODO: Blur is weak 
TODO: Edge is weak
*/

#include <bits/stdc++.h>
using namespace std;
#include "helpers/Image_Class.h"
#include "helpers/apply_filter.cpp"
#include "menu.h"

int main() {
    string filename;
    // Get the filename from the user
    cout << "Enter the filename of the image: ";
    cin >> filename;
    Image image(filename);
    cout << "Image loaded successfully!" << endl;

    // Main menu loop
    displayMenu(image, filename);

    return 0;
}