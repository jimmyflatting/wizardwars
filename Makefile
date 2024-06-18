FRAMEWORKS = -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL
LIBS = -I/opt/homebrew/include -L/opt/homebrew/lib -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_gfx -lSDL2_net -lxml2
LOCALS = -I./include -I./utils
SRC = src/*c utils/*.c

compile:
	clang -o bin/client $(SRC) $(LOCALS) $(LIBS) $(FRAMEWORKS)
