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
    void equipArmor(Armor &armor_type);
    void unequipArmor(int slot);

    // cero lógico, pon un int en 0, un array en nullptr, etc
    // armor_{0},
    Player() : armor_{0}, armor_count_{0}, x_{0.0f}, y_{0.0f}, speed_{10.0f}, maxHp_{100}, hp_{maxHp_}, gold_{50}, max_speed_{10000.0f}
    //! constructor
    {
    }

private:
    Armor *armor_[10];
    int armor_count_;
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