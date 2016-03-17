#pragma once
#include <SFML/System/Vector2.hpp>

class Boid;

class SteeringBehaviour abstract
{
public:
	SteeringBehaviour(Boid* parent){ this->parent = parent; }

	virtual sf::Vector2f CalculateSteeringForce() abstract;
	sf::Vector2f Seek(sf::Vector2f target);

	bool enabled;
	int type;
	float weight = 1.0f;

	enum
	{
		SB_APPROACH,
		SB_COHESION,
		SB_ALIGNMENT,
		SB_SEPARATION,
		SB_WANDER,
		SB_PURSUIT,
		SB_ARRIVE,
	};
protected:
	Boid* parent;
};

