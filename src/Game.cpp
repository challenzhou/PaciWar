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

#include "Game.hpp"

#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>
#include <chrono>


#include "Collisions/Collider.hpp"

#include "Utils/RandomGenerator.hpp"
#include "Tracer.hpp"
#include "Settings.hpp"

Game::Game(const Settings& settings, sf::RenderWindow& targetWindow)
    :m_targetWindow(targetWindow),m_view(sf::Vector2f(settings.getGameAreaWidth()/2,settings.getGameAreaHeight()/2),sf::Vector2f(settings.getGameAreaWidth(),settings.getGameAreaHeight())),m_userInterface(settings.getFontPath()
#if DEBUG_INFO == 1
    ,settings.getDebugFontPath()
#endif
    ),
     m_state(settings)
{
}

void Game::render()
{
    auto now = std::chrono::system_clock::now();
    auto drawTiming = std::chrono::time_point_cast<std::chrono::milliseconds>(now);


    m_targetWindow.clear(sf::Color::Black);
    m_targetWindow.setView(m_view);

    m_state.getObjects().draw(m_targetWindow);
    m_state.getBorders().draw(m_targetWindow);


    m_targetWindow.setView(m_targetWindow.getDefaultView());
    m_userInterface.draw(m_targetWindow);

    m_targetWindow.display();
    m_userInterface.setDrawTime(drawTiming.time_since_epoch().count());
    m_userInterface.setDrawTime(0);
}

void Game::update()
{
    auto now = std::chrono::system_clock::now();
    auto updateTiming = std::chrono::time_point_cast<std::chrono::milliseconds>(now);

    m_state.update();

    m_view.setCenter((m_state.getObjects().getPlayer().getPosition())/VIEW_DELAY_FACTOR);

    m_userInterface.update(m_state);
    m_userInterface.setUpdateTime(updateTiming.time_since_epoch().count());
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
    std::unique_ptr<Tracer> tracer;
    if ( m_state.getSettings().isTracing() )
    {
        tracer.reset(new Tracer(m_state.getSettings().getTraceFile()));
    }

    while (m_targetWindow.isOpen() && !m_state.isGameOver())
    {
        if (tracer)
        {
            tracer->trace(m_state);
        }

        render();
        update();

        checkClosure();
    }

    std::cout << "Score : " << m_state.getScore().getScore() << std::endl;
    return true;
}
