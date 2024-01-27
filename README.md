# Hello ~~Miyoo~~ WebAssembly
A silly little demo for the browser
--------------------------------------
- Uses raylib (both SDL versions are very buggy in the browser)
- Wasm bits handled by Emscripten

## Building for the web
- Install emscripten (`brew install emscripten`)
- Download the wasm build of raylib into the dep directory:
```sh
curl -sL https://github.com/raysan5/raylib/releases/download/5.0/raylib-5.0_webassembly.zip \
| bsdtar -x -C ./dep
```
- Run `TARGET=wasm make` to build the project
- Serve the build `npx http-server ./bin`

## Building for desktop
- Install raylib from your package manager (`brew install raylib`)
- Run `TARGET=desktop make`


Assets
------
All files in the assets directory are CC0 or Public Domain:

- sfx/hop.wav: https://opengameart.org/content/8-bit-sound-effect-pack
- bgm/Mars.wav: https://opengameart.org/content/nes-shooter-music-5-tracks-3-jingles
- img/battleback8.png: https://opengameart.org/content/backgrounds-3
- font/MMXSNES.ttf: https://nimblebeastscollective.itch.io/nb-pixel-font-bundle

