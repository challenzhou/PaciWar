#include "ObjectBank.hpp"

#include "Collisions/Collider.hpp"
#include "Utils/Palette.hpp"

#include "GameState.hpp"

#include <iostream>

ObjectBank::ObjectBank()
	:m_barriersPool(250),
     m_enemiesPool(1000),m_enemiesDeathPool(1000),m_bonusPool(1000),
     m_particleSystemPool(100),
     m_explosionsPool(25)
{
	
}

void ObjectBank::createEnemyDeath(const sf::Vector2f& position)
{
    m_enemiesDeathPool.add(position);
}

void ObjectBank::createBonus(const sf::Vector2f& position)
{
    m_bonusPool.add(position);
}

void ObjectBank::createParticleSystem(const sf::Vector2f& position, const sf::Color& color)
{
    m_particleSystemPool.add(position,color);
}

void ObjectBank::createExplosion(const sf::Vector2f& position)
{
    m_explosionsPool.add(position);
}

void ObjectBank::createBarrier(const sf::Vector2f& position)
{
	m_barriersPool.add(position);
}

void ObjectBank::createEnemy(const sf::Vector2f& position)
{
	m_enemiesPool.add(position);
}

void ObjectBank::draw(sf::RenderWindow& targetWindow)
{
	m_barriersPool.update(std::bind(&Entity::draw,
									  std::placeholders::_1,
									  std::ref(targetWindow)));
	m_enemiesPool.update(std::bind(&Entity::draw,
									  std::placeholders::_1,
									  std::ref(targetWindow)));
    m_enemiesDeathPool.update(std::bind(&EnemyDeath::draw,
                                         std::placeholders::_1,
                                         std::ref(targetWindow)));
    m_bonusPool.update(std::bind(&Entity::draw,
                       std::placeholders::_1,
                       std::ref(targetWindow)));

    m_particleSystemPool.update(std::bind(&FixedColorParticleSystem::draw,
                                          std::placeholders::_1,
                                          std::ref(targetWindow)));
    m_explosionsPool.update(std::bind(&RadialExplosion::draw,
                                      std::placeholders::_1,
                                      std::ref(targetWindow)));

#ifndef NDEBUG
	m_barriersPool.update(std::bind(&Entity::debugDraw,
									  std::placeholders::_1,
									  std::ref(targetWindow)));
	m_enemiesPool.update(std::bind(&Entity::debugDraw,
									  std::placeholders::_1,
									  std::ref(targetWindow)));
    m_bonusPool.update(std::bind(&Entity::debugDraw,
                                 std::placeholders::_1,
                                 std::ref(targetWindow)));
    m_player.debugDraw(targetWindow);
#endif

	m_player.draw(targetWindow);
}

void ObjectBank::update(GameState& gstate)
{
    m_player.update(gstate);

    m_barriersPool.update(std::bind(&Barrier::update,
									  std::placeholders::_1,
                                      std::ref(gstate)));
    m_enemiesPool.update(std::bind(&Enemy::update,
									  std::placeholders::_1,
                                      std::ref(gstate)));
    m_bonusPool.update(std::bind(&Bonus::update,
                                 std::placeholders::_1,
                                 std::ref(gstate)));

    m_enemiesDeathPool.update(std::bind(&EnemyDeath::update,
                                        std::placeholders::_1,
                                        std::ref(gstate)));
    m_enemiesDeathPool.purge(std::bind(&EnemyDeath::isDead,
                                        std::placeholders::_1));

    m_particleSystemPool.update(std::bind(&FixedColorParticleSystem::update,
                                          std::placeholders::_1,
                                          gstate.getTime().getElapsedTime()));
    m_particleSystemPool.purge(std::bind(&FixedColorParticleSystem::isDead,
                                          std::placeholders::_1));

    m_explosionsPool.update(std::bind(&RadialExplosion::update,
                                      std::placeholders::_1,
                                      gstate.getTime().getElapsedTime()));
    m_explosionsPool.purge(std::bind(&RadialExplosion::isValid,
                                     std::placeholders::_1));

    applyCollision(gstate);
}

void ObjectBank::applyCollision(GameState& gstate)
{
    unsigned int barriersKilled = 0;
    unsigned int enemiesKilled = 0;
    unsigned int collectedBonusCounter = 0;

    sf::Color rainbowColor = Palette::fromHSV(gstate.getRainbowGradient(),1,1);

    Pool<Barrier>::iterator itBarrier = m_barriersPool.begin();
    for ( ; itBarrier != m_barriersPool.end() ; ++itBarrier)
    {
        BarrierCollisionResult bcr = Collider::collides(m_player,*itBarrier);
        if ( bcr.collided )
        {
            switch (bcr.getLoser())
            {
                case BarrierCollisionResult::BARRIER:
                    createBonus(itBarrier->getLeftEdgePosition());
                    createBonus(itBarrier->getRightEdgePosition());
                    createExplosion(itBarrier->getPosition());
                    itBarrier->kill();
                    barriersKilled+=2;

                    gstate.getBorders().impulse(rainbowColor);
                    break;
                case BarrierCollisionResult::PLAYER:
                    // std::cout << "Player kill" << std::endl;
                    break;
            }

            break;
        }
    }


    Pool<Enemy>::iterator itEnemies = m_enemiesPool.begin();
    for ( ; itEnemies != m_enemiesPool.end() ; ++itEnemies )
    {
        Pool<RadialExplosion>::const_iterator itExplosion = m_explosionsPool.begin();
        for ( ; itExplosion != m_explosionsPool.end() ; ++itExplosion )
        {
            CollisionResult cr = Collider::collides(*itEnemies, *itExplosion);
            if ( cr.collided )
            {
                itEnemies->kill();
                createEnemyDeath(itEnemies->getPosition());
                createBonus(itEnemies->getPosition());
                createParticleSystem(itEnemies->getPosition(),rainbowColor);
                enemiesKilled +=1;
            }
        }

        if ( ! itEnemies->isDead() )
        {
            CollisionResult cr = Collider::collides(m_player,*itEnemies);
            if ( cr.collided )
            {
                // std::cout << "Player kill by enemy" << std::endl;
            }
        }
    }

    Pool<Bonus>::iterator itBonusses = m_bonusPool.begin();
    for ( ; itBonusses != m_bonusPool.end() ; ++itBonusses )
    {
        CollisionResult cr = Collider::collides(m_player,*itBonusses);
        if ( cr.collided )
        {
            itBonusses->kill();
            collectedBonusCounter++;
        }
    }


    // Purge all collided elements
    m_barriersPool.purge(std::bind(&Entity::isDead,
                                   std::placeholders::_1));
    m_enemiesPool.purge(std::bind(&Entity::isDead,
                                  std::placeholders::_1));
    m_bonusPool.purge(std::bind(&Bonus::isDead,
                                std::placeholders::_1));

    gstate.getScore().addMultiplier(collectedBonusCounter);
    gstate.getScore().addKill(enemiesKilled,barriersKilled);
}
