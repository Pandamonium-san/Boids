#include "Boid.h"
#include "Utility.h"
#include <iostream>
#include <time.h>
#include <random>
#include "BoidMagnet.h"

Boid::Boid(sf::Vector2f pos, sf::Vector2f vel)
{
	body = sf::ConvexShape(3);
	body.setPoint(0, sf::Vector2f(0, 6));
	body.setPoint(1, sf::Vector2f(24, 12));
	body.setPoint(2, sf::Vector2f(0, 18));
	body.setOrigin(sf::Vector2f(8, 12));
	body.setPosition(pos);
	velocity = vel;

	sbs.push_back(new Approach(this, &game.mousePos));
	sbs.push_back(new Cohesion(this));
	sbs.push_back(new Separation(this, 1.2f));
	sbs.push_back(new Alignment(this));
	sbs.push_back(new Wander(this, 0.5f));
	sbs.push_back(new Arrive(this, &game.mousePos, 5.f));
	sbs.push_back(new Attract(this, 1.f));
	sbs.push_back(new Pursuit(this, game.player));

	type = TYPE_BOID;
}

void Boid::Update(float deltaTime)
{
	ClearNear();
	CalculateAcceleration();
	CalculateVelocity(deltaTime);
	body.move(velocity * deltaTime);
	LookAtHeading();
	WrapAroundScreen();
	GameObject::Update(deltaTime);
}

void Boid::DoCollision(GameObject* other)
{
	if (other->type != TYPE_BOID)
	{
		sf::Vector2f dir = GetPosition() - other->GetPosition();
		velocity += Utility::Normalized(dir)*100.f;
	}
}


sf::Vector2f Boid::CalculateBehavioursForce()
{
	sf::Vector2f totalForce(0, 0);
	SBList::iterator itr;
	for (itr = sbs.begin(); itr != sbs.end(); ++itr)
	{
		if ((*itr)->enabled)
		{
			sf::Vector2f force = (*itr)->CalculateSteeringForce();
			totalForce += force * (*itr)->weight;
		}
	}
	if (Utility::LengthOfVector2(totalForce) > B_MAXFORCE)
		totalForce = Utility::Normalized(totalForce) * B_MAXFORCE;
	return totalForce;
}

void Boid::CalculateAcceleration()
{
	acceleration = sf::Vector2f(0, 0);
	AddForce(CalculateBehavioursForce());
}

void Boid::CalculateVelocity(float dt)
{
	velocity += acceleration * dt;
	float speed = Utility::LengthOfVector2(velocity);
	if (speed > B_MAXSPD)
		velocity = Utility::Normalized(velocity) * B_MAXSPD;
}

void Boid::WrapAroundScreen()
{
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
}

void Boid::AddForce(sf::Vector2f force)
{
	acceleration += force / mass;
}

void Boid::LookAtHeading()
{
	body.setRotation(Utility::VectorToAngle(velocity));
}

GameObjList& Boid::GetNear()
{
	if (near.size() > 0)
		return near;

	GameObjList::iterator itr;
	for (itr = game.gameObjects.begin(); itr != game.gameObjects.end(); ++itr)
	{
		if ((*itr)->type == TYPE_BOID || (*itr)->type == TYPE_MAGNET)
		{
			sf::Vector2f local = (*itr)->GetPosition() - this->GetPosition();
			float angle = Utility::AngleBetweenVectors(Direction(), local);
			float distance = Utility::LengthOfVector2(local);
			if (distance <= B_PERC_RAD && abs(angle) < B_PERC_ANG)
				near.push_back(*itr);
		}
	}
	return near;
}

void Boid::ClearNear()
{
	near.clear();
}

SteeringBehaviour* Boid::GetBehaviour(int type)
{
	SBList::iterator itr;
	for (itr = sbs.begin(); itr != sbs.end(); ++itr)
	{
		if ((*itr)->type == type)
			return *itr;
	}
}

sf::Vector2f Boid::Direction()
{
	return Utility::AngleToVector(body.getRotation());
}
sf::Vector2f Boid::GetPosition()
{
	return body.getPosition();
}
sf::Vector2f Boid::GetVelocity()
{
	return velocity;
}

void Boid::Draw(sf::RenderWindow* window)
{
	if (debug)
		DrawDebug(window);
	window->draw(body);
}

void Boid::SetHitbox()
{
	hitbox = body.getGlobalBounds();
}

void Boid::DrawDebug(sf::RenderWindow* window)
{
	//BoidList nb = GetNeighbours();

	//sf::Text n = sf::Text("Neighbours " + Utility::ToString(nb.size()), game.Arial);
	////c.setPosition(0, 30);
	////a.setPosition(0, 60);
	////s.setPosition(0, 90);

	////sf::CircleShape sep = sf::CircleShape(10, 10);
	////sep.setFillColor(sf::Color::Green);
	////sep.setOrigin(5.f, 5.f);
	////sep.setPosition(se);
	////window->draw(sep);
	//window->draw(n);
	////window->draw(c);
	////window->draw(a);
	////window->draw(s);
	//window->draw(v);
}

void Boid::DebugMode(bool activate)
{
	if (activate)
	{
		debug = true;
		body.setFillColor(sf::Color::Cyan);
	}
	else
	{
		debug = false;
		body.setFillColor(sf::Color::White);
	}
}