#include "Spawner.hpp"

#include <memory>
#include <vector>

#include "GameState.hpp"
#include "Barrier.hpp"

#include "RandomGenerator.hpp"
#include "SFML/Vector2Utils.hpp"

Spawner::Spawner(const sf::IntRect& limits)
    :m_spawnGrid(limits),m_numberBarrierBySpawn(1),m_numberEnemiesBySpawn(1)
{

}

void Spawner::spawnBarriers(GameState& gstate)
{
    sf::IntRect limits = gstate.getBorders().getRestrictedLimits();

    for (unsigned int i = 0 ; i < m_numberBarrierBySpawn ; i++)
    {
        sf::Vector2f pos;
        do
        {
            pos.x = RandomGenerator::getInt(limits.left,limits.left+limits.width);
            pos.y = RandomGenerator::getInt(limits.top,limits.top+limits.height);
        }while(SFMLUtils::distance(pos,gstate.getObjects().getPlayer().getPosition()) < BARRIER_SPAWN_DISTANCE*BARRIER_SPAWN_DISTANCE);
        gstate.getObjects().createBarrier(pos);
    }
}

void Spawner::spawnEnemies(GameState& gstate)
{
    m_spawnGrid.spawnEnemies(gstate,m_numberEnemiesBySpawn);
    m_numberEnemiesBySpawn++;
}
