#pragma once
#include <SFML/System/Vector2.hpp>
#include <string>
class Boid;

class SteeringBehaviour abstract
{
public:
	SteeringBehaviour(Boid* parent){ this->parent = parent; }

	virtual sf::Vector2f CalculateSteeringForce() abstract;
	sf::Vector2f Seek(sf::Vector2f target);

	std::string name;
	bool enabled =  false;
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
		SB_ATTRACT,
	};
protected:
	Boid* parent;
};

