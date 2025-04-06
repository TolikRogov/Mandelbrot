#pragma once

#include "Mandelbrot_utilities.hpp"

typedef double mandel_t;

#define _DIR_DATA 				"data/"
#define _DIR_FONTS 				_DIR_DATA "fonts/"
#define _DIR_SHARE_TECH_CYR 	_DIR_FONTS "Share Tech CYR/"
#define _FONT_SHARE_TECH_CYR	_DIR_SHARE_TECH_CYR "Share-Tech-CYR.otf"

#define _TEXT_FPS_POSITION sf::Vector2((float)sfml.text.getCharacterSize() / 3.0f, 0.0f)

const char KEY_MODE_GRAPHICS[] 		= "--graphics";
const char KEY_MODE_TEST[]			= "--test";

const char KEY_FUNC_BASE[]			= "--base";
const char KEY_FUNC_ARRAY[]			= "--array";
const char KEY_FUNC_FUNCTION[]		= "--function";
const char KEY_FUNC_INTRINSICS[]	= "--intrinsics";

const unsigned int WINDOW_WIDTH 	= 800;
const unsigned int WINDOW_HEIGHT 	= 800;
const unsigned int PIXELS_SIZE 		= WINDOW_HEIGHT * WINDOW_WIDTH * 4;
const unsigned int SIZE				= 4;

const mandel_t WIDTH_PER_SEGMENTS	= 3.0;
const mandel_t X_CENTER_IN_SEGMENTS = 2.0;
const mandel_t HEIGHT_PER_SEGMENTS 	= 3.0;
const mandel_t PART_OF_Y_CENTER_PXL = 2.0;
const mandel_t SHIFT_SENSITIVITY	= 35.0;
const mandel_t MAX_RADIUS 			= 100.0;
const mandel_t STANDARD_SCALE 		= 1.0;
const mandel_t STANDARD_TIME_DELAY	= 1.0;
const mandel_t SCALE_COEFFICIENT	= 1.5;

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
	unsigned int runs;
	double fps;
	clock_t start_time;
};

struct SFML {
	sf::RenderWindow window;
	sf::Uint8* pixels;
	sf::Texture texture;
	sf::Sprite sprite;
	sf::Font font;
	sf::Text text;
};

typedef MandelbrotStatusCode (*mandel_func_t)(SFML* sfml, Mandel_struct* mnd);

struct LabWork {
	RunningMode mode;
	unsigned int runs;
	mandel_func_t func;
	SFML* sfml;
};

MandelbrotStatusCode RunLab(LabWork* lab);
MandelbrotStatusCode KeyHandler(sf::Event* event, Mandel_struct* mnd);
MandelbrotStatusCode BaseVersionMandelbrot(SFML* sfml, Mandel_struct* mnd);
MandelbrotStatusCode ArrayVersionMandelbrot(SFML* sfml, Mandel_struct* mnd);
MandelbrotStatusCode FunctionVersionMandelbrot(SFML* sfml, Mandel_struct* mnd);
MandelbrotStatusCode IntrinsicsVersionMandelbrot(SFML* sfml, Mandel_struct* mnd);
MandelbrotStatusCode CalcFPS(SFML* sfml, Mandel_struct* mnd);
