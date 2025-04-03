#include "Mandelbrot.hpp"

inline void mm_set_ps(mandel_t* dst, mandel_t e0, mandel_t e1, mandel_t e2, mandel_t e3) {
	dst[0] = e0;
	dst[1] = e1;
	dst[2] = e2;
	dst[3] = e3;
}
inline void mm_set_epi32(int* dst, int e0, int e1, int e2, int e3) {
	dst[0] = e0;
	dst[1] = e1;
	dst[2] = e2;
	dst[3] = e3;
}
inline void mm_set_ps1		(mandel_t* dst, mandel_t a) 				{ for (size_t i = 0; i < 4; i++) dst[i] = a; }
inline void mm_cpy_ps		(mandel_t* dst, mandel_t* src) 				{ for (size_t i = 0; i < 4; i++) dst[i] = src[i]; }
inline void mm_add_ps		(mandel_t* dst, mandel_t* a, mandel_t* b)	{ for (size_t i = 0; i < 4; i++) dst[i] = a[i] + b[i]; }
inline void mm_sub_ps		(mandel_t* dst, mandel_t* a, mandel_t* b)	{ for (size_t i = 0; i < 4; i++) dst[i] = a[i] - b[i]; }
inline void mm_mul_ps		(mandel_t* dst, mandel_t* a, mandel_t* b)	{ for (size_t i = 0; i < 4; i++) dst[i] = a[i] * b[i]; }
inline void mm_add_epi32	(int* dst, int* a, int* b)					{ for (size_t i = 0; i < 4; i++) dst[i] = a[i] + (int)b[i]; }
inline void mm_mul_1epi32	(int* dst, int* a, int val)					{ for (size_t i = 0; i < 4; i++) dst[i] = a[i] * (int)val; }
inline void mm_cmple_ps 	(int* a, mandel_t* b, int val)				{ for (size_t i = 0; i < 4; i++) { if (b[i] <= val) a[i] = 1; } }
inline int  mm_move_mask_ps (int* a) {
	int mask = 0;
	for (size_t i = 0; i < 4; i++)
		mask |= (a[i] << i);
	return mask;
}

MandelbrotStatusCode RunLab(LabWork* lab) {
	SFML* sfml = lab->sfml;
	Mandel_struct mnd = {.scale	= STANDARD_SCALE,
						 .dx 	= WIDTH_PER_SEGMENTS / (mandel_t)WINDOW_WIDTH,
			 			 .dy 	= HEIGHT_PER_SEGMENTS / (mandel_t)WINDOW_HEIGHT,
						 .runs 	= lab->runs};

	if (lab->mode == MODE_TEST) {
		lab->func(sfml, &mnd);
		return MANDELBROT_NO_ERROR;
	}

	while (sfml->window.isOpen()) {
		sf::Event event;
		while (sfml->window.pollEvent(event)) {
			switch (event.type) {
				case sf::Event::Closed: 	{ sfml->window.close(); break; }
				case sf::Event::KeyPressed: { KeyHandler(&event, &mnd); }
				default: break;
			}
		}

		lab->func(sfml, &mnd);
		sfml->texture.update(sfml->pixels);

		sfml->window.clear();
		sfml->window.draw(sfml->sprite);
		sfml->window.display();
		CalcFPS(sfml, &mnd);
	}

	return MANDELBROT_NO_ERROR;
}

