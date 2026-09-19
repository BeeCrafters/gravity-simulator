CC = gcc

SOURCE_DIR = src
BUILD_DIR = build

SOURCES = $(wildcard $(SOURCE_DIR)/*.c)
OBJECTS = $(patsubst $(SOURCE_DIR)/%.c, $(BUILD_DIR)/%.o, $(SOURCES))

# TODO: Come up with a better executable name
TARGET = g-sim

INCLUDES += -Iinclude -Ivendor/glad/include -Ivendor/cgltf/include

# This part detects the glfw installation type
GLFW_INSTALL_TYPE = $(shell cat vendor/glfw/install-type 2>/dev/null)
ifeq ($(GLFW_INSTALL_TYPE),local)

LDFLAGS += -Lvendor/glfw/lib -glfw3 -lm
INCLUDES += -Ivendor/glfw/include
TARGET := $(TARGET).exe

else

LDFLAGS += $(shell pkg-config glfw3 --libs) -lGL -ldl -lm

endif



CFLAGS = -std=c23 -O2 $(INCLUDES)

all: $(TARGET)

debug: $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -D__DEBUG__ -g $^ -o $(TARGET)-debug

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.c vendor/glfw/install-type vendor/cgltf/include/cgltf.h
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@


# GLFW installation type selection. This is necessary so we can have support for users / team members with local installations.
vendor/glfw/install-type:
	@printf "\nYou have not selected your GLFW installation type yet.\nThis selection is so that users who want to use local GLFW installations can specify so.\nFor a better explanation, refer to the README.\n\tSelect your installation type. [ (system/local) or (s/l) ]: "
	@read installtype; mkdir -p vendor/glfw; if [ "$$(echo $$installtype)" == "system" ] || [ "$$(echo $$installtype)" == "s" ]; then echo system > vendor/glfw/install-type; elif [ "$$(echo $$installtype)" == "local" ] || [ "$$(echo $$installtype)" == "l" ]; then echo local > vendor/glfw/install-type; printf "\nLittle disclaimer about local installations!\n\tYou're on your own about installing glfw in vendor/glfw.\n\tThe Makefile expects dynamic libraries be at vendor/glfw/lib, and the include folder should be at vendor/glfw/include.\n"; fi
	@printf "GLFW install type selected.\n\n"
	cat vendor/glfw/install-type

vendor/cgltf/include/cgltf.h:
	@mkdir -p vendor/cgltf/include
	curl https://raw.githubusercontent.com/jkuhlmann/cgltf/refs/heads/master/cgltf.h > $@
	@echo "cgltf.h installed."

clean:
	@rm -rf $(BUILD_DIR) $(TARGET) $(TARGET)-debug
