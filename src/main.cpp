#include "global.hpp"

// define global variables
SDL_Window *gWindow = nullptr;
SDL_Renderer *gRenderer = nullptr;
SceneManager scene_manager;
MainMenu main_menu;

int main()
{
    if (!Window::init(gWindow))
    {
        std::cerr << "Failed to initialize window." << SDL_GetError() << std::endl;
        return 1;
    }
    if (!Renderer::init(gRenderer, gWindow))
    {
        std::cerr << "Failed to initialize renderer." << SDL_GetError() << std::endl;
        return 1;
    }

    // add scenes to the scene manager
    scene_manager.add_scene(&main_menu);

    // start at main menu scene
    scene_manager.change_scene(&main_menu);

    bool quit = false;
    while (!quit)
    {
        Uint32 start_time = SDL_GetTicks64();

        Renderer::update(gRenderer);
        Renderer::render(gRenderer);

        SDL_Event e;
        SDL_PollEvent(&e);
        if (e.type == SDL_QUIT)
        {
            quit = true;
            break;
        }
        scene_manager.handle_events();

        handle_frame_rate(start_time);
    }

    Renderer::close(gRenderer);
    Window::close(gWindow);

    return 0;
}