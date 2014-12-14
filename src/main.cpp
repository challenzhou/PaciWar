#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <iostream>
#include <memory>

#include "Settings.hpp"
#include "Keyboard.hpp"

#include "Collisions/Collider.hpp"

#include "Barrier.hpp"
#include "Enemy.hpp"
#include "Player.hpp"

#include "Spawner.hpp"

#include "globals.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(WIN_WIDTH,WIN_HEIGHT), Settings::windowName);
	window.setVerticalSyncEnabled(true);
	
    std::shared_ptr<Player> p = std::make_shared<Player>(sf::Vector2f(100,100));

    std::vector<std::shared_ptr<Barrier> > barriers;
    barriers.push_back(std::make_shared<Barrier>(sf::Vector2f(WIN_WIDTH/2,WIN_HEIGHT/2)));

    std::vector<std::shared_ptr<Enemy> > enemies;
    enemies.push_back(std::make_shared<Enemy>(sf::Vector2f(WIN_WIDTH/2,WIN_HEIGHT/2)));
    std::vector<std::shared_ptr<Entity> > gameEntities;
    gameEntities.push_back(barriers[barriers.size()-1]);
    gameEntities.push_back(p);
    gameEntities.push_back(enemies[enemies.size()-1]);
    Spawner spawner(sf::Vector2f(20,20),sf::Vector2f(WIN_WIDTH-20,WIN_HEIGHT-20));

    Keyboard keyboard;
	
    sf::Clock gameClock;
    sf::Time elapsedTime = sf::Time::Zero;
    sf::Time previousStartTime  = gameClock.getElapsedTime();
    sf::Time previonsEnemySpawnTime;
    sf::Time previonsBarrierSpawnTime;

	while (window.isOpen())
    {
        elapsedTime = gameClock.getElapsedTime() - previousStartTime;

        // TODO : Find better to catch window closure, or
        // filter most event to not take too much time here (mouse events)
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                break;
            }

            if (event.type == sf::Event::KeyPressed ||
                event.type == sf::Event::KeyReleased )
            {
                if ( event.key.code == sf::Keyboard::Escape )
                {
                    window.close();
                    break;
                }
            }
        }
                
        window.clear(sf::Color::Black);

        // First try with input.
        // TODO : better input, should take time between two iteration
        // TODO : having a progressive system (aka acceleration) on key press
        // float updateSpeed = elapsedTime.asSeconds() * 1000;
        keyboard.update();
        p->move(keyboard.getMovement());


        for(auto pGameEntity : gameEntities)
        {
            pGameEntity->draw(window);
        }

        for(auto pGameEntity : gameEntities)
        {
            pGameEntity->debugDraw(window);
        }

        for (auto b : barriers)
        {
            if ( Collider::collides(*p,*b) == CollisionResult::PLAYER)
            {
                std::cout << "PLayer kill" << std::endl;
            }
            if ( Collider::collides(*p,*b) == CollisionResult::BARRIER)
            {
                std::cout << "Barrier kill" << std::endl;
            }

            b->update(elapsedTime.asMilliseconds());
        }

        for (auto e : enemies)
        {
            e->move(enemies, *p);
        }


        window.display();

        previousStartTime = gameClock.getElapsedTime();
        if ((gameClock.getElapsedTime() - previonsEnemySpawnTime).asMilliseconds() > 1000 )
        {
            std::vector<std::shared_ptr<Enemy> > newEnemies;
            spawner.spawnEnemies(p->getPosition(),newEnemies);
            for ( unsigned int i = 0 ; i < newEnemies.size() ; i++)
            {
                enemies.push_back(newEnemies[i]);
                gameEntities.push_back(newEnemies[i]);
            }

            previonsEnemySpawnTime = gameClock.getElapsedTime();
        }

        if ((gameClock.getElapsedTime() - previonsBarrierSpawnTime).asMilliseconds() > 5000 )
        {
            std::vector<std::shared_ptr<Barrier> > newBarriers;
            spawner.spawnBarriers(p->getPosition(),newBarriers);
            for ( unsigned int i = 0 ; i < newBarriers.size() ; i++)
            {
                barriers.push_back(newBarriers[i]);
                gameEntities.push_back(newBarriers[i]);
            }

            previonsBarrierSpawnTime = gameClock.getElapsedTime();
        }
        // std::cout << "Elapsed time (s): " << elapsedTime.asSeconds() << std::endl;
    }
	
	return 0;
}
