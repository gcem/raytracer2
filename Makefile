all:
	g++ -g *.cpp */*.cpp -Ilibraries -Isurfaces -I. -o raytracer -std=c++11
