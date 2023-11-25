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
bool canShoot = true;
int mouseX, mouseY;

enum AlienState {
    ALIEN_GREEN,
    ALIEN_ORANGE,
    ALIEN_RED,
    ALIEN_DESTROYED
};


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



std::vector<Projectile> projectiles;

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
                x = WINDOW_WIDTH - width;
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
        Projectile projectile;
        projectile.width = 10;
        projectile.height = 10;
        projectile.x = x + width / 2 - projectile.width / 2;
        projectile.y = y;
        projectile.color = glm::vec3(255, 165, 0);
        projectile.speed = 0.5f;
        projectiles.push_back(projectile);
    }
};

struct Alien {
    float x;
    float y;
    int height = 50;
    int width = 50;
    float speed = 0.5f;
    glm::vec3 color;
    int direction;
    AlienState state;

    SDL_Rect getSDLrect() {
        SDL_Rect r;
        r.x = x;
        r.y = y;
        r.w = width;
        r.h = height;
        return r;
    }

   
};

std::vector<Alien> aliens;
void initAliens() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 6; j++) {
            Alien alien;
            alien.x = 100 + j * 100;
            alien.y = 30 + i * 70;
            alien.color.r = 0.0f;
            alien.color.g = 255.0f;
            alien.color.b = 0.0f;
            alien.direction = 1;
            alien.state = ALIEN_GREEN;
            aliens.push_back(alien);
        }
    }

}

void updateAliensPos() {
    for (int i = 0; i < aliens.size(); i++) {
        aliens[i].x += aliens[i].speed * aliens[i].direction;
        if (aliens[0].x < 10)
        {
            for (auto& alien : aliens) {
                alien.direction = 1;
            }
        }
        if (aliens[5].width + aliens[5].x > WINDOW_WIDTH - 10)
        {
            for (auto& alien : aliens) {
                alien.direction = -1;
            }
        }
    }

}

bool collides_proj_alien(Projectile projectile, Alien alien) {
    SDL_Rect projectileRect = projectile.getSDLRect();
    SDL_Rect alienRect = alien.getSDLrect();

    return SDL_HasIntersection(&projectileRect, &alienRect) == SDL_TRUE;
}


Player player;


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

    for (int i = 0; i < aliens.size(); i++) {
        if (aliens[i].state == ALIEN_GREEN) {
            SDL_SetRenderDrawColor(windowRenderer, 0, 255, 0, 255);
        }
        else if (aliens[i].state == ALIEN_ORANGE) {
            SDL_SetRenderDrawColor(windowRenderer, 255, 165, 0, 255);
        }
        else if (aliens[i].state == ALIEN_RED) {
            SDL_SetRenderDrawColor(windowRenderer, 255, 0, 0, 255);
        }
        else if (aliens[i].state == ALIEN_DESTROYED) {
            SDL_SetRenderDrawColor(windowRenderer, 0, 0, 255, 255);
        }
        

        SDL_Rect alien = aliens[i].getSDLrect();
        SDL_RenderFillRect(windowRenderer, &alien);
    }
    updateAliensPos();

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

    elapseTime = 0;
}


void changeColor(Alien& alien) {
    if (alien.state == ALIEN_GREEN) {
        alien.state = ALIEN_ORANGE;
    }
    else if (alien.state == ALIEN_ORANGE) {
        alien.state = ALIEN_RED;
    }
    else if (alien.state == ALIEN_RED) {
        alien.state = ALIEN_DESTROYED;
    }
}

void movement(Uint64 dt) {
    player.processMovement(dt);
    for (auto& projectile : projectiles) {
        projectile.processMovement(dt);
    }

    for (auto it = projectiles.begin(); it != projectiles.end(); /* no increment here */) {
        for (auto& alien : aliens) {
            if (collides_proj_alien(*it, alien)) {
                // Erase projectile and update iterator
                it = projectiles.erase(it);
                std::cout << "da";
                changeColor(alien);
                // Break out of the inner loop since the projectile is removed
                break;
            }
        }
        // Increment the iterator only if the projectile was not removed
        if (it != projectiles.end()) {
            ++it;
        }
    }
    aliens.erase(std::remove_if(aliens.begin(), aliens.end(),
        [](const Alien& alien) { return alien.state == ALIEN_DESTROYED; }),
        aliens.end());

}

Uint32 shootTimer(Uint32 interval, void* name) {
    canShoot = true;
    return 0;
}

void events() {
    const Uint8* state = SDL_GetKeyboardState(nullptr);
    if (state[SDL_SCANCODE_SPACE] && canShoot) {
        canShoot = false;
        SDL_TimerID timerId = SDL_AddTimer(1000, shootTimer, nullptr);
        player.shoot();
    }
}

bool canMove = false;
Uint32 aliensTimer(Uint32 interval, void* name) {
    canMove = true;
    return interval;
}

void aliensDown() {
    if (canMove) {
        for (auto& alien : aliens) {
            alien.y += 50;
        }
        canMove = false;
    }

}



int main(int argc, char* argv[]) {

    //Initialize window
    if (!initWindow()) {

        std::cout << "Failed to initialize" << std::endl;
        return -1;
    }

    initGame();
    initAliens();
    SDL_TimerID timerId = SDL_AddTimer(5000, aliensTimer, nullptr);
    

    while (!quit) {
        startTime = SDL_GetTicks64();
        processEvents();
        drawFrame();
        movement(elapseTime);
        events();
        endTime = SDL_GetTicks64();
        elapseTime = static_cast<Uint64>(endTime - startTime);
    }

    cleanup();
    return 0;
}
