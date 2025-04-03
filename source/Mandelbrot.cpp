#include "Mandelbrot.hpp"

MandelbrotStatusCode RunSFML(SFML* sfml) {

	Mandel_struct mnd = {.scale	= 1.0f,
						 .dx 	= WIDTH_PER_SEGMENTS / (mandel_t)WINDOW_WIDTH,
			 			 .dy 	= HEIGHT_PER_SEGMENTS / (mandel_t)WINDOW_HEIGHT};

	while (sfml->window.isOpen())
	{
		sf::Event event;
		while (sfml->window.pollEvent(event))
		{
			switch (event.type) {
				case sf::Event::Closed: 	{ sfml->window.close(); break; }
				case sf::Event::KeyPressed: { KeyHandler(&event, &mnd); }
				default: break;
			}
		}

		RunMandelbrot(sfml, &mnd);
		sfml->texture.update(sfml->pixels);
		CalcFPS(sfml, &mnd);

		sfml->window.clear();
		sfml->window.draw(sfml->sprite);
		sfml->window.display();
	}

	return MANDELBROT_NO_ERROR;
}

MandelbrotStatusCode CalcFPS(SFML* sfml, Mandel_struct* mnd) {
	mnd->frames_count++;
	double dt = 0.0f;
	if (mnd->frames_count == 1) time(&(mnd->start_time));
	else if ((dt = difftime(time(NULL), mnd->start_time)) - 1.0f >= EPS) {
		mnd->fps = ((double)mnd->frames_count / dt);
		mnd->frames_count = 0;
		printf("FPS: %lg\n", mnd->fps);
	}
	return MANDELBROT_NO_ERROR;
}

MandelbrotStatusCode RunMandelbrot(SFML* sfml, Mandel_struct* mnd) {
	for (unsigned int y_index = 0; y_index < WINDOW_HEIGHT; y_index++) {
		mandel_t x_0 = (mnd->horizontal + (-(mandel_t)WINDOW_WIDTH * 2.0f / WIDTH_PER_SEGMENTS) * mnd->scale)  * mnd->dx;
		mandel_t y_0 = (mnd->vertical + ((mandel_t)y_index - (mandel_t)WINDOW_HEIGHT / 2.0f) * mnd->scale) * mnd->dy;
		for (unsigned int x_index = 0; x_index < WINDOW_WIDTH; x_index++, x_0 += mnd->dx * mnd->scale) {
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
			unsigned int cur_pixel_index = 4 * (x_index + y_index * WINDOW_WIDTH);
			sfml->pixels[cur_pixel_index	] 	= (sf::Uint8)(n % MAXIMUM_ITERATIONS);
			sfml->pixels[cur_pixel_index + 1] 	= (sf::Uint8)(n % MAXIMUM_ITERATIONS);
			sfml->pixels[cur_pixel_index + 2] 	= (sf::Uint8)((MAXIMUM_ITERATIONS * COLOR_SENSITIVITY - n) / 20);
			sfml->pixels[cur_pixel_index + 3] 	= 255;
		}
	}
	return MANDELBROT_NO_ERROR;
}

MandelbrotStatusCode KeyHandler(sf::Event* event, Mandel_struct* mnd) {
	switch (event->key.scancode) {
		case sf::Keyboard::Scan::Left:		{ mnd->horizontal	-= SHIFT_SENSITIVITY * mnd->scale; break; }
		case sf::Keyboard::Scan::Right:		{ mnd->horizontal 	+= SHIFT_SENSITIVITY * mnd->scale; break; }
		case sf::Keyboard::Scan::Up: 		{ mnd->vertical 	-= SHIFT_SENSITIVITY * mnd->scale; break; }
		case sf::Keyboard::Scan::Down: 		{ mnd->vertical 	+= SHIFT_SENSITIVITY * mnd->scale; break; }
		case sf::Keyboard::Scan::Num0:	 	{ mnd->scale 		/= 1.5f; break; }
		case sf::Keyboard::Scan::Num9:		{ mnd->scale 		*= 1.5f; break; }
		default: return MANDELBROT_UNDEFINED_ERROR;
	}
	return MANDELBROT_NO_ERROR;
}

const wchar_t* MandelbrotErrorsMessenger(MandelbrotStatusCode status) {
	switch (status) {
		case MANDELBROT_NO_ERROR:							return L"MANDELBROT ERROR - NO ERROR";
		case MANDELBROT_SFML_CREATE_ERROR:					return L"MANDELBROT ERROR - SFML DOESN'T CREATE CLASS";
		case MANDELBROT_ALLOCATION_ERROR:					return L"MANDELBROT ERROR - MEMORY WAS ALLOCATED WITH ERROR";
		case MANDELBROT_NULL_POINTER:						return L"MANDELBROT ERROR - NULL POINTER";
		case MANDELBROT_UNDEFINED_ERROR:					return L"MANDELBROT ERROR - ERROR IS UNDEFINED, SORRY!";
		default: 											return L"UNDEFINED ERROR";
	}
}

