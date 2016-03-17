#include "Cohesion.h"
#include "Boid.h"

Cohesion::Cohesion(Boid* parent, float weight) : SteeringBehaviour(parent)
{
	this->weight = 1.0f;
	type = SB_COHESION;
}

sf::Vector2f Cohesion::CalculateSteeringForce()
{
	GameObjList& neighbours = parent->GetNear();
	if (neighbours.size() == 0)
		return sf::Vector2f(0, 0);
	sf::Vector2f centerOfMass = sf::Vector2f(0, 0);
	GameObjList::const_iterator itr;
	for (itr = neighbours.begin(); itr != neighbours.end(); ++itr)
	{
		if ((*itr)->type == GameObject::TYPE_BOID)
			centerOfMass += (*itr)->GetPosition();
	}
	centerOfMass = centerOfMass / (float)neighbours.size();
	return Seek(centerOfMass);
}


