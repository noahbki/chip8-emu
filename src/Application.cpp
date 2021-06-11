#include "Application.h"

Application* Application::instance = 0;

Application::Application()
{
    
}

Application::~Application()
{
    delete instance;
}

Application* Application::the()
{
    if (!instance)
        instance = new Application();

    return instance;
}

void Application::create(int width, int height, const char* title)
{
    m_width = width;
    m_height = height;
    m_title = title;

    // FIXME: Error handling.
    SDL_Init(SDL_INIT_VIDEO);
    m_window = SDL_CreateWindow(m_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
        m_width, m_height, 0);

    SDL_DestroyWindow(m_window);
    SDL_Quit();

}