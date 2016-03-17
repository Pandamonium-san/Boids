#pragma once
#include "SteeringBehaviour.h"
class Pursuit :
	public SteeringBehaviour
{
public:
	Pursuit(Boid* parent, Boid* target, float weight = 1.0f);
	sf::Vector2f CalculateSteeringForce() override;
	Boid* target;
};

