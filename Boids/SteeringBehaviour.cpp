#include "SteeringBehaviour.h"
#include "Utility.h"
#include "Boid.h"

sf::Vector2f SteeringBehaviour::Seek(sf::Vector2f target)
{
	sf::Vector2f desired = target - parent->GetPosition();
	desired = Utility::Normalized(desired) * B_MAXSPD;
	sf::Vector2f steer = desired - parent->GetVelocity();
	return steer;
}