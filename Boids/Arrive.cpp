#include "Arrive.h"
#include "Boid.h"
#include "Utility.h"

Arrive::Arrive(Boid* parent, sf::Vector2f* target, float weight) : SteeringBehaviour(parent)
{
	this->target = target;
	this->weight = weight;
	type = SB_ARRIVE;
}

sf::Vector2f Arrive::CalculateSteeringForce()
{
	float distance = Utility::Distance(parent->GetPosition(), *target);
	if (distance > B_ARRIVAL_RADIUS)
		return Seek(*target);
	else
	{
		sf::Vector2f desired = *target - parent->GetPosition();
		desired = Utility::Normalized(desired) * B_MAXSPD * (distance / B_ARRIVAL_RADIUS);
		sf::Vector2f steer = desired - parent->GetVelocity();
		return steer;
	}
}