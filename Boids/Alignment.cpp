#include "Alignment.h"
#include "Boid.h"

Alignment::Alignment(Boid* parent, float weight) : SteeringBehaviour(parent)
{
	this->weight = weight;
	type = SB_ALIGNMENT;
}

sf::Vector2f Alignment::CalculateSteeringForce()
{
	GameObjList& neighbours = parent->GetNear();
	if (neighbours.size() == 0)
		return sf::Vector2f(0, 0);

	sf::Vector2f vector = sf::Vector2f(0, 0);
	int count = 0;
	GameObjList::const_iterator itr;
	for (itr = neighbours.begin(); itr != neighbours.end(); ++itr)
	{
		Boid* b = dynamic_cast<Boid*>(*itr);
		if (b != NULL)
		{
			vector += b->Direction();
			++count;
		}
	}
	if (count > 0)
		vector = vector / (float)count;
	else
		return vector;
	return Seek(vector + parent->GetPosition());
}