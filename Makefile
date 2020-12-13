all:
	g++ -O3 -march=native *.cpp */*.cpp -Ilibraries -Isurfaces -Itexture -Itransformations -Ialgebra -I. -lpthread -ljpeg -o raytracer -std=c++17

debug:
	g++ -g -march=native *.cpp */*.cpp -Ilibraries -Isurfaces -Itexture -Itransformations -Ialgebra -I. -lpthread -ljpeg -o raytracer -std=c++17