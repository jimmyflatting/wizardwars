FRAMEWORKS = -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL
LIBS = -I/opt/homebrew/include -L/opt/homebrew/lib -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_gfx -lSDL2_net -lxml2
LOCALS = -I./include -I./utils
SERVER = src/server.c
CLIENT = src/client.c src/client/*.c utils/*.c
TILEMAP = src/tilemap.c

compile:
	clang -o bin/server $(SERVER) $(LOCALS) $(LIBS) $(FRAMEWORKS)
	clang -o bin/client $(CLIENT) $(LOCALS) $(LIBS) $(FRAMEWORKS)
	clang -o bin/tilemap $(TILEMAP) $(LOCALS) $(LIBS) $(FRAMEWORKS)
