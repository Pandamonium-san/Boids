#include "Separation.h"
#include "Boid.h"
#include "Utility.h"

Separation::Separation(Boid* parent, float weight) : SteeringBehaviour(parent)
{
	this->weight = weight;
	type = SB_SEPARATION;
}

sf::Vector2f Separation::CalculateSteeringForce()
{
	GameObjList& neighbours = parent->GetNear();
	if (neighbours.size() == 0)
		return sf::Vector2f(0, 0);

	sf::Vector2f vector = sf::Vector2f(0, 0);
	GameObjList::const_iterator itr;
	int count = 0;
	for (itr = neighbours.begin(); itr != neighbours.end(); ++itr)
	{
		if ((*itr)->type == GameObject::TYPE_BOID)
		{
			sf::Vector2f dir = parent->GetPosition() - (*itr)->GetPosition();
			float distance = Utility::LengthOfVector2(dir);
			if (distance <= B_SEP_RAD)
			{
				vector += dir / distance;
				++count;
			}
		}
	}
	if (count > 0)
	{
		vector = vector / (float)count;
		return Seek(vector + parent->GetPosition());
	}
	else
		return sf::Vector2f(0, 0);
}


