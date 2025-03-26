#pragma once

#include "Mandelbrot_utilities.hpp"

typedef double mandel_t;

const unsigned int WINDOW_WIDTH 	= 800;
const unsigned int WINDOW_HEIGHT 	= 800;
const unsigned int PIXELS_SIZE 		= WINDOW_HEIGHT * WINDOW_WIDTH * 4;

const int MAXIMUM_ITERATIONS 		= 256;
const mandel_t MAX_RADIUS 			= 100;

struct SFML {
	sf::RenderWindow window;
	sf::Uint8* pixels;
	sf::Texture texture;
	sf::Sprite sprite;
};

MandelbrotStatusCode RunSFML(SFML* sfml);
