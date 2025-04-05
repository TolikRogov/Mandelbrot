#include "Mandelbrot.hpp"

int main(int argc, char* argv[]) {
	clock_t start_t = clock();

	if (argc < 4)
		MANDELBROT_ERROR_CHECK(MANDELBROT_CMD_LINE_KEY_AMOUNT_ERROR);

	MandelbrotStatusCode mandelbrot_status = MANDELBROT_NO_ERROR;
	SFML sfml 	= {.pixels = new sf::Uint8[PIXELS_SIZE]};
	LabWork lab = {.sfml = &sfml};

	lab.runs = (unsigned int)atoi(argv[3]);
	if (!strcmp(argv[1], KEY_FUNC_BASE))
		lab.func = &BaseVersionMandelbrot;
	else if (!strcmp(argv[1], KEY_FUNC_ARRAY))
		lab.func = &ArrayVersionMandelbrot;
	else if (!strcmp(argv[1], KEY_FUNC_FUNCTION))
		lab.func = &FunctionVersionMandelbrot;
	else if (!strcmp(argv[1], KEY_FUNC_INTRINSICS))
		lab.func = &IntrinsicsVersionMandelbrot;
	else
		MANDELBROT_ERROR_CHECK(MANDELBROT_CMD_LINE_KEY_FUNC_ERROR);

	if (!strcmp(argv[2], KEY_MODE_GRAPHICS)) {
		lab.mode = MODE_GRAPHICS;
		sfml.window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Mandelbrot!");

		if (!sfml.texture.create(WINDOW_WIDTH, WINDOW_HEIGHT))
			MANDELBROT_ERROR_CHECK(MANDELBROT_SFML_CREATE_ERROR);

		sfml.sprite.setTexture(sfml.texture);

		mandelbrot_status = RunLab(&lab);
		MANDELBROT_ERROR_CHECK(mandelbrot_status);
	}
	else if (!strcmp(argv[2], KEY_MODE_TEST)) {
		lab.mode = MODE_TEST;
		mandelbrot_status = RunLab(&lab);
		MANDELBROT_ERROR_CHECK(mandelbrot_status);
	}
	else
		MANDELBROT_ERROR_CHECK(MANDELBROT_CMD_LINE_KEY_ERROR);

	printf("PROGRAM TIME: %lg sec\n", (double)(clock() - start_t) / CLOCKS_PER_SEC);

	return 0;
}
