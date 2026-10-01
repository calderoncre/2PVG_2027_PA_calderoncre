#include "Player.hpp"
#include <cassert>

void Player::changeLife(int change)
{
    if (hp_ <= 0)
        return;
    hp_ = clamp(maxHp_, hp_ + change, 0);
}

bool Player::spendMoney(int change)
{
    if (gold_ - change < 0)
        return false;
    gold_ -= change;
    return true;
}

void buyPotion(Player &p)
{
    if (p.spendMoney(30))
    {
        p.changeLife(40);
    }
    else
    {
        printf("estas pelao \n");
    }
}

void fallInLava(Player &p)
{
    p.takeDamage(50);
}

void pickUpBoots(Player &p)
{
    if (p.changeSpeed(10))
        printf("speed cambiada correctamente \n");
    else
        printf("speed cambiada maaaal \n");
}

void Player::equipArmor(const Armor &armor_type) // tipo
{
    int max_slots = 10;
    int slot = 0;
    for (slot; slot < max_slots; slot++)
    {
        if (armor_[slot] == nullptr)
            break;
    }
    if (slot >= 10)
        return;
    armor_[slot] = new Armor{armor_type};
    if (armor_[slot] != nullptr)
        printf("armor asignada a slot \n");
    else
        printf("armor ha fallado asignando slot\n");

    // if (armor_ == nullptr) // first armor to equip:
    // {
    //     armor_ = (Armor **)malloc(sizeof(Armor *) * armor_count_ + 1); // one armor
    //     armor_[0] = new Armor{armor_type};
    //     armor_count_++;
    // }
    // else
    // {
    //     int max_slots = 10;
    //     int slot = 0;
    //     for (int slot = 0; slot < max_slots; slot++)
    //     {
    //         if (armor_[slot] == nullptr)
    //             break;
    //     }
    //     if (slot > 10)
    //         return;

    //     //! IDEA
    // crear nuevo arary armor_count + 1
    // ! cuando agregue una nueva armadura
    // copiar los punteros del array atiguo
    // liberar el array antiguo
    // hacer armor_ = array nuevo
    // crear la nuevo armor en su ultima posicion del array
    // armor_count ++

    //     armor_[slot] = new Armor{armor_type};
    //     armor_count_++;
    //     if (armor_[slot] != nullptr)
    //         printf("ha funcionado");
    //     else
    //         printf("no ha funcionado");
    //     // funcion free anrmadura individual
    // }
}

void Player::takeDamage(int damage)
{
    int reducedDamage = damage;
    for (int i = 0; i < 10; i++)
    {
        if (armor_[i] == nullptr)
            continue;
        reducedDamage = armor_[i]->applyDamageReduction(reducedDamage);
        if (armor_[i]->isDestroyed())
        {
            // free single armor
            delete armor_[i];
            armor_[i] = nullptr;
        }
    }
    changeLife(-reducedDamage);
}

void Player::unequipArmor(int slot)
{
    assert(slot >= 0 && "problema en armor slot");
    assert(slot < 10 && "problema en armor slot");
    if (armor_[slot] == nullptr)
        return;
    delete armor_[slot];
    armor_[slot] = nullptr;
}

bool Player::changeSpeed(int change)
{
    if (hp_ == 0)
        return false;
    speed_ = clamp(max_speed_, speed_ * change, 0.0f);
    return true;
}
void Player::printCurrentArmor()
{
    int max_slots = 10;
    int slot = 0;
    for (int slot = 0; slot < max_slots; slot++)
    {
        printf("slot %d ---> %p \n", slot, (void *)armor_[slot]);
    }
    printf("\n ");
}
void Player::printAllArmorHP() const
{
    for (int i = 0; i < armor_max_count_; i++)
    {
        if (armor_[i] == nullptr)
            return;
        printf("%d: %d hp // ", i, armor_[i]->getHp());
    }
    printf("\n ");
}
void Player::printPlayer()
{
    printf("pos(%g, %g) | hp %d/%d | gold %d | speed %g\n", x_, y_, hp_, maxHp_, gold_, speed_);
}
