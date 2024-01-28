# assumes a desktop with the raylib library installed and the emscripten sdk
# i.e., `brew install raylib` for desktop
# for web, `brew install emscripten` and then download the wasm build of raylib into dep:
# curl -sL https://github.com/raysan5/raylib/releases/download/5.0/raylib-5.0_webassembly.zip | bsdtar -x -C ./dep
# then run `TARGET=desktop make` or `TARGET=wasm make` as needed

DESKTOP_CXX := clang++
DESKTOP_PREFIX := /opt/homebrew
DESKTOP_CXXFLAGS := -I $(DESKTOP_PREFIX)/include -std=c++20
DESKTOP_LDFLAGS := -L $(DESKTOP_PREFIX)/lib -lraylib -lm
DESKTOP_TARGET_EXEC := app.desktop.bin

WASM_CXX := emcc
WASM_PREFIX := ./dep
WASM_RAYLIB_PATH := $(WASM_PREFIX)/raylib-5.0_webassembly
WASM_CXXFLAGS := -I $(WASM_RAYLIB_PATH)/include
WASM_LDFLAGS := -L $(WASM_RAYLIB_PATH)/lib \
	-lraylib \
	-lm \
	-s USE_GLFW=3 \
	-s GL_ENABLE_GET_PROC_ADDRESS \
	-s WASM=1 \
	-s MODULARIZE=1 \
	-s EXPORT_ES6=1 \
	--embed-file ./assets
WASM_TARGET_EXEC := index.js

ifeq ($(TARGET),wasm)
	CXX=$(WASM_CXX)
	CXXFLAGS=$(WASM_CXXFLAGS)
	LDFLAGS=$(WASM_LDFLAGS)
	TARGET_EXEC=$(WASM_TARGET_EXEC)
else
	CXX=$(DESKTOP_CXX)
	CXXFLAGS=$(DESKTOP_CXXFLAGS)
	LDFLAGS=$(DESKTOP_LDFLAGS)
	TARGET_EXEC=$(DESKTOP_TARGET_EXEC)
endif

BUILD_DIR := ./build/$(TARGET)
SRC_DIRS := ./src
BIN_DIR := ./bin
ASSETS_DIR := ./assets

SRCS := $(shell find $(SRC_DIRS) -name '*.cpp')
WEB_SRCS := $(shell find $(SRC_DIRS) -name '*.html')
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

ifeq ($(TARGET),wasm)
bin/$(TARGET_EXEC): $(OBJS) web
	mkdir -p $(dir $@)
	$(CXX) $(OBJS) -o bin/$(TARGET_EXEC) $(LDFLAGS)

web: $(WEB_SRCS)
	cp $(WEB_SRCS) bin
else
bin/$(TARGET_EXEC): $(OBJS)
	mkdir -p $(dir $@)
	$(CXX) $(OBJS) -o bin/$(TARGET_EXEC) $(LDFLAGS)
	cp -r $(ASSETS_DIR) bin
endif

$(BUILD_DIR)/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)/*
	rm -rf $(BIN_DIR)/*