MandelbrotStatusCode FunctionVersionMandelbrot(SFML* sfml, Mandel_struct* mnd) {
	mandel_t cur_dx = mnd->dx * mnd->scale;
	for (unsigned int runs = 0; runs < mnd->runs; runs++) {
		for (unsigned int y_index = 0; y_index < WINDOW_HEIGHT; y_index++) {
			mandel_t x_0 = (mnd->horizontal + (-(mandel_t)WINDOW_WIDTH * X_CENTER_IN_SEGMENTS / WIDTH_PER_SEGMENTS) * mnd->scale) * mnd->dx;
			mandel_t y_0 = (mnd->vertical + ((mandel_t)y_index - (mandel_t)WINDOW_HEIGHT / PART_OF_Y_CENTER_PXL) * mnd->scale) * mnd->dy;
			for (unsigned int x_index = 0; x_index < WINDOW_WIDTH; x_index += 4, x_0 += 4 * cur_dx) {
				mandel_t x0[4] = {}, y0[4] = {}, X[4]  = {}, Y[4]  = {};
				mm_set_ps(x0, x_0, x_0 + cur_dx, x_0 + 2 * cur_dx, x_0 + 3 * cur_dx);
				mm_set_ps1(y0, y_0);
				mm_cpy_ps(X, x0);
				mm_cpy_ps(Y, y0);
				int N[4] = {};
				for (int n = 0; n < MAXIMUM_ITERATIONS; n++) {
					mandel_t x2[4] = {}; mm_mul_ps(x2, X, X);
					mandel_t y2[4] = {}; mm_mul_ps(y2, Y, Y);
					mandel_t xy[4] = {}; mm_mul_ps(xy, X, Y);
					mandel_t r2[4] = {}; mm_add_ps(r2, x2, y2);
					int cmp[4] = {}; 	 mm_cmple_ps(cmp, r2, MAX_RADIUS);
					int mask = 			 mm_move_mask_ps(cmp);
					if (!mask) break;
					mm_add_epi32(N, N, cmp);
					mm_sub_ps(X, x2, y2);
					mm_add_ps(X, X, x0);
					mm_add_ps(Y, xy, xy);
					mm_add_ps(Y, Y, y0);
				}
				mm_mul_1epi32(N, N, COLOR_SENSITIVITY);
				for (unsigned int i = 0; i < 4; i++) {
					unsigned int cur_pixel_index = 4 * (x_index + y_index * WINDOW_WIDTH + i);
					sfml->pixels[cur_pixel_index	] 	= (sf::Uint8)(N[i] % MAXIMUM_ITERATIONS);
					sfml->pixels[cur_pixel_index + 1] 	= (sf::Uint8)(N[i] % MAXIMUM_ITERATIONS);
					sfml->pixels[cur_pixel_index + 2] 	= (sf::Uint8)((MAXIMUM_ITERATIONS * COLOR_SENSITIVITY - N[i]) / COLOR_COEFFICIENT);
					sfml->pixels[cur_pixel_index + 3] 	= 255;
				}
			}
		}
	}
	return MANDELBROT_NO_ERROR;
}

MandelbrotStatusCode CalcFPS(SFML* sfml, Mandel_struct* mnd) {
	mnd->frames_count++;
	double dt = 0.0f;
	if (mnd->frames_count == 1) mnd->start_time = clock();
	else if ((dt = (double)(clock() - mnd->start_time) / CLOCKS_PER_SEC) - STANDARD_TIME_DELAY >= EPS) {
		mnd->fps = ((double)mnd->frames_count / dt);
		mnd->frames_count = 0;
		printf("FPS: %lg\n", mnd->fps);
	}
	return MANDELBROT_NO_ERROR;
}

MandelbrotStatusCode BaseVersionMandelbrot(SFML* sfml, Mandel_struct* mnd) {
	for (unsigned int N = 0; N < mnd->runs; N++) {
		for (unsigned int y_index = 0; y_index < WINDOW_HEIGHT; y_index++) {
			mandel_t x_0 = (mnd->horizontal + (-(mandel_t)WINDOW_WIDTH * X_CENTER_IN_SEGMENTS / WIDTH_PER_SEGMENTS) * mnd->scale) * mnd->dx;
			mandel_t y_0 = (mnd->vertical + ((mandel_t)y_index - (mandel_t)WINDOW_HEIGHT / PART_OF_Y_CENTER_PXL) * mnd->scale) * mnd->dy;
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
				sfml->pixels[cur_pixel_index + 2] 	= (sf::Uint8)((MAXIMUM_ITERATIONS * COLOR_SENSITIVITY - n) / COLOR_COEFFICIENT);
				sfml->pixels[cur_pixel_index + 3] 	= 255;
			}
		}
	}
	return MANDELBROT_NO_ERROR;
}

