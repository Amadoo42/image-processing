#pragma once
#include <GL/gl.h>
#include "../core/ImageProcessor.h"

inline GLuint loadTexture(const Image &image) {
    if(image.width == 0 || image.height == 0) {
        std::cerr << "Invalid image data for texture." << std::endl;
        return 0;
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.width, image.height, 0, GL_RGB, GL_UNSIGNED_BYTE, image.imageData);

    return textureID;
}