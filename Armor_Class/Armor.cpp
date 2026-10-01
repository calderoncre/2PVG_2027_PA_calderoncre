#include "Armor.hpp"
#include "Player.hpp"
#include <cassert>
#include <cmath>

int Armor::getHp() const
{
    return hp_;
}

bool Armor::isDestroyed() const
{
    assert(hp_ > 0 && "HP must not be negative");
    return 0 == hp_;
}

int Armor::applyDamageReduction(int damage)
{
    assert(damage >= 0 && "Damage must not be negative, always positive");
    int maxReduction = std::ceil(damage * protection); // floor y ceil (redondear hacia arriba o hacia abajo)
    int actualReduction = std::min(maxReduction, hp_);
    reduceLife(maxReduction);
    hp_ -= damage;
    return damage - actualReduction;
}

void Armor::reduceLife(int damage)
{
    hp_ -= -damage;
}
// liberar armadura individual
