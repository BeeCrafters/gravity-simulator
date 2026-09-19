CC = gcc

SOURCE_DIR = src
BUILD_DIR = build

SOURCES = $(wildcard $(SOURCE_DIR)/*.c)
OBJECTS = $(patsubst $(SOURCE_DIR)/%.c, $(BUILD_DIR)/%.o, $(SOURCES))

# TODO: Come up with a better executable name
TARGET = g-sim

# This part detects the glfw installation type
GLFW_INSTALL_TYPE = $(shell cat vendor/glfw/install-type)
ifeq ($(GLFW_INSTALL_TYPE),local)

LDFLAGS = -Lvendor/glfw/lib -glfw3 -lm
INCLUDES = -Iinclude -Ivendor/glad/include -Ivendor/glfw/include
TARGET = $(TARGET).exe

else

LDFLAGS= $(shell pkg-config glfw3 --libs) -lGL -ldl -lm
INCLUDES = -Iinclude -Ivendor/glad/include

endif



CFLAGS = -std=c23 -O2 $(INCLUDES)

all: $(TARGET)

debug: $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -g -c $^ -o $(TARGET)-debug

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.c vendor/glad
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

vendor/glad:
	mkdir vendor/glad
	pip install glad
	glad --profile core --api gl=3.3 --generator c --out-path vendor/glad

clean:
	rm -rf $(BUILD_DIR) $(TARGET) $(TARGET)-debug

# This is mainly for Windows/Mac users who don't have glfw packaged in their OSs.
# Such users should run this target before anything else.
local-glfw:
	echo local > vendor/glfw/install-type
	echo "You're on your own about installing glfw in vendor/glfw ."
	echo "The Makefile expects dynamic libraries be at vendor/glfw/lib ."
	echo "The includes should be at vendor/glfw/include ."
	echo "Do not delete vendor/glfw/install-type !"
