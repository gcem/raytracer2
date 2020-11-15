all:
	g++ -O3 -march=native *.cpp */*.cpp -Ilibraries -Isurfaces -I. -lpthread -o raytracer -std=c++11

debug:
	g++ -O3 -march=native *.cpp */*.cpp -Ilibraries -Isurfaces -I. -lpthread -o raytracer -std=c++11