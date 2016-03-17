#include "Wander.h"
#include "Boid.h"
#include "Utility.h"

Wander::Wander(Boid* parent, float weight) : SteeringBehaviour(parent)
{
	this->weight = weight;
	type = SB_WANDER;
	wanderAngle = 0;
	name = "Wander";
}

sf::Vector2f Wander::CalculateSteeringForce()
{
	sf::Vector2f vector(0, 0);
	sf::Vector2f circlePos = parent->Direction() * B_WANDER_CIRCLE_OFFSET + parent->GetPosition();

	std::uniform_real<double> y(-1, 1);
	wanderAngle += (float)y(game.rng) * B_WANDER_STEP;
	sf::Vector2f displacement = Utility::AngleToVector(wanderAngle) * B_WANDER_CIRCLE_RADIUS;
	vector = circlePos + displacement;
	return Seek(vector);
}