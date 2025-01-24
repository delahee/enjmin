#pragma once

#include <chrono>
#include <ctime>
#include <SFML/Graphics/Color.hpp>


using namespace std;
using namespace std::chrono;

namespace Lib {
	double getTimeStamp();
	constexpr double pi() { return 3.141592653589793238462643383279502884; }
	sf::Color makeFromHSV(float _h, float _s, float _v);
};