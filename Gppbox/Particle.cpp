#include "Particle.hpp"

Particle::Particle() {
	el = RectangleShape(Vector2f(8, 8));
	el.setOrigin(4, 4);
	el.setFillColor(sf::Color(0xff0000ff));
	life = Dice::randF() * 6.0f + 2.0f * 6.0f;
}
