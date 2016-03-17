#pragma once
#include "SteeringBehaviour.h"
class Arrive :
	public SteeringBehaviour
{
public:
	Arrive(Boid* parent, sf::Vector2f* target, float weight = 1.0f);
	sf::Vector2f CalculateSteeringForce() override;
	const sf::Vector2f* target;
};

