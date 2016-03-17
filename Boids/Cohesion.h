#pragma once
#include "SteeringBehaviour.h"
class Cohesion :
	public SteeringBehaviour
{
public:
	Cohesion(Boid* parent, float weight = 1.0f);
	sf::Vector2f CalculateSteeringForce() override;
};

