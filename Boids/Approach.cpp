#include "Approach.h"

Approach::Approach(Boid* parent, sf::Vector2f* target, float weight) : SteeringBehaviour(parent)
{
	this->target = target;
	this->weight = weight;
	type = SB_APPROACH;
	name = "Approach";
}


sf::Vector2f Approach::CalculateSteeringForce()
{
	return Seek(*target);
}
