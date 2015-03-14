#ifndef GAME_HPP
#define GAME_HPP

#include <memory>
#include <vector>

#include <SFML/Graphics.hpp>

#include "ObjectPool.hpp"

#include "ObjectBank.hpp"
#include "ParticleSystem.hpp"

#include "Spawner.hpp"
#include "GameTime.hpp"

#include "Keyboard.hpp"

class Game
{
private:
    sf::RenderWindow& m_targetWindow;

    ObjectBank m_objects;
    Pool<ParticleSystem> m_particleSystemPool;

    Spawner m_spawner;

    Keyboard m_keyboard;
    GameTime m_gameTime;

    void render();
    void update();

    void spawnParticles();
    void checkClosure();

public:
    Game(sf::RenderWindow& targetWindow);

    bool run();
};

#endif
