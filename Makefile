FRAMEWORKS = -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL
LIBS = -I/opt/homebrew/include -L/opt/homebrew/lib -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_gfx -lSDL2_net
LOCALS = -I./include
SERVER = src/server.c
CLIENT = src/client.c

compile:
	clang -o bin/server $(SERVER) $(LOCALS) $(LIBS) $(FRAMEWORKS)
	clang -o bin/client $(CLIENT) $(LOCALS) $(LIBS) $(FRAMEWORKS)