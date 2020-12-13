all:
	g++ -O3 -march=native *.cpp */*.cpp -Ilibraries -Isurfaces -Itexture -Itransformations -Ialgebra -I. -lpthread -o raytracer -std=c++11