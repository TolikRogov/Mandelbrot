#include "Mandelbrot.hpp"

MandelbrotStatusCode RunSFML(SFML* sfml) {

	while (sfml->window.isOpen())
	{
		sf::Event event;
		while (sfml->window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				sfml->window.close();
		}

		for (unsigned int y_index = 0; y_index < WINDOW_HEIGHT; y_index++) {
			mandel_t x_0 = -2.0f;
			mandel_t y_0 = (mandel_t)(1.5f - ((mandel_t)y_index * 3.0f / (mandel_t)WINDOW_HEIGHT));
			for (unsigned int x_index = 0; x_index < WINDOW_WIDTH; x_index++, x_0 += 3.0f / (mandel_t)WINDOW_WIDTH) {
				mandel_t X = x_0,
						 Y = y_0;
				int n = 0;
				for (; n < MAXIMUM_ITERATIONS; n++) {
					mandel_t x2 = X * X,
							 y2 = Y * Y,
							 xy = X * Y;
					mandel_t r2 = x2 + y2;
					if (r2 >= MAX_RADIUS) break;
					X = x2 - y2 + x_0;
					Y = xy + xy + y_0;
				}
				if (n >= MAXIMUM_ITERATIONS)
					continue;
				n *= 8;
				sfml->pixels[4 * (x_index + y_index * WINDOW_WIDTH)] 		= (sf::Uint8)(n);
				sfml->pixels[4 * (x_index + y_index * WINDOW_WIDTH) + 1] 	= (sf::Uint8)(n);
				sfml->pixels[4 * (x_index + y_index * WINDOW_WIDTH) + 2] 	= (sf::Uint8)(128 - n);
				sfml->pixels[4 * (x_index + y_index * WINDOW_WIDTH) + 3] 	= 255;
			}
		}
		sfml->texture.update(sfml->pixels);

		sfml->window.clear();
		sfml->window.draw(sfml->sprite);
		sfml->window.display();
	}

	return MANDELBROT_NO_ERROR;
}

const wchar_t* MandelbrotErrorsMessenger(MandelbrotStatusCode status) {
	switch (status) {
		case MANDELBROT_NO_ERROR:							return L"MANDELBROT ERROR - NO ERROR";
		case MANDELBROT_SFML_CREATE_ERROR:					return L"MANDELBROT ERROR - SFML DOESN'T CREATE CLASS";
		case MANDELBROT_ALLOCATION_ERROR:					return L"MANDELBROT ERROR - MEMORY WAS ALLOCATED WITH ERROR";
		case MANDELBROT_NULL_POINTER:						return L"MANDELBROT ERROR - NULL POINTER";
		default: 											return L"UNDEFINED ERROR";
	}
}

