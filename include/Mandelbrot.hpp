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

struct SFML {
	sf::RenderWindow window;
	sf::Uint8* pixels;
	sf::Texture texture;
	sf::Sprite sprite;
};

MandelbrotStatusCode RunSFML(SFML* sfml);
void CalculateShift(sf::Event* event, mandel_t* horizontal_shift, mandel_t* vertical_shift, mandel_t* scale);
