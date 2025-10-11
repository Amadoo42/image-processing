// sdl_gl_openfile.cpp
// Build: g++ sdl_gl_openfile.cpp -lSDL2 -lGL -std=c++17 -O2

#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <cstdio>
#include <memory>
#include <array>
#include <string>
#include <vector>
#include <iostream>

// stb_image: https://github.com/nothings/stb/blob/master/stb_image.h
// Put stb_image.h alongside this file and #define STB_IMAGE_IMPLEMENTATION once.
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// ------------------ openFileDialog (Linux) ------------------
static std::string runCommandCapture(const char* cmd) {
    std::array<char, 4096> buf;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) return {};
    while (fgets(buf.data(), buf.size(), pipe.get()) != nullptr) result += buf.data();
    if (!result.empty() && result.back() == '\n') result.pop_back();
    return result;
}

std::string openFileDialog_Linux() {
    // Try kdialog (KDE)
    // kdialog prints the selected filename to stdout
    std::string out = runCommandCapture("which kdialog >/dev/null 2>&1 && kdialog --getopenfilename 2>/dev/null || true");
    if (!out.empty()) return out;

    // Try zenity (GNOME)
    // zenity prints the selected filename to stdout
    out = runCommandCapture("which zenity >/dev/null 2>&1 && zenity --file-selection --title=\"Select an image\" --file-filter='*.png *.jpg *.jpeg *.bmp' 2>/dev/null || true");
    if (!out.empty()) return out;

    // If neither found, return empty -> caller may implement custom in-app browser
    return std::string();
}

// ------------------ loadImageToGLTexture ------------------
GLuint loadImageToTexture(const std::string &path) {
    int w,h,n;
    unsigned char* data = stbi_load(path.c_str(), &w, &h, &n, 0);
    if (!data) {
        std::cerr << "stb_image failed to load: " << path << "\n";
        return 0;
    }

    GLenum format = GL_RGB;
    if (n == 1) format = GL_RED;
    else if (n == 3) format = GL_RGB;
    else if (n == 4) format = GL_RGBA;

    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    // simple filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
    return tex;
}

// ------------------ Simple helper: point inside rect ------------------
bool pointInRect(int x, int y, int rx, int ry, int rw, int rh) {
    return x >= rx && x <= rx + rw && y >= ry && y <= ry + rh;
}

// ------------------ main ------------------
int main(int argc, char** argv) {
    (void)argc; (void)argv;
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init error: " << SDL_GetError() << "\n";
        return 1;
    }

    // Create window with GL context (compat profile assumed)
    SDL_Window* win = SDL_CreateWindow("SDL+GL Open File Demo",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (!win) { std::cerr << "CreateWindow failed\n"; return 1; }

    SDL_GLContext glctx = SDL_GL_CreateContext(win);
    if (!glctx) { std::cerr << "GL context failed\n"; return 1; }

    // Simple app state
    bool running = true;
    GLuint texture = 0;
    int winw = 800, winh = 600;

    // Button rectangle (in window coords, origin at top-left for mouse)
    const int bx = 20, by = 20, bw = 160, bh = 40;

    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = false;
            else if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                winw = e.window.data1;
                winh = e.window.data2;
                glViewport(0,0,winw,winh);
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                int mx = e.button.x;
                int my = e.button.y;
                if (pointInRect(mx, my, bx, by, bw, bh)) {
                    // Button clicked: open file dialog
                    std::string path = openFileDialog_Linux();
                    if (!path.empty()) {
                        std::cout << "Picked: " << path << std::endl;
                        // load into GL texture
                        if (texture) { glDeleteTextures(1, &texture); texture = 0; }
                        texture = loadImageToTexture(path);
                        if (!texture) std::cerr << "Failed to make texture\n";
                    } else {
                        std::cout << "No dialog available or user cancelled.\n";
                        // (Optional) fallback: implement an in-app file browser here.
                    }
                }
            }
        }

        // --- Render ---
        glClearColor(0.2f,0.2f,0.25f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw a simple button (using immediate mode for simplicity)
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, winw, winh, 0, -1, 1); // top-left origin
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        // Button background
        glColor3f(0.8f, 0.3f, 0.2f);
        glBegin(GL_QUADS);
            glVertex2i(bx, by);
            glVertex2i(bx + bw, by);
            glVertex2i(bx + bw, by + bh);
            glVertex2i(bx, by + bh);
        glEnd();

        // (Very simple text alternative)
        // Real text: use a bitmap font, SDL_ttf, or textured glyphs. Here we skip text drawing.

        // If texture loaded, draw centered quad showing it (maintain aspect roughly)
        if (texture) {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, texture);

            int tw = 400, th = 300;
            int x = (winw - tw) / 2;
            int y = (winh - th) / 2;

            glColor3f(1,1,1);
            glBegin(GL_QUADS);
              glTexCoord2f(0,0); glVertex2i(x,y);
              glTexCoord2f(1,0); glVertex2i(x+tw,y);
              glTexCoord2f(1,1); glVertex2i(x+tw,y+th);
              glTexCoord2f(0,1); glVertex2i(x,y+th);
            glEnd();

            glDisable(GL_TEXTURE_2D);
        }

        SDL_GL_SwapWindow(win);
        SDL_Delay(10);
    }

    if (texture) glDeleteTextures(1, &texture);
    SDL_GL_DeleteContext(glctx);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
