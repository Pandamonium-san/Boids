#pragma once
#include "SteeringBehaviour.h"
class Approach :
	public SteeringBehaviour
{
public:
	Approach(Boid* parent, sf::Vector2f* target, float weight = 1.0f);
	sf::Vector2f CalculateSteeringForce() override;
	const sf::Vector2f* target;
};

