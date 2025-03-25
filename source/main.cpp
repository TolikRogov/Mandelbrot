#include "Mandelbrot.hpp"

int main()
{
	srand((unsigned int)time(NULL));

	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Mandelbrot!");

	sf::Uint8* pixels = new sf::Uint8[PIXELS_SIZE];

	sf::Texture texture;
	texture.create(WINDOW_WIDTH, WINDOW_HEIGHT);

	sf::Sprite sprite;
	sprite.setTexture(texture);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		for (size_t i = 0; i < PIXELS_SIZE; i++)
			pixels[i] = (sf::Uint8)(rand() % 255);
		texture.update(pixels);

		window.clear();
		window.draw(sprite);
		window.display();
	}

	return 0;
}
