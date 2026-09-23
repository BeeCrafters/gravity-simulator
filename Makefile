CC = gcc

SOURCE_DIR = src
BUILD_DIR = build

SOURCES = $(wildcard $(SOURCE_DIR)/*.c)
OBJECTS = $(patsubst $(SOURCE_DIR)/%.c, $(BUILD_DIR)/%.o, $(SOURCES))

# TODO: Come up with a better executable name
TARGET = g-sim

INCLUDES += -Iinclude -Ivendor/glad/include -Ivendor/cgltf/include

# This part detects if the pkg-config binary and the glfw3 package exist or not
GLFW_SYSTEM_EXISTS = $(shell pkg-config --exists glfw3 2>/dev/null && echo yes || echo no)
ifeq ($(GLFW_SYSTEM_EXISTS),no)

$(shell echo -$$'\n\nYou are missing pkg-config and/or a glfw package. Here are some useful instructions:\n\nFor posix compliant OS users:\n\tInstall the glfw development package from your package manager.\n\tIf there is no fitting package, use a local installation.\n\nFor MSYS2 users:\n\tRun "sudo pacman -S mingw-w64-{env type}-glfw".\n\tReplace {env type} with "ucrt-x86_64", "x86_64" or "clang-x86_64".\n\nOtherwise, simply install glfw locally into vendor/glfw:\n\tThe Makefile expects dynamic libraries be at vendor/glfw/lib, and the include folder should be at vendor/glfw/include.\n\n\nThe build will continue with the assumption of a local installation.\n')

LDFLAGS += -Lvendor/glfw/lib -lglfw -lGL -ldl -lm
INCLUDES += -Ivendor/glfw/include
# TARGET := $(TARGET).exe

else

LDFLAGS += $(shell pkg-config glfw3 --libs) -lGL -ldl -lm

endif



CFLAGS = -std=c23 -O2 $(INCLUDES)

all: $(TARGET)

debug: $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -D__DEBUG__ -g $^ -o $(TARGET)-debug

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.c vendor/cgltf/include/cgltf.h
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@


vendor/cgltf/include/cgltf.h:
	@mkdir -p vendor/cgltf/include
	curl https://raw.githubusercontent.com/jkuhlmann/cgltf/refs/heads/master/cgltf.h > $@
	@echo "cgltf.h installed."

clean:
	@rm -rf $(BUILD_DIR) $(TARGET) $(TARGET)-debug
