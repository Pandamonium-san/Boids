#pragma once
#include "SteeringBehaviour.h"
class Separation :
	public SteeringBehaviour
{
public:
	Separation(Boid* parent, float weight = 1.0f);
	sf::Vector2f CalculateSteeringForce() override;
};

