# Building and uploading binaries to the mini is kinda tedious, so this helps facilitate local development
# assumes an ubuntu desktop with the SDL 1.2 dev packages installed
# i.e., `apt install libsdl1.2-dev libsdl-image1.2 libsdl-mixer1.2 libsdl-ttf2.0`
# then run `TARGET=ubuntu make` or `TARGET=miyoo make` as needed

MIYOO_CXX := arm-linux-gnueabihf-g++
MIYOO_PREFIX := /opt/miyoomini-toolchain/arm-linux-gnueabihf/libc
MIYOO_CXXFLAGS := -I$(MIYOO_PREFIX)/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT
MIYOO_LDFLAGS := -L$(MIYOO_PREFIX)/usr/lib -lSDL -lSDL_image -lSDL_ttf -lSDL_mixer -lpthread
MIYOO_TARGET_EXEC := app.miyoo.bin

UBUNTU_CXX := g++
UBUNTU_PREFIX := /
UBUNTU_CXXFLAGS := -I$(UBUNTU_PREFIX)/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT
UBUNTU_LDFLAGS := -L$(UBUNTU_PREFIX)/usr/lib -lSDL -lSDL_image -lSDL_ttf -lSDL_mixer -lpthread
UBUNTU_TARGET_EXEC := app.ubuntu.bin

ifeq ($(TARGET),miyoo)
	CXX=$(MIYOO_CXX)
	CXXFLAGS=$(MIYOO_CXXFLAGS)
	LDFLAGS=$(MIYOO_LDFLAGS)
	TARGET_EXEC=$(MIYOO_TARGET_EXEC)
else
	CXX=$(UBUNTU_CXX)
	CXXFLAGS=$(UBUNTU_CXXFLAGS)
	LDFLAGS=$(UBUNTU_LDFLAGS)
	TARGET_EXEC=$(UBUNTU_TARGET_EXEC)
endif

BUILD_DIR := ./build/$(TARGET)
SRC_DIRS := ./src
BIN_DIR := ./bin
ASSETS_DIR := ./assets

SRCS := $(shell find $(SRC_DIRS) -name '*.cpp')
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

bin/$(TARGET_EXEC): $(OBJS)
	mkdir -p $(dir $@)
	$(CXX) $(OBJS) -o bin/$(TARGET_EXEC) $(LDFLAGS)
	cp -r $(ASSETS_DIR) bin

$(BUILD_DIR)/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -r $(BUILD_DIR)/*
	rm $(BIN_DIR)/*
