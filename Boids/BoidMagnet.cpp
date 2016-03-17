#include "BoidMagnet.h"

BoidMagnet::BoidMagnet(sf::Vector2f pos, bool attract, sf::Vector2f vel)
{
	body = sf::CircleShape(12.5f);
	body.setPosition(pos);
	body.setOrigin(12.5f, 12.5f);
	velocity = vel;
	this->attract = attract;
	if (attract)
		body.setFillColor(sf::Color::Cyan);
	else if (!attract)
		body.setFillColor(sf::Color::Magenta);
	type = TYPE_MAGNET;
}

sf::Vector2f BoidMagnet::GetPosition()
{
	return body.getPosition();
}

void BoidMagnet::Update(float deltaTime)
{
	body.move(velocity * deltaTime);
	sf::Vector2f newPos = body.getPosition();
	if (newPos.x > WINDOW_SIZE_X)
		newPos.x = newPos.x - WINDOW_SIZE_X;
	else if (newPos.x < 0)
		newPos.x = newPos.x + WINDOW_SIZE_X;

	if (newPos.y > WINDOW_SIZE_Y)
		newPos.y = newPos.y - WINDOW_SIZE_Y;
	else if (newPos.y < 0)
		newPos.y = newPos.y + WINDOW_SIZE_Y;

	body.setPosition(newPos);
	GameObject::Update(deltaTime);
}

void BoidMagnet::Draw(sf::RenderWindow* window)
{
	window->draw(body);
}

void BoidMagnet::SetHitbox()
{
	hitbox = body.getGlobalBounds();
}