#include "Vector2.hpp"

#include "Math/constants.hpp"

float Math::length(const sf::Vector2f& v)
{
    return sqrt(v.x * v.x + v.y * v.y);
}

void Math::normalise(sf::Vector2f& v)
{
    if (v.x == 0 && v.y == 0)
    {
        return;
    }

    float l = Math::length(v);
    v.x = v.x/l;
    v.y = v.y/l;
}

float Math::distance(const sf::Vector2f& v1, const sf::Vector2f& v2)
{
    float deltaX = v1.x - v2.x;
    float deltaY = v1.y - v2.y;
    return deltaX*deltaX + deltaY*deltaY;
}

float Math::getAngle(const sf::Vector2f& v)
{
    float angle = atan(v.y/v.x) * 180.f/M_PI;
    if ( v.x >= 0 ) // First quadrant, is ok
    {
        if ( v.y < 0 ) // Forth quadran
        {
            angle += 360;
        }
    }
    if ( (v.x < 0) ) // Second and third
    {
        angle += 180;
    }

    return angle;
}

sf::Vector2f Math::getVectorFromAngle(float angle)
{
    return std::move(sf::Vector2f(std::cos(angle*M_PI/180.f),std::sin(angle*M_PI/180.f)));

}