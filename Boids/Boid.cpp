#include "Boid.h"
#include "Utility.h"
#include <iostream>
#include <time.h>
#include <random>
#include "BoidMagnet.h"

bool Boid::cohesion = true;
bool Boid::alignment = true;
bool Boid::separation = true;
bool Boid::wander = false;
bool Boid::pursuit = false;
bool Boid::arrive = false;
BoidMagnet* Boid::target;

Boid::Boid(sf::Vector2f pos, sf::Vector2f vel)
{
	body = sf::ConvexShape(3);
	body.setPoint(0, sf::Vector2f(0, 6));
	body.setPoint(1, sf::Vector2f(24, 12));
	body.setPoint(2, sf::Vector2f(0, 18));
	body.setOrigin(sf::Vector2f(8, 12));
	body.setPosition(pos);
	velocity = vel;
	currentSpeed = B_MAXSPD;

	type = TYPE_BOID;
}

void Boid::Update(float deltaTime)
{
	CalculateAcceleration();
	CalculateVelocity(deltaTime);
	body.move(velocity * deltaTime);
	LookAtHeading();
	WrapAroundScreen();
	GameObject::Update(deltaTime);
}

void Boid::LookAtHeading()
{
	body.setRotation(Utility::VectorToAngle(velocity));
}

void Boid::CalculateVelocity(float dt)
{
	velocity += acceleration * dt;
	float speed = Utility::LengthOfVector2(velocity);
	if (speed > B_MAXSPD)
		velocity = Utility::Normalized(velocity) * B_MAXSPD;
}

void Boid::CalculateAcceleration()
{
	acceleration = sf::Vector2f(0, 0);
	if (pursuit)
		acceleration += Pursuit();
	if (arrive)
		acceleration += Arrive(game.mousePos)/game.dt;
	BoidList neighbours = GetNeighbours();
	if (neighbours.size() != 0)
	{
		if (cohesion)
			acceleration += Cohesion(neighbours);
		if (alignment)
			acceleration += Alignment(neighbours);
		if (separation)
			acceleration += Separation(neighbours);
	}
	if (wander)
		acceleration += Wander() * 0.5f;

	MagnetList magnets = GetMagnets();
	acceleration += MagnetsAttract(magnets);

	float force = Utility::LengthOfVector2(acceleration);
	if (force > B_MAXFORCE)
		acceleration = Utility::Normalized(acceleration) * B_MAXFORCE;
	acceleration += MagnetsRepel(magnets) * 10.f;
}

sf::Vector2f Boid::Seek(sf::Vector2f target)
{
	sf::Vector2f desired = target - body.getPosition();
	desired = Utility::Normalized(desired) * B_MAXSPD;
	sf::Vector2f steer = desired - velocity;
	return steer;
}

sf::Vector2f Boid::Arrive(sf::Vector2f target)
{
	float distance = Utility::Distance(this->body.getPosition(), target);
	if (distance > B_ARRIVAL_RADIUS)
		return Seek(target);
	else
	{
		sf::Vector2f desired = target - body.getPosition();
		desired = Utility::Normalized(desired) * B_MAXSPD * (distance/B_ARRIVAL_RADIUS);
		sf::Vector2f steer = desired - velocity;
		return steer;
	}
}

sf::Vector2f Boid::Pursuit()
{
	float estimated = Utility::Distance(this->body.getPosition(), target->body.getPosition()) / B_MAXSPD;
	sf::Vector2f desired = target->body.getPosition() + target->velocity*estimated - body.getPosition();
	desired = Utility::Normalized(desired) * B_MAXSPD;
	sf::Vector2f steer = desired - velocity;
	return steer;
}

sf::Vector2f Boid::Cohesion(const BoidList& neighbours)
{
	sf::Vector2f centerOfMass = sf::Vector2f(0, 0);
	BoidList::const_iterator itr;
	for (itr = neighbours.begin(); itr != neighbours.end(); ++itr)
	{
		centerOfMass += (*itr)->body.getPosition();
	}
	centerOfMass = centerOfMass / (float)neighbours.size();
	return Seek(centerOfMass);
}

sf::Vector2f Boid::Alignment(const BoidList& neighbours)
{
	sf::Vector2f vector = sf::Vector2f(0, 0);
	BoidList::const_iterator itr;
	for (itr = neighbours.begin(); itr != neighbours.end(); ++itr)
	{
		vector += (*itr)->Direction();
	}
	vector = vector / (float)neighbours.size();
	return Seek(vector + body.getPosition());
}

sf::Vector2f Boid::Separation(const BoidList& neighbours)
{
	sf::Vector2f vector = sf::Vector2f(0, 0);
	BoidList::const_iterator itr;
	int count = 0;
	for (itr = neighbours.begin(); itr != neighbours.end(); ++itr)
	{
		sf::Vector2f dir = this->body.getPosition() - (*itr)->body.getPosition();
		float distance = Utility::LengthOfVector2(dir);
		if (distance <= B_SEP_RAD)
		{
			vector += dir / distance;
			++count;
		}
	}
	if (count > 0)
	{
		vector = vector / (float)count;
		return Seek(vector + body.getPosition());
	}
	else
	{
		return sf::Vector2f(0, 0);
	}
}

