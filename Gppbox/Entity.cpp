#include "Entity.hpp"
#include "C.hpp"
#include <imgui.h>

Entity::Entity(sf::Shape* _spr) : spr(_spr) {

}

void Entity::update(double dt){
	double rate = 1.0 / dt;
	double dfr = 60.0f / rate;

	rx += dx * dt;
	ry += dy * dt;

	dx = dx * pow(frx, dfr);
	dy = dy * pow(fry, dfr);

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

	sf::Vector2f coo = { cx + rx, cy + ry };
	bool chgCoo = DragFloat2("coo x/y", &coo.x, 1.0f, -2000,2000);
	if (chgCoo) 
		setCooGrid(coo.x, coo.y);
	
	chg |= DragFloat2("dx/dy", &dx, 0.01f, -20,20);

	return chg||chgCoo;
}

sf::Vector2i Entity::getPosPixel()
{
	return sf::Vector2i( (cx+rx)*C::GRID_SIZE, (cy+ry) * C::GRID_SIZE );
}

