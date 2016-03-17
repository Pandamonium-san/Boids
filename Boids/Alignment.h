#pragma once
#include "SteeringBehaviour.h"
class Alignment :
	public SteeringBehaviour
{
public:
	Alignment(Boid* parent, float weight = 1.0f);
	sf::Vector2f CalculateSteeringForce() override;
};

