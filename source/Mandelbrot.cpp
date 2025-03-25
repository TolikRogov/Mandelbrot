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

		for (size_t i = 0; i < PIXELS_SIZE; i++)
			sfml->pixels[i] = (sf::Uint8)(rand() % 255);
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

