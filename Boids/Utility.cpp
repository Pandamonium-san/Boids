#include "Utility.h"
#include <iostream>

#define RADIANTOANGLE 57.2958279088f

sf::Vector2f Utility::AngleToVector(float angle)
{
	float radians = angle / RADIANTOANGLE;
	return sf::Vector2f(cos(radians), sin(radians));
}

float Utility::VectorToAngle(const sf::Vector2f& v)
{
	return (atan2(-v.y, -v.x) * RADIANTOANGLE) + 180;
}

float Utility::ScalarProduct(const sf::Vector2f& v1, const sf::Vector2f& v2)
{
	return (v1.x * v2.x + v1.y * v2.y);
}

float Utility::LengthOfVector2(const sf::Vector2f& v)
{
	return sqrt(v.x * v.x + v.y * v.y);
}

bool Utility::VectorsAreParallel(const sf::Vector2f& v1, const sf::Vector2f& v2, float tolerance)
{
	return ScalarProduct(v1, v2) / (LengthOfVector2(v1) * LengthOfVector2(v2)) > 1 - tolerance;
}

float Utility::Distance(const sf::Vector2f& v1, const sf::Vector2f& v2)
{
	return sqrt((v1.x - v2.x)*(v1.x - v2.x) + (v1.y - v2.y)*(v1.y - v2.y));
}

float Utility::DistanceSquared(const sf::Vector2f& v1, const sf::Vector2f& v2)
{
	return (v1.x - v2.x)*(v1.x - v2.x) + (v1.y - v2.y)*(v1.y - v2.y);
}

sf::Vector2f Utility::Normalized(sf::Vector2f v)
{
	float length = LengthOfVector2(v);
	if (length != length || length == 0)
		return v;
	return v / length;
}

float Utility::AngleBetweenVectors(const sf::Vector2f& v1, const sf::Vector2f& v2)
{
	return acos(ScalarProduct(v1, v2) / (LengthOfVector2(v1) * LengthOfVector2(v2))) * RADIANTOANGLE;
}



