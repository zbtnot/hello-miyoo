#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>

// Screen formatting
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int BITS_PER_PIXEL = 32;

// Font formatting
const int FONT_SIZE = 64;

const int AUDIO_CHUNK_SIZE = 512;

// Button Helpers
const SDLKey BTN_A = SDLK_SPACE;
const SDLKey BTN_MENU = SDLK_ESCAPE;

// Resource paths
const char *imagePath = "assets/img/battleback8.png";
const char *fontPath = "assets/font/MMXSNES.ttf";
const char *bgmPath = "assets/bgm/Mars.wav";
const char *sfxPath = "assets/sfx/hop.wav";

const SDL_Color COLOR_WHITE = {255, 255, 255};
const SDL_Color COLOR_BLACK = {0, 0, 0};

const int LOOP_MUSIC = 1;

int main(int argc, char **argv) {
    // init
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();
    Mix_Init(MIX_INIT_OGG);
    Mix_OpenAudio(
            MIX_DEFAULT_FREQUENCY,
            MIX_DEFAULT_FORMAT,
            MIX_DEFAULT_CHANNELS,
            AUDIO_CHUNK_SIZE
    );

    Mix_Chunk *sfx = Mix_LoadWAV(sfxPath);
    Mix_Chunk *bgm = Mix_LoadWAV(bgmPath);

    SDL_Surface *video = SDL_SetVideoMode(
            SCREEN_WIDTH,
            SCREEN_HEIGHT,
            BITS_PER_PIXEL,
            SDL_HWSURFACE | SDL_DOUBLEBUF
    );
    SDL_Surface *screen = SDL_CreateRGBSurface(
            SDL_HWSURFACE,
            SCREEN_WIDTH,
            SCREEN_HEIGHT,
            BITS_PER_PIXEL,
            0, 0, 0, 0
    );
    bool done = false;

    // load resources
    TTF_Font *font = TTF_OpenFont(fontPath, FONT_SIZE);
    SDL_Surface *text = TTF_RenderUTF8_Shaded(font, "Hello world", COLOR_BLACK, COLOR_WHITE);
    SDL_Surface *background = IMG_Load(imagePath);

    // play music
    int bgmChannel = Mix_PlayChannel(-1, bgm, -1);

    while (!done) {
        // poll for input
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                done = true;
                continue;
            }

            if (event.type == SDL_KEYUP) {
                switch (event.key.keysym.sym) {
                    case BTN_A:
                        Mix_PlayChannel(-1, sfx, 0);
                        break;
                    case BTN_MENU:
                        done = true;
                        continue;
                }
            }
        }

        // clear screen
        SDL_FillRect(screen, nullptr, SDL_MapRGB(screen->format, COLOR_WHITE.r, COLOR_WHITE.g, COLOR_WHITE.b));
        // draw image to screen
        SDL_BlitSurface(background, nullptr, screen, nullptr);

        // to center the text we need to know how wide the text is
        SDL_Rect textCentered = {
                (Sint16)((SCREEN_WIDTH - text->w) / 2),
                (Sint16)((SCREEN_HEIGHT - text->h) / 2),
                0,
                0
        };
        // TTF_RenderUTF8_Shaded puts a nice outlined box around our text, but it has too much height.
        // This clips it down to a nice size.
        SDL_Rect textClipped = {
                0,
                12,
                (Uint16)text->w,
                (Uint16)text->h
        };
        SDL_BlitSurface(text, &textClipped, screen, &textCentered);

        // draw screen to vram
        SDL_BlitSurface(screen, nullptr, video, nullptr);
        SDL_Flip(video);
    }
    Mix_HaltChannel(-1);

    Mix_FreeChunk(sfx);
    Mix_FreeChunk(bgm);

    TTF_CloseFont(font);

    SDL_FreeSurface(text);
    SDL_FreeSurface(background);
    SDL_FreeSurface(screen);
    SDL_FreeSurface(video);

    // shutdown
    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    return 0;
}