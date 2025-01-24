#include "Entity.hpp"
#include "C.hpp"
#include <imgui.h>
#include "Game.hpp"

Entity::Entity(sf::Shape* _spr) : spr(_spr) {

}

void Entity::update(double dt){
	double rate = 1.0 / dt;
	double dfr = 60.0f / rate;

	dx = dx * pow(frx, dfr);
	dy = dy * pow(fry, dfr);

	rx += dx * dt;
	ry += dy * dt;

	Game& g = *Game::me;
	
	if (rx > 1.0f) {
		if (! g.hasCollision( cx+rx,cy+ry)) {
			rx--;
			cx++;
		}
		else {
			dx = 0;
			rx = 0.7f;
		}

	}
	if (rx < 0) {
		if (!g.hasCollision(cx + rx, cy + ry)) {
			rx++;
			cx--;
		}
		else {
			dx = 0;
			rx = 0.3f;
		}

	}

	
	syncPos();
}

void Entity::setCooPixel(int px, int py){
	cx = px / C::GRID_SIZE;
	cy = py / C::GRID_SIZE;

	rx = (px - (cx * C::GRID_SIZE)) / (float)C::GRID_SIZE;
	ry = (py - (cy * C::GRID_SIZE)) / (float)C::GRID_SIZE;

	syncPos();
}

void Entity::setCooGrid(float coox, float cooy){
	cx = (int)coox;
	rx = coox - cx;

	cy = (int)cooy;
	ry = cooy - cy;
	syncPos();
}

void Entity::syncPos() {
	sf::Vector2f pos = { (cx + rx) * C::GRID_SIZE, (cy + ry) * C::GRID_SIZE };
	spr->setPosition(pos);
}

void Entity::draw(sf::RenderWindow& win){
	if (spr)
		win.draw(*spr);
}

bool Entity::im()
{
	using namespace ImGui;

	bool chg = false;
	
	Value("cx", cx);
	Value("cy", cy);

	Value("rx",rx);
	Value("ry",ry);

	sf::Vector2i pix = getPosPixel();
	chg |= DragInt2("rx/ry", &pix.x, 1.0f, -2000,2000);
	if (chg) 
		setCooPixel(pix.x, pix.y);

	chg |= DragInt2("cx/cy", &cx, 1.0f, -2000, 2000);
	
	sf::Vector2f coo = { cx + rx, cy + ry };
	bool chgCoo = DragFloat2("coo px x/y", &coo.x, 1.0f, -2000,2000);
	if (chgCoo) 
		setCooGrid(coo.x, coo.y);
	
	chg |= DragFloat2("dx/dy", &dx, 0.01f, -20,20);
	chg |= DragFloat2("frx/fry", &frx, 0.001f, 0, 1);

	return chg||chgCoo;
}

sf::Vector2i Entity::getPosPixel()
{
	return sf::Vector2i( (cx+rx)*C::GRID_SIZE, (cy+ry) * C::GRID_SIZE );
}

