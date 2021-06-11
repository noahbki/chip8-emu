#pragma once

#include <SDL.h>

class Application {
public:
    void create(int width, int height, const char* title);

    static Application* the();
private:
    static Application* instance;
    Application();
    ~Application();

    int m_width;
    int m_height;
    const char* m_title;

    SDL_Window* m_window;

};