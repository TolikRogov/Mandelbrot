#include "Mandelbrot.hpp"

int main(int argc, char* argv[]) {
	clock_t start_t = clock();

	MandelbrotStatusCode mandelbrot_status = MANDELBROT_NO_ERROR;
	SFML sfml = {};

	if (argc < 3)
		MANDELBROT_ERROR_CHECK(MANDELBROT_CMD_LINE_KEY_AMOUNT_ERROR);

	sfml.pixels = new sf::Uint8[PIXELS_SIZE];
	sfml.runs = (unsigned int)atoi(argv[2]);
	printf("%u\n", sfml.runs);

	if (!strcmp(argv[1], KEY_GRAPHICS)) {
		sfml.mode = MODE_GRAPHICS;
		sfml.window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Mandelbrot!");

		if (!sfml.texture.create(WINDOW_WIDTH, WINDOW_HEIGHT))
			MANDELBROT_ERROR_CHECK(MANDELBROT_SFML_CREATE_ERROR);

		sfml.sprite.setTexture(sfml.texture);

		mandelbrot_status = RunLab(&sfml);
		MANDELBROT_ERROR_CHECK(mandelbrot_status);
	}
	else if (!strcmp(argv[1], KEY_TEST)) {
		sfml.mode = MODE_TEST;
		mandelbrot_status = RunLab(&sfml);
		MANDELBROT_ERROR_CHECK(mandelbrot_status);
	}
	else
		MANDELBROT_ERROR_CHECK(MANDELBROT_CMD_LINE_KEY_ERROR);

	printf("PROGRAM TIME: %lg sec\n", (double)(clock() - start_t) / CLOCKS_PER_SEC);

	return 0;
}
