#pragma once
#include <SFML/Graphics.hpp>
#include <list>
#include <random>
#include <string>
#include <map>

#define WINDOW_SIZE_X 800
#define WINDOW_SIZE_Y 600

class Boid;
class GameObject;
class GA;

typedef std::list<GameObject*> GameObjList;

class Game{
public:
	Game(int width, int height, std::string name);
	void RunGameLoop();
	void Initialize();
	void HandleEvents();
	void Update();
	void Draw();
	void PostGameObj(GameObject* obj);
	void FindAllGameObjOfType(int type, GameObjList& list);

	void SpawnPlayer();
	void SpawnBoid(sf::Vector2f pos);
	void SpawnMagnet(sf::Vector2f pos, bool attract);
	void ClearAll();
	void ToggleBehaviour(int sbType);
	void DrawBehaviours();

	std::mt19937 rng;
	sf::RenderWindow* window;
	sf::Clock clock;
	float dt;

	GameObjList gameObjects;
	Boid* player;
	sf::Vector2f mousePos;
	sf::Font Arial;

	std::map<int, bool> sbMap;

private:
	bool paused = false;
};

extern Game game;