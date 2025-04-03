#pragma once

#include "Mandelbrot_utilities.hpp"

typedef double mandel_t;

const char KEY_GRAPHICS[] 			= "--graphics";
const char KEY_TEST[]				= "--test";

const unsigned int WINDOW_WIDTH 	= 800;
const unsigned int WINDOW_HEIGHT 	= 800;
const unsigned int PIXELS_SIZE 		= WINDOW_HEIGHT * WINDOW_WIDTH * 4;

const mandel_t WIDTH_PER_SEGMENTS	= 3.0f;
const mandel_t X_CENTER_IN_SEGMENTS = 2.0f;
const mandel_t HEIGHT_PER_SEGMENTS 	= 3.0f;
const mandel_t PART_OF_Y_CENTER_PXL = 2.0f;
const mandel_t SHIFT_SENSITIVITY	= 35.0f;
const mandel_t MAX_RADIUS 			= 100.0f;
const mandel_t STANDARD_SCALE 		= 1.0f;
const mandel_t STANDARD_TIME_DELAY	= 1.0f;
const mandel_t SCALE_COEFFICIENT	= 1.5f;

const int MAXIMUM_ITERATIONS 		= 256;
const int COLOR_SENSITIVITY			= 8;
const int COLOR_COEFFICIENT 		= 20;

const double EPS 					= 1e-6;

enum RunningMode {MODE_GRAPHICS, MODE_TEST};

struct Mandel_struct {
	mandel_t horizontal;
	mandel_t vertical;
	mandel_t scale;
	mandel_t dx;
	mandel_t dy;
	unsigned int frames_count;
	double fps;
	clock_t start_time;
};

struct SFML {
	sf::RenderWindow window;
	sf::Uint8* pixels;
	sf::Texture texture;
	sf::Sprite sprite;
	RunningMode mode;
	unsigned int runs;
};

MandelbrotStatusCode RunLab(SFML* sfml);
MandelbrotStatusCode KeyHandler(sf::Event* event, Mandel_struct* mnd);
MandelbrotStatusCode BaseVersionMandelbrot(SFML* sfml, Mandel_struct* mnd);
MandelbrotStatusCode CalcFPS(SFML* sfml, Mandel_struct* mnd);
