#pragma once
#include "SteeringBehaviour.h"
class Wander :
	public SteeringBehaviour
{
public:
	Wander(Boid* parent, float weight = 1.0f);
	sf::Vector2f CalculateSteeringForce() override;
private:
	float wanderAngle;
};

