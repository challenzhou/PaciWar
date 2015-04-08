#ifndef BONUS_HPP
#define BONUS_HPP

#include <SFML/Graphics.hpp>

#include "Entity.hpp"

#include "Momentum.hpp"

class Bonus : public Entity
{
private:
    static constexpr float SPEED = 0.53f;              // Should be a bit greater than Player::SPEED otherwise
                                                        // you can't get them
    static constexpr unsigned int LIFETIME = 4000;
    static constexpr unsigned int MAGNET_DISTANCE = 60; // Should be a bit smaller than RadialExplosion::RADIUS_MAX
                                                        // to keep player going back to get these and take risk

    static constexpr unsigned int BLINK_TIME = 1000;
    static constexpr unsigned int BLINK_DELAY = 200;

    static const sf::Color normalColor;
    static const sf::Color lightColor;

    sf::Color m_color;

    int m_life;

    Momentum m_momentum;

public:
    Bonus(const sf::Vector2f& position)
        :Entity(position),m_color(normalColor),m_life(LIFETIME) {}

    void draw(sf::RenderWindow& window);
    void move(unsigned int deltaTime, const Entity& target);

    void getBoundingSpheres(BoundingSpheres& boundingSpheres)const;

    static constexpr float SIZE = 1;

    bool isDead()const { return m_life < 0 || m_dead; }
};

#endif
