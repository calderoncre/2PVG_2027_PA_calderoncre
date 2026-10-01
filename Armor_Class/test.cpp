#include "Player.hpp"

int main()
{
    Player hero;
    Armor *armor_type = new Armor(200, 0.5f);
    Armor *casco = new Armor(50, 0.1f);
    Armor *calcetines = new Armor(75, 0.4f);

    hero.printPlayer();

    int precioArmadura = 200;
    if (hero.spendMoney(precioArmadura))
    {
        // equipar armadura
    }
    else
    {
        printf("estas pelao! \n");
    }

    printf("potions twice! \n");
    buyPotion(hero);
    hero.printPlayer();
    buyPotion(hero);
    hero.printPlayer();
    printf("\n_________ \n");

    printf("boots twice \n");
    pickUpBoots(hero);
    hero.printPlayer();
    pickUpBoots(hero);
    hero.printPlayer();
    printf("\n ________\n");

    printf("equip armor \n");
    hero.printCurrentArmor();
    hero.equipArmor(*armor_type);
    hero.printCurrentArmor();
    hero.equipArmor(*armor_type);
    hero.printCurrentArmor();
    hero.equipArmor(*armor_type);
    hero.printCurrentArmor();
    hero.equipArmor(*armor_type);
    hero.printCurrentArmor();
    hero.equipArmor(*armor_type);
    hero.printCurrentArmor();
    hero.equipArmor(*armor_type);
    hero.printCurrentArmor();
    hero.equipArmor(*armor_type);
    hero.printCurrentArmor();
    hero.equipArmor(*armor_type);
    hero.printCurrentArmor();
    hero.equipArmor(*armor_type);
    hero.printCurrentArmor();

    printf("unequip armor \n");
    hero.printAllArmorHP();
    hero.printCurrentArmor();
    hero.unequipArmor(3);

    hero.printCurrentArmor();
    printf("AFTER REORDER: \n");
    hero.reorderArmorList();
    hero.printCurrentArmor();

    printf("fall in lava twice \n");
    fallInLava(hero);
    hero.printPlayer();
    fallInLava(hero);
    hero.printPlayer();

    hero.printCurrentArmor();
    hero.printAllArmorHP();

    return 0;
}
