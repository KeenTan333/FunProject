#include <SDL.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768
#define GRAVITY 9.81f
#define MAX_THRUST 15.0f
#define FPS 60
#define ROCKET_HEIGHT 120.0f
#define ROCKET_WIDTH 20.0f
#define GROUND_HEIGHT 100.0f

typedef struct {
    float x, y;
    float vx, vy;
    float throttle;
    float gimbal; // radians
} Rocket;

void handle_input(Rocket *rocket, bool *running) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT)
            *running = false;
        if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
                case SDLK_q: *running = false; break;
                case SDLK_w: rocket->throttle = 1.0f; break;
                case SDLK_a: rocket->gimbal = -1.0f; break;
                case SDLK_d: rocket->gimbal = +1.0f; break;
            }
        } else if (e.type == SDL_KEYUP) {
            switch (e.key.keysym.sym) {
                case SDLK_w: rocket->throttle = 0.0f; break;
                case SDLK_a: rocket->gimbal = 0.0f; break;
                case SDLK_d: rocket->gimbal = 0.0f; break;
            }
        }
    }
}

void update_physics(Rocket *rocket) {

    ////  Simple controller for throttle ////
    // Comment out this section to disable the controller
    // Lands rocket according to reference trajectory
    double ref = (0.15f * (SCREEN_HEIGHT - GROUND_HEIGHT - (ROCKET_HEIGHT/2) - rocket->y)) + 0.5;
    double err = rocket->vy - ref;

    rocket->throttle = atan(10 * err);

    // Saturate throttle
    if (rocket->throttle < 0) rocket->throttle = 0;
    if (rocket->throttle > 1.0f) rocket->throttle = 1.0f;
    //// Controller ends here ////

    float ax = MAX_THRUST * rocket->throttle * sinf(rocket->gimbal);
    float ay = -MAX_THRUST * rocket->throttle * cosf(rocket->gimbal) + GRAVITY;

    float dT = 1.0f / FPS;
    rocket->vx += ax * dT;
    rocket->vy += ay * dT;

    rocket->x += rocket->vx * dT;
    rocket->y += rocket->vy * dT;

    if (rocket->y > SCREEN_HEIGHT - GROUND_HEIGHT - (ROCKET_HEIGHT/2)) {
        if (fabs(rocket->vy) > 1.0f) {
            printf("💥 CRASHED! vy=%.2f\n", rocket->vy);
            exit(0);
        } else {
            printf("🎉 LANDED SAFELY! vy=%.2f\n", rocket->vy);
            exit(0);
        }
    }
}

void draw(SDL_Renderer *renderer, Rocket *rocket) {
    SDL_SetRenderDrawColor(renderer, 20, 20, 30, 255);
    SDL_RenderClear(renderer);

    // Draw ground
    SDL_SetRenderDrawColor(renderer, GROUND_HEIGHT, 180, GROUND_HEIGHT, 255);
    SDL_Rect ground = {0, SCREEN_HEIGHT - GROUND_HEIGHT, SCREEN_WIDTH, GROUND_HEIGHT};
    SDL_RenderFillRect(renderer, &ground);

    // Rocket corners before rotation (relative to center)
    float hw = ROCKET_WIDTH / 2;
    float hh = ROCKET_HEIGHT / 2;

    float c = cosf(rocket->gimbal);
    float s = sinf(rocket->gimbal);

    SDL_FPoint points[5];
    points[0].x = rocket->x + (-hw) * c - (-hh) * s;
    points[0].y = rocket->y + (-hw) * s + (-hh) * c;

    points[1].x = rocket->x + ( hw) * c - (-hh) * s;
    points[1].y = rocket->y + ( hw) * s + (-hh) * c;

    points[2].x = rocket->x + ( hw) * c - ( hh) * s;
    points[2].y = rocket->y + ( hw) * s + ( hh) * c;

    points[3].x = rocket->x + (-hw) * c - ( hh) * s;
    points[3].y = rocket->y + (-hw) * s + ( hh) * c;

    points[4] = points[0]; // close the loop

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawLinesF(renderer, points, 5);

    // Draw thrust direction
    float fx = rocket->x + 50 * sinf(rocket->gimbal);
    float fy = rocket->y + 100 * rocket->throttle * cosf(rocket->gimbal) + (ROCKET_HEIGHT/2);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderDrawLineF(renderer, rocket->x, rocket->y + (ROCKET_HEIGHT/2), fx, fy);

    SDL_RenderPresent(renderer);
}

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("2D Rocket Game",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    Rocket rocket = {
        .x = SCREEN_WIDTH / 2.0f,
        .y = 100,
        .vx = 0,
        .vy = 0,
        .throttle = 0,
        .gimbal = 0
    };

    Uint32 frame_delay = 1000 / FPS;
    bool running = true;

    while (running) {
        Uint32 start_time = SDL_GetTicks();

        handle_input(&rocket, &running);
        update_physics(&rocket);
        draw(renderer, &rocket);

        printf("x=%.1f y=%.1f | vx=%.2f vy=%.2f | throttle=%.3f%% | gimbal=%.2f°\r",
               rocket.x, rocket.y,
               rocket.vx, rocket.vy,
               rocket.throttle * 100.0f,
               rocket.gimbal * 180.0f / M_PI);
        fflush(stdout);

        Uint32 elapsed = SDL_GetTicks() - start_time;
        if (elapsed < frame_delay)
            SDL_Delay(frame_delay - elapsed);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
