#include <cstdio>
#include <string>

#define clamp(a, b, c) std::min((c), std::max((a), (b)))

struct Player
{
public:
    void changeLife(int change);
    bool changeGold(int change);
    bool changeSpeed(int change);
    void printPlayer();

    std::string name;

private:
    float x_ = 0.0f;
    float y_ = 0.0f;
    float speed_ = 5.0f;
    float max_speed_ = 10000.0f;
    int hp_ = 100;
    int maxHp_ = 100;
    int gold_ = 50;
};

void Player::changeLife(int change)
{
    hp_ = clamp(0, hp_ + change, maxHp_);
}

bool Player::changeGold(int change)
{
    if (hp_ == 0)
        return false;
    if (change < 0 && -change > gold_)
        return false;
    gold_ = std::max(0, gold_ + change);
    return true;
}

bool Player::changeSpeed(int change)
{
    if (hp_ == 0)
        return false;
    if (speed_ < 0 || speed_ > max_speed_)
        return false;
    speed_ = clamp(0.0f, speed_ * change, max_speed_);
    return true;
}

void buyPotion(Player &p)
{
    if (p.changeGold(-30))
    {
        p.changeLife(40);
    }
}

void fallInLava(Player &p)
{
    p.changeLife(-150);
}

void buyArmor(Player &p)
{
    if (p.changeGold(-300))
    {
        p.changeLife(0);
    }
}

void pickUpBoots(Player &p)
{
    p.changeSpeed(10);
}
// the idea its that printPlayer should not be inside the class Player , con herencia dentro de un tiempo lo podremos hacer (andreu)
void Player::printPlayer()
{
    printf("%s | pos(%g, %g) | hp %d/%d | gold %d | speed %g\n",
           name.c_str(), x_, y_, hp_, maxHp_, gold_, speed_);
}

int main()
{
    Player hero;
    hero.name = "Aria";
    hero.printPlayer();
    buyPotion(hero);
    hero.printPlayer();

    buyPotion(hero);
    hero.printPlayer();

    pickUpBoots(hero);
    hero.printPlayer();
    pickUpBoots(hero);
    hero.printPlayer();

    fallInLava(hero);
    fallInLava(hero);
    hero.printPlayer();

    // hero.x_ += hero.speed_;
    // hero.name = "Ghost";
    // hero.maxHp_ = -5;
    hero.printPlayer();

    return 0;
}
