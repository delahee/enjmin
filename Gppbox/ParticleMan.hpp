#pragma once

#include <vector>
#include "Particle.hpp"

class ParticleMan {
public:
	std::vector<Particle> parts;
	
	void add(Particle & p) {
		parts.push_back(p);
	}

	void update(double dt) {
		for (auto iter = parts.begin(); iter != parts.end(); ) {
			Particle & p = *iter;
			p.update((float)dt);
			if (p.destroyed)
				iter = parts.erase(iter);
			else
				iter++;
		}
	}

	void draw(sf::RenderWindow& win) {
		for (Particle & p : parts) {
			win.draw(p.el);
		}
	}
};