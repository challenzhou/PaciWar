#include "Borders.hpp"

#include "Entity.hpp"

const sf::Color Borders::normalColor = sf::Color(220,220,220);

Borders::Location Borders::getCollisionLocation(const sf::Vector2f& position)const
{
    Borders::Location location = 0;

    if ( position.x <= m_innerLimits.left )
    {
        location |= Borders::LEFT;
    }

    if ( position.x >= m_innerLimits.left+m_innerLimits.width)
    {
        location |= Borders::RIGHT;
    }

    if ( position.y <= m_innerLimits.top )
    {
        location |= Borders::TOP;
    }

    if ( position.y >= m_innerLimits.top+m_innerLimits.height )
    {
        location |= Borders::DOWN;
    }

    return location;
}

void Borders::draw(sf::RenderWindow& window)
{
    sf::Color color = normalColor;
    sf::Vertex outerRect[] =
    {
        sf::Vertex(sf::Vector2f(m_limits.left - Borders::GAP, m_limits.top - Borders::GAP),                                   color),
        sf::Vertex(sf::Vector2f(m_limits.left+m_limits.width + Borders::GAP, m_limits.top - Borders::GAP),                    color),
        sf::Vertex(sf::Vector2f(m_limits.left+m_limits.width + Borders::GAP, m_limits.top+m_limits.height + Borders::GAP),    color),
        sf::Vertex(sf::Vector2f(m_limits.left - Borders::GAP, m_limits.top+m_limits.height + Borders::GAP),                   color),
        sf::Vertex(sf::Vector2f(m_limits.left - Borders::GAP, m_limits.top - Borders::GAP),                                   color)
    };

    // Second lines
    sf::Vertex innerRect[] =
    {
        sf::Vertex(sf::Vector2f(m_limits.left + Borders::GAP, m_limits.top + Borders::GAP),                                   color),
        sf::Vertex(sf::Vector2f(m_limits.left+m_limits.width - Borders::GAP, m_limits.top + Borders::GAP),                    color),
        sf::Vertex(sf::Vector2f(m_limits.left+m_limits.width - Borders::GAP, m_limits.top+m_limits.height - Borders::GAP),    color),
        sf::Vertex(sf::Vector2f(m_limits.left + Borders::GAP, m_limits.top+m_limits.height - Borders::GAP),                   color),
        sf::Vertex(sf::Vector2f(m_limits.left + Borders::GAP, m_limits.top + Borders::GAP),                                   color)
    };

    window.draw(outerRect,5,sf::LinesStrip);
    window.draw(innerRect,5,sf::LinesStrip);
}

bool Borders::isOutside(const sf::Vector2f& position)
{
    return ! m_innerLimits.contains(sf::Vector2i(position));
}


bool Borders::isOutside(const Entity& entity)
{
    return isOutside(entity.getPosition());
}

void Borders::clamp(const sf::Vector2f& position, sf::Vector2f& direction) const
{
    Borders::Location location = getCollisionLocation(position);

    if ( location & Borders::LEFT )
        if ( direction.x < 0)
            direction.x = 0;

    if ( location & Borders::RIGHT )
        if ( direction.x > 0 )
            direction.x = 0;

    if ( location & Borders::TOP )
        if ( direction.y < 0 )
             direction.y = 0;

    if ( location & Borders::DOWN )
        if ( direction.y > 0 )
            direction.y = 0;
}

void Borders::bounce(const sf::Vector2f& position, sf::Vector2f& direction)const
{
    Borders::Location location = getCollisionLocation(position);

    if ( location & Borders::LEFT ||
         location & Borders::RIGHT )
            direction.x = -direction.x;

    if ( location & Borders::TOP ||
         location & Borders::DOWN )
            direction.y = -direction.y;
}
