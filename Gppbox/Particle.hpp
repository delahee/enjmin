#pragma once

#include "SFML/Graphics.hpp"
#include "Lib.hpp"
#include "Dice.hpp"
#include <functional>

using namespace sf;
class Particle {
public:

	RectangleShape	el;
	float			x = 0.0f;
	float			y = 0.0f;

	float			dx = 0.0f;
	float			dy = 0.0f;

	float			life = 1.0f;

	float			scaleX = 1.0f;
	float			scaleY = 1.0f;

	bool			destroyed = false;

	std::function<void(Particle * lthis, float dt)> bhv;

	Particle();

	void update(float dt) {
		x += dx * dt;
		y += dy * dt;

		el.setPosition(x, y);
		el.rotate(dt * 3.0f);

		life -= dt;
		
		el.setScale(scaleX, scaleY);
		
		bhv(this,dt);

		if (el.getSize().x <= 0.1 || el.getSize().y <= 0.1) {
			destroyed = true;
		}
	}
};