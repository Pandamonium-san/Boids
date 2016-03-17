#pragma once
#include "GameObject.h"

#define M_RADIUS 25.f

class BoidMagnet : public GameObject
{
public:
	BoidMagnet(sf::Vector2f pos, bool attract, sf::Vector2f vel = sf::Vector2f(0, 0));
	void Update(float deltaTime) override;
	void Draw(sf::RenderWindow* window) override;
	void SetHitbox() override;
	sf::Vector2f GetPosition() override;

	sf::CircleShape body;
	sf::Vector2f velocity;
	bool attract;
};