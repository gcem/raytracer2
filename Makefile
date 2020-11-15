all:
	g++ -O3 *.cpp */*.cpp -Ilibraries -Isurfaces -I. -lpthread -o raytracer -std=c++11
	
debug:
	g++ -g *.cpp */*.cpp -Ilibraries -Isurfaces -I. -lpthread -o raytracer -std=c++11