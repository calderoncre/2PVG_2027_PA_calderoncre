#ifndef ARMOR_HPP
#define ARMOR_HPP

class Armor
{
public:
    //! constructors
    Armor()
        : vida_{100}, proteccion_{0.3f}
    {
    }

    Armor(int vida, float proteccion)
        : vida_{vida}, proteccion_{proteccion}
    {
    }

    // Pass the damage in, return reduced damage
    // int applyDamageReduction(int damage);

private:
    int vida_;
    float proteccion_; //< Fraction of damage that is absorbed
};

#endif