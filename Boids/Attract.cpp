#include "Attract.h"
#include "Boid.h"
#include "Utility.h"
#include "BoidMagnet.h"

Attract::Attract(Boid* parent, float weight) : SteeringBehaviour(parent)
{
	this->weight = weight;
	type = SB_ATTRACT;
	name = "Attract";
}

sf::Vector2f Attract::CalculateSteeringForce()
{
	GameObjList& near = parent->GetNear();
	sf::Vector2f vector(0, 0);
	int count = 0;
	GameObjList::const_iterator itr;
	for (itr = near.begin(); itr != near.end(); ++itr)
	{
		BoidMagnet* bm = dynamic_cast<BoidMagnet*>(*itr);
		if (bm != NULL)
		{
			sf::Vector2f dir = bm->GetPosition() - parent->GetPosition();
			if (bm->attract)
				vector += dir;
			else
				vector -= dir;
			++count;
		}
	}
	if (count > 0)
	{
		vector = vector / (float)count;
		return Seek(vector + parent->GetPosition());
	}
	else
		return vector;
}