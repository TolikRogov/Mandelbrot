#pragma once

#include <SFML/Graphics.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define RED(str) 		"\033[31;1m" str "\033[0m"
#define YELLOW(str) 	"\033[33;4m" str "\033[0m"
#define GREEN(str) 		"\033[32;1m" str "\033[0m"
#define BLUE(str)		"\033[34;1m" str "\033[0m"
#define TEAL(str)		"\033[36;1m" str "\033[0m"

#define MANDELBROT_ERROR_CHECK(status) {																			\
	if (status != MANDELBROT_NO_ERROR) {																			\
		fprintf(stderr, "\n\n" RED("Error (code %d): %ls, ") YELLOW("File: %s, Function: %s, Line: %d\n\n"),   		\
					status, MandelbrotErrorsMessenger(status), __FILE__, __PRETTY_FUNCTION__, __LINE__);			\
		fclose(stderr);																								\
		return status;																								\
	}																												\
}

#define MANDELBROT_ERROR_MESSAGE(status) {																			\
	if (status != MANDELBROT_NO_ERROR) {																			\
		fprintf(stderr, "\n\n" RED("Error (code %d): %ls, ") YELLOW("File: %s, Function: %s, Line: %d\n\n"),   		\
					status, MandelbrotErrorsMessenger(status), __FILE__, __PRETTY_FUNCTION__, __LINE__);			\
		fclose(stderr);																								\
		break;																										\
	}																												\
}

enum MandelbrotStatusCode {
	MANDELBROT_NO_ERROR,
	MANDELBROT_SFML_CREATE_ERROR,
	MANDELBROT_ALLOCATION_ERROR,
	MANDELBROT_NULL_POINTER,
	MANDELBROT_UNDEFINED_ERROR,
	MANDELBROT_CMD_LINE_KEY_AMOUNT_ERROR,
	MANDELBROT_CMD_LINE_KEY_ERROR,
	MANDELBROT_CMD_LINE_KEY_FUNC_ERROR,
};

const wchar_t* MandelbrotErrorsMessenger(MandelbrotStatusCode status);
