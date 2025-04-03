#pragma once

#include "Mandelbrot_utilities.hpp"

typedef double mandel_t;

const unsigned int WINDOW_WIDTH 	= 800;
const unsigned int WINDOW_HEIGHT 	= 800;
const unsigned int PIXELS_SIZE 		= WINDOW_HEIGHT * WINDOW_WIDTH * 4;

const mandel_t WIDTH_PER_SEGMENTS	= 3.0f;
const mandel_t HEIGHT_PER_SEGMENTS 	= 3.0f;
const mandel_t SHIFT_SENSITIVITY	= 35.0f;
const mandel_t MAX_RADIUS 			= 100.0f;

const int MAXIMUM_ITERATIONS 		= 256;
const int COLOR_SENSITIVITY			= 8;

const double EPS 					= 1e-6;

struct Mandel_struct {
	mandel_t horizontal;
	mandel_t vertical;
	mandel_t scale;
	mandel_t dx;
	mandel_t dy;
	unsigned int frames_count;
	double fps;
	time_t start_time;
};

struct SFML {
	sf::RenderWindow window;
	sf::Uint8* pixels;
	sf::Texture texture;
	sf::Sprite sprite;
};

MandelbrotStatusCode RunSFML(SFML* sfml);
MandelbrotStatusCode KeyHandler(sf::Event* event, Mandel_struct* mnd);
MandelbrotStatusCode RunMandelbrot(SFML* sfml, Mandel_struct* mnd);
MandelbrotStatusCode CalcFPS(SFML* sfml, Mandel_struct* mnd);
