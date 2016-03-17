#include <iostream>
#include <iterator>
#include <algorithm>
#include "Game.h"
#include "GameObject.h"
#include <time.h>
#include <fstream>
#include "Boid.h"
#include "BoidMagnet.h"
//#include "Utility.h"

Game::Game(int width, int height, std::string name)
{
	srand(time(NULL));
	window = new sf::RenderWindow(sf::VideoMode(WINDOW_SIZE_X, WINDOW_SIZE_Y), name);
	window->setFramerateLimit(60);
	Arial.loadFromFile("arial.ttf");
	Initialize();
}

void Game::RunGameLoop()
{
	while (window->isOpen())
	{
		HandleEvents();
		Update();
		Draw();
	}
}

void Game::Initialize()
{
	clock.restart();
	rng.seed(time(NULL));

	for (size_t i = 0; i < 30; i++)
	{
		int x = rand() % window->getSize().x;
		int y = rand() % window->getSize().y;
		int vx = rand() % 400 - 200;
		int vy = rand() % 400 - 200;
		Boid* boid = new Boid(sf::Vector2f(x, y), sf::Vector2f(vy, vx));
		PostGameObj(boid);
		if (i == 1)
			boid->DebugMode(true);
	}

}

void Game::PostGameObj(GameObject* obj)
{
	gameObjects.push_back(obj);
}

bool RemoveNotActive(GameObject* obj)
{
	if (!obj->isActive)
	{
		delete obj;
		return true;
	}
	return false;
}

void Game::SpawnBoid(sf::Vector2f pos)
{
	std::uniform_int_distribution<int> x(-200, 200);
	Boid* boid = new Boid(pos, sf::Vector2f(x(rng), x(rng)));
	PostGameObj(boid);
}

void Game::SpawnMagnet(sf::Vector2f pos, bool attract)
{
	BoidMagnet* m = new BoidMagnet(pos, attract);
	PostGameObj(m);
}

void Game::ClearAll()
{
	GameObjList::iterator itr;
	for (itr = gameObjects.begin(); itr != gameObjects.end(); ++itr)
	{
		(*itr)->isActive = false;
	}
	Boid::pursuit = false;
}

void Game::TogglePursuit()
{
	Boid::pursuit = !Boid::pursuit;
	if (Boid::pursuit)
	{
		std::uniform_int_distribution<int> x(-200, 200);
		Boid::target = new BoidMagnet(sf::Vector2f(x(rng), x(rng)), false, sf::Vector2f(x(rng), x(rng)));
		PostGameObj(Boid::target);
		std::cout << "Pursuit enabled." << std::endl;
	}
	else if (!Boid::pursuit)
	{
		Boid::target->isActive = false;
		std::cout << "Pursuit disabled." << std::endl;
	}
}

void Game::HandleEvents()
{
	sf::Event event;
	while (window->pollEvent(event))
	{
		window->setKeyRepeatEnabled(false);
		if (event.type == sf::Event::Closed)
			window->close();
		else if (event.type == sf::Event::MouseMoved)
		{
			mousePos = sf::Vector2f(event.mouseMove.x, event.mouseMove.y);
		}
		else if (event.type == sf::Event::MouseButtonPressed)
		{
			switch (event.mouseButton.button)
			{
			case sf::Mouse::Left:
				SpawnBoid(sf::Vector2f(event.mouseButton.x, event.mouseButton.y));
				break;
			case sf::Mouse::Middle:
				SpawnMagnet(sf::Vector2f(event.mouseButton.x, event.mouseButton.y), true);
				break;
			case sf::Mouse::Right:
				SpawnMagnet(sf::Vector2f(event.mouseButton.x, event.mouseButton.y), false);
				break;
			default:
				break;
			}
		}
		else if (event.type == sf::Event::KeyPressed)
		{
			switch (event.key.code)
			{
			case sf::Keyboard::P:
				paused = !paused;
				std::cout << "Paused: " << paused << std::endl;
				break;
			case sf::Keyboard::Num1:
				Boid::cohesion = !Boid::cohesion;
				std::cout << "Cohesion: " << Boid::cohesion << std::endl;
				break;
			case sf::Keyboard::Num2:
				Boid::alignment = !Boid::alignment;
				std::cout << "Alignment: " << Boid::alignment << std::endl;
				break;
			case sf::Keyboard::Num3:
				Boid::separation = !Boid::separation;
				std::cout << "Separation: " << Boid::separation << std::endl;
				break;
			case sf::Keyboard::Num4:
				Boid::wander = !Boid::wander;
				std::cout << "Wander: " << Boid::wander << std::endl;
				break;
			case sf::Keyboard::Num5:
				TogglePursuit();
				break;
			case sf::Keyboard::Num6:
				Boid::arrive = !Boid::arrive;
				std::cout << "Arrive: " << Boid::arrive << std::endl;
				break;
			case sf::Keyboard::C:
				ClearAll();
				break;
			default:
				break;
			}
		}

		if (paused)
			return;

		GameObjList::iterator list1;
		for (list1 = gameObjects.begin(); list1 != gameObjects.end(); ++list1)
		{
			if ((*list1)->isActive)
			{
				(*list1)->HandleEvents(event);
			}
			else continue;
		}
	}
}
void Game::Update()
{
	sf::Time time = clock.getElapsedTime();
	dt = time.asSeconds();
	clock.restart();
	
	if (paused)
		return;

	GameObjList::iterator list1;
	for (list1 = gameObjects.begin(); list1 != gameObjects.end(); ++list1)
	{
		if ((*list1)->isActive)
		{
			(*list1)->Update(dt);
		}
		else continue;

		GameObjList::iterator list2;
		for (list2 = gameObjects.begin(); list2 != gameObjects.end(); ++list2)
		{
			//obj may have already collided, turning it inactive
			if (!(*list1)->isActive)
				continue;
			if (list1 == list2)
				continue;

			if ((*list2)->isActive && (*list1)->IsColliding(*list2))
			{
				(*list1)->DoCollision(*list2);
			}
		}

	}

	GameObjList::iterator end = gameObjects.end();
	GameObjList::iterator newEnd = remove_if(gameObjects.begin(), gameObjects.end(), RemoveNotActive);
	if (newEnd != end)
		gameObjects.erase(newEnd, end);
}

void Game::Draw()
{
	if (paused)
		return;

	window->clear();

	GameObjList::iterator list;
	for (list = gameObjects.begin(); list != gameObjects.end(); ++list)
	{
		if ((*list)->isActive)
			(*list)->Draw(window);
	}

	window->display();
}

void Game::FindAllGameObjOfType(int type, GameObjList& list)
{
	GameObjList::iterator itr;
	for (itr = gameObjects.begin(); itr != gameObjects.end(); ++itr)
	{
		if (!(*itr)->isActive)
			continue;

		if ((*itr)->type == type)
			list.push_back(*itr);
	}
}
