#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <cstdio>
#include <string>
#include "Armor.hpp"

#define clamp(a, b, c) std::min((a), std::max((b), (c)))

class Player
{
public:
    std::string name;
    void changeLife(int change);
    bool spendMoney(int change);

    void takeDamage(int damage);
    bool changeSpeed(int change);
    // void equipArmor(Armor &armor); // el antiguo malloc esta aquí, entenderlo muy bien
    // delete es el free
    void printPlayer();
    void printCurrentArmor();
    void printAllArmorHP() const;

    void equipArmor(const Armor &armor_type);
    void unequipArmor(int slot);
    void reorderArmorList()
    {
        for (int i = 0; i < armor_max_count_ - 1; i++)
        {
            if (armor_[i] == nullptr)
            {
                if (armor_[i + 1] == nullptr) // momento en el que ya no hay mas armaduras que ordenar, ha llegado al ultimo nodo
                    return;                   // salimos de la funcion directamente
                armor_[i] = armor_[i + 1];
                armor_[i + 1] = nullptr;
            }
        }
    }
    // cero lógico, pon un int en 0, un array en nullptr, etc
    // armor_{0},
    Player() : armor_{0}, armor_max_count_{10}, x_{0.0f}, y_{0.0f}, speed_{10.0f}, maxHp_{100}, hp_{maxHp_}, gold_{50}, max_speed_{10000.0f}
    //! constructor de la clase
    {
    }
    //! destructor de la clase
    ~Player()
    {
    }

private:
    const int armor_max_count_;
    Armor *armor_[10];
    float x_;
    float y_;
    float speed_;
    int maxHp_;
    int hp_;
    int gold_;
    float max_speed_;

    int *sexo;

    // speed ente minSpeed y maxSpeed
};

void buyPotion(Player &p);
void fallInLava(Player &p);
void pickUpBoots(Player &p);

#endif