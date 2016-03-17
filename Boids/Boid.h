#pragma once
#include "GameObject.h"
#include "Steer.h"

#define B_MAXSPD 300.0f
#define B_SEP_RAD 50.0f
#define B_PERC_RAD 100.0f
#define B_PERC_ANG 150.0f
#define B_MAXFORCE 500.f
#define B_WANDER_CIRCLE_OFFSET 40.f
#define B_WANDER_CIRCLE_RADIUS 50.f
#define B_WANDER_STEP 20.f
#define B_ARRIVAL_RADIUS 200.f

class Boid;
class BoidMagnet;

typedef std::vector<Boid*> BoidList;
typedef std::vector<BoidMagnet*> MagnetList;
typedef std::vector<SteeringBehaviour*> SBList;

class Boid : public GameObject
{
public:
	Boid(sf::Vector2f pos, sf::Vector2f vel);
	void Update(float deltaTime) override;
	void Draw(sf::RenderWindow* window) override;
	void SetHitbox() override;
	void DoCollision(GameObject* other) override;

	sf::Vector2f Direction();
	sf::Vector2f GetPosition() override;
	sf::Vector2f GetVelocity();
	GameObjList& GetNear();
	SteeringBehaviour* GetBehaviour(int type);

	sf::ConvexShape body;

	void DebugMode(bool activate);
private:
	void LookAtHeading();
	void WrapAroundScreen();
	sf::Vector2f CalculateBehavioursForce();
	void CalculateAcceleration();
	void CalculateVelocity(float dt);
	void AddForce(sf::Vector2f force);
	void ClearNear();

	sf::Vector2f velocity;
	sf::Vector2f acceleration;

	SBList sbs;
	float mass = 1.0f;
	GameObjList near;

	void DrawDebug(sf::RenderWindow* window);
	bool debug = false;
};