MandelbrotStatusCode ArrayVersionMandelbrot(SFML* sfml, Mandel_struct* mnd) {
	mandel_t cur_dx = mnd->dx * mnd->scale;
	for (unsigned int runs = 0; runs < mnd->runs; runs++) {
		for (unsigned int y_index = 0; y_index < WINDOW_HEIGHT; y_index++) {
			mandel_t x_0 = (mnd->horizontal + (-(mandel_t)WINDOW_WIDTH * X_CENTER_IN_SEGMENTS / WIDTH_PER_SEGMENTS) * mnd->scale) * mnd->dx;
			mandel_t y_0 = (mnd->vertical + ((mandel_t)y_index - (mandel_t)WINDOW_HEIGHT / PART_OF_Y_CENTER_PXL) * mnd->scale) * mnd->dy;
			for (unsigned int x_index = 0; x_index < WINDOW_WIDTH; x_index += 4, x_0 += 4 * cur_dx) {
				mandel_t x0[4] = {x_0, x_0 + cur_dx, x_0 + 2 * cur_dx, x_0 + 3 * cur_dx},
						 y0[4] = {y_0, y_0, y_0, y_0},
						 X[4] = {x0[0], x0[1], x0[2], x0[3]},
						 Y[4] = {y_0, y_0, y_0, y_0};
				int N[4] = {};
				for (int n = 0; n < MAXIMUM_ITERATIONS; n++) {
					mandel_t x2[4] = {}; for (size_t i = 0; i < 4; i++) x2[i] = X[i]  * X[i];
					mandel_t y2[4] = {}; for (size_t i = 0; i < 4; i++) y2[i] = Y[i]  * Y[i];
					mandel_t xy[4] = {}; for (size_t i = 0; i < 4; i++) xy[i] = X[i]  * Y[i];
					mandel_t r2[4] = {}; for (size_t i = 0; i < 4; i++) r2[i] = x2[i] + y2[i];
					int cmp[4] = {};
					for (size_t i = 0; i < 4; i++) if (r2[i] <= MAX_RADIUS) cmp[i] = 1;
					int mask = 0;
					for (size_t i = 0; i < 4; i++) mask |= (cmp[i] << i);
					if (!mask) break;
					for (size_t i = 0; i < 4; i++) N[i] = N[i] + cmp[i];
					for (size_t i = 0; i < 4; i++) X[i] = x2[i] - y2[i] + x0[i];
					for (size_t i = 0; i < 4; i++) Y[i] = xy[i] + xy[i] + y0[i];
				}
				for (unsigned int i = 0; i < 4; i++) {
					N[i] *= COLOR_SENSITIVITY;
					unsigned int cur_pixel_index = 4 * (x_index + y_index * WINDOW_WIDTH + i);
					sfml->pixels[cur_pixel_index	] 	= (sf::Uint8)(N[i] % MAXIMUM_ITERATIONS);
					sfml->pixels[cur_pixel_index + 1] 	= (sf::Uint8)(N[i] % MAXIMUM_ITERATIONS);
					sfml->pixels[cur_pixel_index + 2] 	= (sf::Uint8)((MAXIMUM_ITERATIONS * COLOR_SENSITIVITY - N[i]) / COLOR_COEFFICIENT);
					sfml->pixels[cur_pixel_index + 3] 	= 255;
				}
			}
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
		case sf::Keyboard::Scan::Num0:	 	{ mnd->scale 		/= SCALE_COEFFICIENT; break; }
		case sf::Keyboard::Scan::Num9:		{ mnd->scale 		*= SCALE_COEFFICIENT; break; }
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
		case MANDELBROT_CMD_LINE_KEY_AMOUNT_ERROR:			return L"MANDELBROT ERROR - WRONG AMOUNT OF CMD LINE KEYS";
		case MANDELBROT_CMD_LINE_KEY_ERROR:					return L"MANDELBROT ERROR - UNKNOWN CMD LINE KEY";
		case MANDELBROT_CMD_LINE_KEY_FUNC_ERROR:			return L"MANDELBROT ERROR - UNKNOWN FUNC VERSION OF MANDELBROT";
		default: 											return L"UNDEFINED ERROR";
	}
}

