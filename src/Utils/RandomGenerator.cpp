/*
 * PaciWar : Remake of the "pacifism" mode from Geometry Wars 2
 * Copyright (C) 2014-2015 LittleWhite (lw.demoscene@gmail.com)
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "RandomGenerator.hpp"

#include "Math/Vector2.hpp"

std::mt19937 RandomGenerator::m_generator = std::mt19937(std::random_device()());

float RandomGenerator::getFloat(float min, float max)
{
    std::uniform_real_distribution<> dist(min,max);
    return dist(m_generator);
}

int RandomGenerator::getInt(int min, int max)
{
    std::uniform_int_distribution<> dist(min,max);
    return dist(m_generator);
}

sf::Vector2i RandomGenerator::getIntVector(int minx, int maxx, int miny, int maxy)
{
    std::uniform_int_distribution<> distx(minx,maxx);
    std::uniform_int_distribution<> disty(miny,maxy);
    return sf::Vector2i(distx(m_generator),disty(m_generator));
}

sf::Vector2f RandomGenerator::getFloatVector(float minx, float maxx, float miny, float maxy)
{
    std::uniform_real_distribution<> distx(minx,maxx);
    std::uniform_real_distribution<> disty(miny,maxy);
    return sf::Vector2f(distx(m_generator),disty(m_generator));
}

sf::Vector2f RandomGenerator::getNormalizedDirection()
{
    std::uniform_real_distribution<> dist(-1.0f,1.0f);
    sf::Vector2f direction(dist(m_generator),dist(m_generator));
    Math::normalise(direction);
    return direction;
}

sf::Color RandomGenerator::getColor(unsigned char min, unsigned char max)
{
    std::uniform_int_distribution<> dist(min,max);
    return sf::Color(dist(m_generator),dist(m_generator),dist(m_generator));
}
