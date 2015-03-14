#include "Game.hpp"

#include <iostream>

#include <SFML/Graphics.hpp>

#include "Collisions/Collider.hpp"

#include "globals.hpp"

Game::Game(sf::RenderWindow& targetWindow)
    :m_targetWindow(targetWindow),
     m_spawner(sf::Vector2f(20,20),sf::Vector2f(WIN_WIDTH-20,WIN_HEIGHT-20)),
     m_particleSystemPool(10000)
{
}

void Game::render()
{
    m_targetWindow.clear(sf::Color::Black);

	m_objects.draw(m_targetWindow);

    m_particleSystemPool.update(std::bind(&ParticleSystem::draw,
                                          std::placeholders::_1,
                                          std::ref(m_targetWindow)));

    m_targetWindow.display();
}

void Game::update()
{
    m_gameTime.update();
    m_keyboard.update();

    m_objects.getPlayer().move(m_keyboard.getMovement());
	m_objects.update(m_gameTime.getElapsedTime());
/*
    for (auto b : m_barriers)
    {
        if ( Collider::collides(*m_player,*b) == CollisionResult::PLAYER)
        {
            std::cout << "PLayer kill" << std::endl;
        }
        if ( Collider::collides(*m_player,*b) == CollisionResult::BARRIER)
        {
            std::cout << "Barrier kill" << std::endl;
        }
    }
*/

    if ( m_gameTime.shouldSpawnEnemy())
    {
        std::vector<std::shared_ptr<Enemy> > newEnemies;
        m_spawner.spawnEnemies(m_objects.getPlayer().getPosition(),newEnemies);
        for ( unsigned int i = 0 ; i < newEnemies.size() ; i++)
        {
            // m_enemies.push_back(newEnemies[i]);
        }
    }

    if ( m_gameTime.shouldSpawnBarrier())
    {
        std::vector<std::shared_ptr<Barrier> > newBarriers;
        m_spawner.spawnBarriers(m_objects.getPlayer().getPosition(),newBarriers);
        for ( unsigned int i = 0 ; i < newBarriers.size() ; i++)
        {
            // m_barriers.push_back(newBarriers[i]);
        }
    }


    m_particleSystemPool.update(std::bind(&ParticleSystem::update,
                                          std::placeholders::_1,
                                          m_gameTime.getElapsedTime()));
    m_particleSystemPool.purge(std::bind(&ParticleSystem::isDead,
                                          std::placeholders::_1));

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        spawnParticles();
    }
}

void Game::spawnParticles()
{
    ParticleSystem p(sf::Vector2f(sf::Mouse::getPosition(m_targetWindow)));
    m_particleSystemPool.add(p);
}

void Game::checkClosure()
{
    // TODO : Find better to catch window closure, or
    // filter most event to not take too much time here (mouse events)
    sf::Event event;
    while (m_targetWindow.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            m_targetWindow.close();
            break;
        }

        if (event.type == sf::Event::KeyPressed ||
            event.type == sf::Event::KeyReleased )
        {
            if ( event.key.code == sf::Keyboard::Escape )
            {
                m_targetWindow.close();
                break;
            }
        }
    }
}

bool Game::run()
{
    while (m_targetWindow.isOpen())
    {
        render();
        update();

        checkClosure();
    }
    return true;
}