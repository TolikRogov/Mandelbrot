#pragma once

#include "Mandelbrot_utilities.hpp"

const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 800;
const unsigned int PIXELS_SIZE = WINDOW_HEIGHT * WINDOW_WIDTH * 4;

struct SFML {
	sf::RenderWindow window;
	sf::Uint8* pixels;
	sf::Texture texture;
	sf::Sprite sprite;
};

MandelbrotStatusCode RunSFML(SFML* sfml);
