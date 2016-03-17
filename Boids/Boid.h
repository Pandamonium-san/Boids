#pragma once
#include "GameObject.h"

#define B_MAXSPD 300.0f
#define B_ROTSPD 180.0f
#define B_SEP_RAD 50.0f
#define B_PERC_RAD 100.0f
#define B_PERC_ANG 150.0f
#define B_MAXFORCE 5000.f
#define B_WANDER_CIRCLE_OFFSET 40.f
#define B_WANDER_CIRCLE_RADIUS 50.f
#define B_WANDER_STEP 20.f
#define B_ARRIVAL_RADIUS 200.f

class Boid;
class BoidMagnet;
class SteeringBehaviour;

typedef std::vector<Boid*> BoidList;
typedef std::vector<BoidMagnet*> MagnetList;
typedef std::vector<SteeringBehaviour*> SBList;

class Boid : public GameObject
{
public:
	static bool cohesion;
	static bool alignment;
	static bool separation;
	static bool wander;
	static bool pursuit;
	static bool arrive;
	static BoidMagnet* target;
public:
	Boid(sf::Vector2f pos, sf::Vector2f vel);
	void Update(float deltaTime) override;
	void Draw(sf::RenderWindow* window) override;
	void SetHitbox() override;

	sf::Vector2f Direction();
	sf::Vector2f GetPosition() override;
	sf::Vector2f GetVelocity();
	GameObjList& GetNear();
	BoidList& GetNeighbours();
	MagnetList GetMagnets();

	sf::ConvexShape body;

	void DebugMode(bool activate);
private:
	void LookAtHeading();
	void WrapAroundScreen();
	void CalculateAcceleration();
	void CalculateVelocity(float dt);
	sf::Vector2f Seek(sf::Vector2f target);
	//sf::Vector2f Cohesion(const BoidList& neighbours);
	//sf::Vector2f Alignment(const BoidList& neighbours);
	//sf::Vector2f Separation(const BoidList& neighbours);
	//sf::Vector2f MagnetsAttract(const MagnetList& near);
	//sf::Vector2f MagnetsRepel(const MagnetList& near);
	//sf::Vector2f Wander();

	//sf::Vector2f Arrive(sf::Vector2f target);
	//sf::Vector2f Pursuit();

	float currentSpeed;
	float wanderAngle = 0;
	sf::Vector2f velocity;
	sf::Vector2f acceleration;

	void ClearNear();
	sf::Vector2f CalculateBehavioursForce();
	void AddForce(sf::Vector2f force);
	SBList sbs;
	float mass = 1.0f;
	BoidList neighbours;
	GameObjList near;

	void DrawDebug(sf::RenderWindow* window);
	bool debug = false;
	sf::Vector2f debugPos;
};