sf::Vector2f Boid::Wander()
{
	sf::Vector2f vector(0, 0);
	sf::Vector2f circlePos = Direction() * B_WANDER_CIRCLE_OFFSET + body.getPosition();

	std::uniform_real<double> y(-1, 1);
	wanderAngle += y(game.rng) * B_WANDER_STEP;
	sf::Vector2f displacement = Utility::AngleToVector(wanderAngle) * B_WANDER_CIRCLE_RADIUS;
	vector = circlePos + displacement;
	debugPos = vector;
	return Seek(vector);
}

sf::Vector2f Boid::MagnetsAttract(const MagnetList& near)
{
	sf::Vector2f vector(0, 0);
	int count = 0;
	MagnetList::const_iterator itr;
	for (itr = near.begin(); itr != near.end(); ++itr)
	{
		sf::Vector2f dir = (*itr)->body.getPosition() - this->body.getPosition();
		if ((*itr)->attract)
		{
			vector += dir;
			++count;
		}
	}
	if (count > 0)
	{
		vector = vector / (float)count;
		return Seek(vector + body.getPosition());
	}
	else
		return vector;
}

sf::Vector2f Boid::MagnetsRepel(const MagnetList& near)
{
	sf::Vector2f vector(0, 0);
	int count = 0;
	MagnetList::const_iterator itr;
	for (itr = near.begin(); itr != near.end(); ++itr)
	{
		sf::Vector2f dir = this->body.getPosition() - (*itr)->body.getPosition();
		float distance = Utility::LengthOfVector2(dir);
		if (distance <= M_RADIUS)
		{
			vector += dir / distance;
			++count;
		}
	}
	if (count > 0)
	{
		vector = vector / (float)count;
		return Seek(vector + body.getPosition());
	}
	else
		return vector;
}

BoidList Boid::GetNeighbours()
{
	BoidList neighbours;
	GameObjList boids;
	game.FindAllGameObjOfType(TYPE_BOID, boids);
	GameObjList::iterator itr;
	for (itr = boids.begin(); itr != boids.end(); ++itr)
	{
		Boid* b = dynamic_cast<Boid*>(*itr);
		if (b == this)
			continue;
		sf::Vector2f local = b->body.getPosition() - this->body.getPosition();
		float angle = Utility::AngleBetweenVectors(Direction(), local);
		float distance = Utility::LengthOfVector2(local);
		if (distance <= B_PERC_RAD && abs(angle) < B_PERC_ANG)
			neighbours.push_back(b);
	}
	return neighbours;
}

MagnetList Boid::GetMagnets()
{
	MagnetList near;
	GameObjList out;
	game.FindAllGameObjOfType(TYPE_MAGNET, out);
	GameObjList::iterator itr;
	for (itr = out.begin(); itr != out.end(); ++itr)
	{
		BoidMagnet* b = dynamic_cast<BoidMagnet*>(*itr);
		sf::Vector2f local = b->body.getPosition() - this->body.getPosition();
		float angle = Utility::AngleBetweenVectors(Direction(), local);
		float distance = Utility::LengthOfVector2(local);
		if (distance <= B_PERC_RAD && abs(angle) < B_PERC_ANG)
			near.push_back(b);
	}
	return near;
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

sf::Vector2f Boid::Direction()
{
	return Utility::AngleToVector(body.getRotation());
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
	BoidList nb = GetNeighbours();

	sf::Vector2f co = Cohesion(nb);
	sf::Vector2f al = Alignment(nb);
	sf::Vector2f se = Separation(nb);

	std::string cs = "Cohesion {" + Utility::ToString(co.x) + ", " + Utility::ToString(co.y) + "}";
	std::string as = "Alignment {" + Utility::ToString(al.x) + ", " + Utility::ToString(al.y) + "}";
	std::string ss = "Separation {" + Utility::ToString(se.x) + ", " + Utility::ToString(se.y) + "}";
	std::string vs = "Speed " + Utility::ToString(Utility::LengthOfVector2(velocity));

	sf::Text n = sf::Text("Neighbours " + Utility::ToString(nb.size()), game.Arial);
	sf::Text c = sf::Text(cs, game.Arial);
	sf::Text a = sf::Text(as, game.Arial);
	sf::Text s = sf::Text(ss, game.Arial);
	sf::Text v = sf::Text(vs, game.Arial);
	n.setPosition(0, 0);
	//c.setPosition(0, 30);
	//a.setPosition(0, 60);
	//s.setPosition(0, 90);
	v.setPosition(0, 30);
	sf::CircleShape com = sf::CircleShape(5, 5);
	com.setFillColor(sf::Color::Red);
	com.setOrigin(2.5f, 2.5f);
	com.setPosition(debugPos);

	//sf::CircleShape sep = sf::CircleShape(10, 10);
	//sep.setFillColor(sf::Color::Green);
	//sep.setOrigin(5.f, 5.f);
	//sep.setPosition(se);
	//window->draw(sep);
	window->draw(com);
	window->draw(n);
	//window->draw(c);
	//window->draw(a);
	//window->draw(s);
	window->draw(v);
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