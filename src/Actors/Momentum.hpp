#ifndef MOMENTUM_HPP
#define MOMENTUM_HPP

#include <SFML/Graphics.hpp>

class Momentum
{
private:
    sf::Vector2f m_direction;
    float m_moveSpeed;
    float m_rotationSpeed;

public:
    Momentum();
    Momentum(float maxMoveSpeed, float maxRotationSpeed);
    Momentum(const sf::Vector2f& direction, float moveSpeed, float rotationSpeed);

    void update(sf::Vector2f& position, float& rotation, unsigned int deltaTime);

    const sf::Vector2f& getDirection()const { return m_direction; }
    void updateDirection(const sf::Vector2f& newDirection) { m_direction = newDirection; }
};

#endif