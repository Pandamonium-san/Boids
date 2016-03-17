#include "Pursuit.h"
#include "Boid.h"
#include "Utility.h"

Pursuit::Pursuit(Boid* parent, Boid* target, float weight) : SteeringBehaviour(parent)
{
	this->target = target;
	this->weight = weight;
	type = SB_PURSUIT;
}

sf::Vector2f Pursuit::CalculateSteeringForce()
{
	float estimated = Utility::Distance(parent->GetPosition(), target->GetPosition()) / B_MAXSPD;
	sf::Vector2f desired = target->GetPosition() + target->GetVelocity()*estimated - parent->GetPosition();
	desired = Utility::Normalized(desired) * B_MAXSPD;
	sf::Vector2f steer = desired - parent->GetVelocity();
	return steer;
}