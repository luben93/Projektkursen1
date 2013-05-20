all:
	g++ Main.cpp Player.cpp -o Game -lGLU -lglut
steff: 
	g++ -lGL steff.cpp Player.cpp positions.c udp.c -o Stiff -lSDLmain -lSDL -lGLU -lglut -pthread

marek: 
	g++ Main.cpp Player.cpp -o Marek -lGL -lGLU -lglut -lpng -lz
mareklucas:
	g++ Main.cpp Player.cpp -o Marek -framework OpenGL -framework GLUT -lpng -lz
julia:
	g++ julia.cpp Player.cpp udp.c positions.c -lSDL -lGLU -lglut -o julia.out
lucas:
	g++ -framework Cocoa -framework OpenGL -framework GLUT -framework SDL -lSDLmain steff.cpp Player.cpp udp.c positions.c -o lucas.out
lucas2:
	g++ -framework OpenGL -framework GLUT main.cpp Player.cpp -o lucas2.out `sdl-config --cflags --libs`
lucas3:
	g++ -framework OpenGL -framework GLUT lucas.cpp Player.cpp -o lucas3.out

steff_ubuntu: 
	g++ steff.cpp Player.cpp positions.c udp.c -o Stiff -lSDLmain -lSDL -lGL -lGLU -lglut -pthread
steff_ubuntu_kopia:
	g++ steff_kopia.cpp Player.cpp positions.c udp.c -o Stiff_kopia -lSDLmain -lSDL -lGL -lGLU -lglut -pthread
