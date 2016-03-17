#pragma once
#include "SteeringBehaviour.h"
class Attract :
	public SteeringBehaviour
{
public:
	Attract(Boid* parent, float weight = 1.0f);
	sf::Vector2f CalculateSteeringForce() override;
};

