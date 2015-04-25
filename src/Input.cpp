#include "Input.hpp"

#include <cassert>

#include "InputDevice.hpp"
#include "Joystick.hpp"
#include "Keyboard.hpp"

#include "SFML/Vector2Utils.hpp"

Input::Input()
{
    // Order is important since only first usable will be counted
    m_devices.push_back(std::unique_ptr<InputDevice>(new Joystick()));
    m_devices.push_back(std::unique_ptr<InputDevice>(new Keyboard()));
}

void Input::update()
{
    for(auto& device : m_devices)
    {
        device->update();
    }
}

sf::Vector2f Input::getMovement()const
{
    for(const auto& device : m_devices)
    {
        if (device->enabled())
        {
            sf::Vector2f movement = device->getMovement();
            // A device can return 0,0 for several reason. So, we continue to check
            // the devices in case that the user uses another device actively
            if (SFMLUtils::length(movement) >= DEADZONE_LIMIT)
            {
                return movement;
            }
        }
    }
    assert(false); // No device enable ?
    return sf::Vector2f(0,0);
}
