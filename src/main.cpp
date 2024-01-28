#include "raylib.h"
#include "raymath.h"
#include <string>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#ifdef __EMSCRIPTEN__
extern "C" {
    EMSCRIPTEN_KEEPALIVE void pause() { emscripten_pause_main_loop(); }
    EMSCRIPTEN_KEEPALIVE void unpause() { emscripten_resume_main_loop(); }
    EMSCRIPTEN_KEEPALIVE void shutdown() { emscripten_cancel_main_loop(); }
}
#endif

// resources
Sound boop;
Music bgm;
Texture2D bg;
Font font;
std::string hello = "Hello World";
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int FONT_SIZE = 64;

// forward declarations
void init();
void deinit();
void loop();

int main(void) {
    const int TARGET_FPS = 60;

    init();

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(loop, TARGET_FPS, 1);
#else
    SetTargetFPS(TARGET_FPS);
    while (!WindowShouldClose()) {
        loop();
    }
#endif

    deinit();

    return 0;
}

void init() {
    const char *title;
    #ifdef __EMSCRIPTEN__
        title = emscripten_get_window_title();
    #else
        title = hello.c_str();
    #endif
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, title);
    InitAudioDevice();

    boop = LoadSound("./assets/sfx/hop.wav");
    bgm = LoadMusicStream("./assets/bgm/Mars.wav");
    bg = LoadTexture("./assets/img/battleback8.png");
    font = LoadFontEx("./assets/font/MMXSNES.ttf", 96, 0, 0);
    SetMusicVolume(bgm, 0.15);
    PlayMusicStream(bgm);
}

void deinit() {
    UnloadFont(font);
    UnloadTexture(bg);
    UnloadMusicStream(bgm);
    UnloadSound(boop);

    CloseAudioDevice();
    CloseWindow();
}

void loop() {
    UpdateMusicStream(bgm);

    // update
    if (IsKeyPressed(KEY_Z)) {
        PlaySound(boop);
    }

    // draw
    BeginDrawing();
    ClearBackground(BLACK);

    // draw the background image, scaling it to the destination viewport
    const Rectangle src = {
        .x = 0,
        .y = 0,
        .width = (float)bg.width,
        .height = (float)bg.height
    };
    const Rectangle dest = {
        .x = 0,
        .y = 0,
        .width = SCREEN_WIDTH,
        .height = SCREEN_HEIGHT
    };
    DrawTexturePro(bg, src, dest, Vector2Zero(), 0, WHITE);

    // draws a white background box for our text
    Vector2 textLength = MeasureTextEx(font, hello.c_str(), FONT_SIZE, 0);
    Vector2 textPosition = {
        .x = SCREEN_WIDTH / 2 - textLength.x / 2,
        .y = SCREEN_HEIGHT / 2 - textLength.y / 2
    };
    DrawRectangleV(
        (Vector2){textPosition.x, textPosition.y + 10},
        (Vector2){textLength.x, textLength.y - 10},
        WHITE
    );

    DrawTextEx(font, hello.c_str(), textPosition, FONT_SIZE, 0.0f, BLACK);

    EndDrawing();
}
