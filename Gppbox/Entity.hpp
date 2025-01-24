#pragma once

#include <SFML/Graphics.hpp>

namespace sf {
	class RenderWindow;
}

class Entity{
public:
	sf::Shape * spr = 0;
	int			cx = 0;
	int			cy = 0;
	float		rx = 0.5f;
	float		ry = 0.0f;

	float		dx = 0.f;
	float		dy = 0.f;
	float		gravy = 0.f;

	float		frx = 0.88f;
	float		fry = 1.0f;

					Entity(sf::Shape* spr);

	void			update( double dt );
	void			setCooPixel( int px, int py);
	void			setCooGrid( float coox, float cooy);
	void			syncPos();
	void			draw(sf::RenderWindow& win);
	bool			im();

	sf::Vector2i	getPosPixel();
};