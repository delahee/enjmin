#pragma once

#include "SFML/Graphics.hpp"

using namespace sf;

class Interp{
public:

	static sf::Vector2f lerp(Vector2f v0, Vector2f v1, float t) {
		float tx = (v1.x - v0.x) * t + v0.x;
		float ty = (v1.y - v0.y) * t + v0.y;
		return sf::Vector2f(tx, ty);
	}

	static float catmull(
		float p0, 
		float p1, 
		float p2, 
		float p3, float t) {
		
		float q = 2.0f * p1;
		float t2 = t * t;

		q += (-p0 + p2) * t;
		q += (2.0f * p0 - 5.0f * p1 + 4 * p2 - p3) * t2;
		q += (-p0 + 3 * p1 - 3 * p2 + p3) * t2 * t;

		return 0.5f * q;
	};

	static Vector2f c2(
		Vector2f p0, 
		Vector2f p1, 
		Vector2f p2, 
		Vector2f p3, float t)
	{
		sf::Vector2f res(
			catmull(p0.x, p1.x, p2.x, p3.x,t),
			catmull(p0.y, p1.y, p2.y, p3.y,t)
			);
		return res;
	}
};