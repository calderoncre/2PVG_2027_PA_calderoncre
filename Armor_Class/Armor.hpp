#ifndef ARMOR_HPP
#define ARMOR_HPP
#include <cassert>
class Armor
{
public:
    //! constructors
    Armor()
        : hp_{100}, protection{0.3f}
    {
    }

    Armor(int vida, float proteccion)
        : hp_{vida}, protection{proteccion}
    {
    }

    int getHp() const;
    bool isDestroyed() const;
    int applyDamageReduction(int damage); // Pass the damage in, return reduced damage
    void reduceLife(int damage);

    const float protection; //< Fraction of damage that is absorbed. Damage reduction between 0 and 1.
private:
    int hp_;
};

#endif