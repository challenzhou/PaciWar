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

#ifndef REPLAYER_HPP
#define REPLAYER_HPP

#include <fstream>

#include "InputDevice.hpp"

#include <SFML/System.hpp>

class Replayer : public InputDevice
{
    typedef std::pair<int64_t, sf::Vector2f> DataLine;

private:
    std::ifstream m_input;
    sf::Clock m_clock;
    int64_t m_lastUpdate;
    int64_t m_accum;
    DataLine m_lastRead;
    bool m_isFirstLine;

    DataLine readLine();

public:
    Replayer(const std::string& inputFile);

    void update();
    sf::Vector2f getMovement()const;

    int64_t getDeltaTime()const;

    void start();

    void restart();
};

#endif
