#include "EnemyDeath.hpp"

#include "GameState.hpp"
#include "Enemy.hpp"

#include "Utils/RandomGenerator.hpp"
#include "Math/Vector2.hpp"

const sf::Color EnemyDeath::baseColor = sf::Color(220,220,220);

EnemyDeath::Line::Line(const sf::Vector2f &center, float rotation)
    :center(center),rotation(rotation),
     moveDirectionBias(RandomGenerator::getNormalizedDirection()),
     moveSpeed(RandomGenerator::getFloat(0.25,0.45f)),
     rotationSpeed(RandomGenerator::getFloat(0.8,1.2f))
{
    Math::normalise(moveDirectionBias);
}

void EnemyDeath::Line::draw(sf::RenderWindow &window, const sf::Color& color) const
{
    const sf::Vertex lines[] =
    {
        sf::Vertex(sf::Vector2f(0, 10),color),
        sf::Vertex(sf::Vector2f(10, 0),color)
    };

    sf::Transform t;
    t.translate(center);
    t.rotate(rotation);

    window.draw(lines,2,sf::Lines,t);
}


EnemyDeath::EnemyDeath(const sf::Vector2f &position)
    :Entity(position),
     m_lines{{
         Line(sf::Vector2f(position.x-2,
                           position.y+2),90),
         Line(sf::Vector2f(position.x-2,
                           position.y-2),180),
         Line(sf::Vector2f(position.x+2,
                           position.y-2),270),
         Line(sf::Vector2f(position.x+2,
                           position.y+2),0)
            }},
     m_lifeTime(0)
{

}

void EnemyDeath::draw(sf::RenderWindow& window)const
{
    static constexpr float ratio = 255 / static_cast<float>(HDURATION);
    
    sf::Color color = sf::Color(baseColor.r,baseColor.g,baseColor.b,255);
    if ( m_lifeTime > HDURATION ) // Fades out after half lifetime
    {
        color.a = (DURATION - m_lifeTime) * ratio;
    }
    
    for (const Line& line : m_lines)
    {
        line.draw(window,color);
    }
}

void EnemyDeath::update(GameState& gstate)
{
    float deltaTime = gstate.getTime().getElapsedTime();
    for (Line& line : m_lines)
    {
        sf::Vector2f direction = line.center - m_position + line.moveDirectionBias ;
        Math::normalise(direction);

        line.center +=  direction * (line.moveSpeed * deltaTime);
        line.rotation += line.rotationSpeed * deltaTime;

        line.moveSpeed -= line.moveSpeed * 0.005 * deltaTime;
        line.rotationSpeed -= line.rotationSpeed * 0.004 * deltaTime;
    }

    m_lifeTime += deltaTime;
}