FLAGS = -std=c++20 -Wall

FRAMEWORKS = -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL

LIBS = -I/opt/homebrew/include -L/opt/homebrew/lib

INCLUDES = -I./include -I./utils -I./src -I./lib/tmxlite/include -lsdl2 -lsdl2_image

FILES = src/main.cpp src/**/*.cpp

all:
	g++ -o build/app $(FILES) $(LIBS) $(INCLUDES) $(FRAMEWORKS) $(FLAGS)
	./build/app

clean:
	rm -rf build/app