#include "Mandelbrot.hpp"

int main() {
	MandelbrotStatusCode mandelbrot_status = MANDELBROT_NO_ERROR;

	SFML sfml = {};

	sfml.window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Mandelbrot!");
	sfml.pixels = new sf::Uint8[PIXELS_SIZE];

	if (!sfml.texture.create(WINDOW_WIDTH, WINDOW_HEIGHT))
		MANDELBROT_ERROR_CHECK(MANDELBROT_SFML_CREATE_ERROR);

	sfml.sprite.setTexture(sfml.texture);

	mandelbrot_status = RunSFML(&sfml);
	MANDELBROT_ERROR_CHECK(mandelbrot_status);

	return 0;
}
