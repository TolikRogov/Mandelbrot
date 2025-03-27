#include "Mandelbrot.hpp"

MandelbrotStatusCode RunSFML(SFML* sfml) {

	mandel_t horizontal_shift = 0,
			 vertical_shift = 0,
			 scale = 1.0f,
			 dx = WIDTH_PER_SEGMENTS / (mandel_t)WINDOW_WIDTH,
			 dy = HEIGHT_PER_SEGMENTS / (mandel_t)WINDOW_HEIGHT;

	while (sfml->window.isOpen())
	{
		sf::Event event;
		while (sfml->window.pollEvent(event))
		{
			switch (event.type) {
				case sf::Event::Closed: 	{ sfml->window.close(); break; }
				case sf::Event::KeyPressed: { CalculateShift(&event, &horizontal_shift, &vertical_shift, &scale); }
				default: break;
			}
		}

		for (unsigned int y_index = 0; y_index < WINDOW_HEIGHT; y_index++) {
			mandel_t x_0 = (-(mandel_t)WINDOW_WIDTH * 2.0f / WIDTH_PER_SEGMENTS) * dx * scale + horizontal_shift * dx;
			mandel_t y_0 = (((mandel_t)y_index - (mandel_t)WINDOW_HEIGHT / 2.0f)) * dy * scale + vertical_shift * dy;
			for (unsigned int x_index = 0; x_index < WINDOW_WIDTH; x_index++, x_0 += dx * scale) {
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
				n *= COLOR_SENSITIVITY;
				sfml->pixels[4 * (x_index + y_index * WINDOW_WIDTH)] 		= (sf::Uint8)(n % MAXIMUM_ITERATIONS);
				sfml->pixels[4 * (x_index + y_index * WINDOW_WIDTH) + 1] 	= (sf::Uint8)(n % MAXIMUM_ITERATIONS);
				sfml->pixels[4 * (x_index + y_index * WINDOW_WIDTH) + 2] 	= (sf::Uint8)((MAXIMUM_ITERATIONS * COLOR_SENSITIVITY - n) / 20);
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

void CalculateShift(sf::Event* event, mandel_t* horizontal_shift, mandel_t* vertical_shift, mandel_t* scale) {
	switch (event->key.scancode) {
		case sf::Keyboard::Scan::Left: 			{ *horizontal_shift	-= SHIFT_SENSITIVITY * *scale; 	break; }
		case sf::Keyboard::Scan::Right:			{ *horizontal_shift += SHIFT_SENSITIVITY * *scale; 	break; }
		case sf::Keyboard::Scan::Up: 			{ *vertical_shift 	-= SHIFT_SENSITIVITY * *scale;	break; }
		case sf::Keyboard::Scan::Down: 			{ *vertical_shift 	+= SHIFT_SENSITIVITY * *scale; 	break; }
		case sf::Keyboard::Scan::NumpadPlus: 	{ *scale /= 1.5f; break; }
		case sf::Keyboard::Scan::NumpadMinus: 	{ *scale *= 1.5f; break; }
		default: break;
	}
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

