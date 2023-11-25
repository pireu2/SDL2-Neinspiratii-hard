#include <iostream>
#include "glm/glm.hpp"
#include <SDL.h>
#include <vector>

//define window dimensions
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const float GLOBAL_ANIMATION_SPEED = 1.0f;

//define SDL Window related variables
SDL_Window* window = NULL;
SDL_Renderer* windowRenderer = NULL;
SDL_Event currentEvent;
Uint64 startTime, endTime, elapseTime;
bool quit = false;

int mouseX, mouseY;



struct Projectile {
    int x;
    int y;
    int height;
    int width;
    float speed;
    glm::vec3 color;

    SDL_Rect getSDLRect() {
        SDL_Rect rect;
        rect.x = x;
        rect.y = y;
        rect.w = width;
        rect.h = height;

        return rect;
    }

    void draw() {
        SDL_SetRenderDrawColor(windowRenderer, color.r, color.g, color.b, 255);
        SDL_Rect temp = this->getSDLRect();
        SDL_RenderFillRect(windowRenderer, &temp);
    }

    void processMovement(Uint64 dt) {
        const Uint8* state = SDL_GetKeyboardState(nullptr);
        y -= dt * speed * GLOBAL_ANIMATION_SPEED;
    }
};

struct Player {
    int x;
    int y;
    int height;
    int width;
    float speed;
    glm::vec3 color;

    SDL_Rect getSDLRect() {
        SDL_Rect rect;
        rect.x = x;
        rect.y = y;
        rect.w = width;
        rect.h = height;

        return rect;
    }

    void draw() {
        SDL_SetRenderDrawColor(windowRenderer, color.r, color.g, color.b, 255);
        SDL_Rect temp = this->getSDLRect();
        SDL_RenderFillRect(windowRenderer, &temp);
    }

    void processMovement(Uint64 dt) {
        const Uint8* state = SDL_GetKeyboardState(nullptr);
        if (state[SDL_SCANCODE_RIGHT]) {
            x += dt * speed * GLOBAL_ANIMATION_SPEED;
            if (x > WINDOW_WIDTH - width) {
                x = WINDOW_WIDTH -  width;
            }
        }
        if (state[SDL_SCANCODE_LEFT]) {
            x -= dt * speed * GLOBAL_ANIMATION_SPEED;
            if (x < 0) {
                x = 0;
            }
        }
    }
    void shoot() {
        std::cout << "da\n";
        Projectile projectile;
        projectile.width = 10;
        projectile.height = 10;
        projectile.x = player.x + player.width / 2 - projectile.width / 2; 
        projectile.y = player.y;
        projectile.color = glm::vec3(255,165,0);
        projectiles.push_back(projectile);
    }
};

struct Alien {
    int x;
    int y;
    int height;
    int width;
    glm::vec3 color;
};

Player player;
std::vector<Projectile> projectiles;

bool initWindow() {

    bool success = true;

    //Try to initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {

        std::cout << "SDL initialization failed" << std::endl;
        success = false;

    }
    else {

        //Try to create the window
        window = SDL_CreateWindow(
            "Hello SDL2 Window!",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            WINDOW_WIDTH,
            WINDOW_HEIGHT,
            SDL_WINDOW_SHOWN);

        if (window == NULL) {

            std::cout << "Failed to create window: " << SDL_GetError() << std::endl;
            success = false;

        }
        else {

            // Create a renderer for the current window
            windowRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

            if (windowRenderer == NULL) {

                std::cout << "Failed to create the renderer: " << SDL_GetError() << std::endl;
                success = false;

            }
            else {

                //Set background color
                SDL_SetRenderDrawColor(windowRenderer, 255, 255, 255, 255);

                //Apply background color
                SDL_RenderClear(windowRenderer);
            }
        }
    }

    return success;
}

void processEvents() {

    //Check for events in queue
    SDL_PollEvent(&currentEvent);

    //User requests quit
    if (currentEvent.type == SDL_QUIT) {

        quit = true;
    }

    //Keyboard event
    if (currentEvent.type == SDL_KEYDOWN) {

        switch (currentEvent.key.keysym.sym) {

        case SDLK_UP:
            break;

        case SDLK_DOWN:
            break;

        case SDLK_LEFT:
            break;

        case SDLK_RIGHT:
            break;

        case SDLK_r:
            break;

        case SDLK_s:
            break;

        case SDLK_ESCAPE:

            quit = true;
            break;

        default:
            break;
        }
    }
}

void drawFrame() {

    //Clear the background
    SDL_SetRenderDrawColor(windowRenderer, 255, 255, 255, 255);
    SDL_RenderClear(windowRenderer);

    //Draw scene
    player.draw();
    for (auto& projectile : projectiles) {
        projectile.draw();
    }
    //TODO

    //Update window
    SDL_RenderPresent(windowRenderer);
}

void cleanup() {

    //Destroy renderer
    if (windowRenderer) {

        SDL_DestroyRenderer(windowRenderer);
        windowRenderer = NULL;
    }

    //Destroy window
    if (window) {

        SDL_DestroyWindow(window);
        window = NULL;
    }

    //Quit SDL
    SDL_Quit();
}

void initGame() {
    player.x = 375;
    player.y = 550;
    player.width = 50;
    player.height = 50;
    player.color = glm::vec3(1.0, 0, 0);
    player.speed = 1.0f;
}


void movement(Uint64 dt) {
    player.processMovement(dt);
    for (auto& projectile : projectiles) {
        projectile.processMovement(dt);
    }
}

int main(int argc, char* argv[]) {

    //Initialize window
    if (!initWindow()) {

        std::cout << "Failed to initialize" << std::endl;
        return -1;
    }

    initGame();


    //Game loop
    while (!quit) {
        startTime = SDL_GetTicks64();
        processEvents();
        drawFrame();
        movement(elapseTime);

        const Uint8* state = SDL_GetKeyboardState(nullptr);
        if (state[SDL_SCANCODE_SPACE]) {
            player.shoot();
        }


        endTime = SDL_GetTicks64();
        elapseTime = endTime - startTime;
    }

    cleanup();
    return 0;
}
