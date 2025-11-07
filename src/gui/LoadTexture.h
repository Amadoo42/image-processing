/*
Contains the loadTexture function, which converts the raw data from the 
custom Image class into an OpenGL texture ID (GLuint) for efficient display by ImGui.
*/

#pragma once
#include <GL/gl.h>
#include "../../external/Image_Class.h"

inline GLuint loadTexture(const Image &image) {
    if(image.width == 0 || image.height == 0) {
        return 0;
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    GLenum format = (image.channels == 4) ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, format, image.width, image.height, 0, format, GL_UNSIGNED_BYTE, image.imageData);

    return textureID;
}