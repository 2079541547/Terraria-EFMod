//
// Created by EternalFuture on 2025/1/25.
//

#include <Item.hpp>

void Item::initialize() {

    Item_Class = BNM::Class("Terraria", "Item", BNM::Image("Assembly-CSharp.dll"));

    useStyle = Item_Class.GetField("useStyle");
    useAnimation = Item_Class.GetField("useAnimation");
    useTime = Item_Class.GetField("useTime");
    width = Item_Class.GetField("width");
    height = Item_Class.GetField("height");
    shoot = Item_Class.GetField("shoot");
    useAmmo = Item_Class.GetField("useAmmo");
    damage = Item_Class.GetField("damage");
    shootSpeed = Item_Class.GetField("shootSpeed");
    noMelee = Item_Class.GetField("noMelee");
    value = Item_Class.GetField("value");
    ranged = Item_Class.GetField("ranged");
    channel = Item_Class.GetField("channel");
    rare = Item_Class.GetField("rare");
    autoReuse = Item_Class.GetField("autoReuse");
    mana = Item_Class.GetField("mana");
    magic = Item_Class.GetField("magic");
    noUseGraphic = Item_Class.GetField("noUseGraphic");

    DefaultToBow = Item_Class.GetMethod("DefaultToBow");
    SetWeaponValues = Item_Class.GetMethod("SetWeaponValues");
    DefaultToFood = Item_Class.GetMethod("DefaultToFood");
    buyPrice = Item_Class.GetMethod("buyPrice");


    HOOK(Item_Class.GetMethod("SetDefaults3"), SetDefaults3, old_SetDefaults3);
    HOOK(Item_Class.GetMethod("SetDefaults4"), SetDefaults4, old_SetDefaults4);
    HOOK(Item_Class.GetMethod("SetDefaults5"), SetDefaults5, old_SetDefaults5);
    HOOK(Item_Class.GetMethod("SetFoodDefaults"), SetFoodDefaults, old_SetFoodDefaults);
}

void Item::SetDefaults4(BNM::UnityEngine::Object *i, int t) {
    old_SetDefaults4(i, t);
    switch (t) {
        case 3853:
            useStyle[i].Set(5);
            useAnimation[i].Set(25);
            useAmmo[i].Set(40);
            width[i].Set(20);
            height[i].Set(34);
            shoot[i].Set(1);
            damage[i].Set(42);
            shootSpeed[i].Set(50.0f);
            noMelee[i].Set(true);
            value[i].Set(180000);
            ranged[i].Set(true);
            useTime[i].Set(12);
            channel[i].Set(true);
            rare[i].Set(6);
            autoReuse[i].Set(true);
            break;
        case 3850:
            useStyle[i].Set(1);
            shootSpeed[i].Set(55.5f);
            shoot[i].Set(680);
            damage[i].Set(71);
            width[i].Set(0);
            height[i].Set(30);
            useAnimation[i].Set(17);
            useTime[i].Set(17);
            noUseGraphic[i].Set(true);
            noMelee[i].Set(true);
            value[i].Set(114514);
            ranged[i].Set(true);
            break;
        default:
            break;
    }
}


/**
 *
						useStyle = 1;
						shootSpeed = 11.5f;
						shoot = 507;
						damage = 17;
						width = 30;
						height = 30;
						maxStack = Item.CommonMaxStack;
						consumable = true;
						UseSound = SoundID.Item1;
						useAnimation = 24;
						useTime = 24;
						noUseGraphic = true;
						noMelee = true;
						knockBack = 4.75f;
						value = Item.sellPrice(0, 0, 0, 5);
						ranged = true;
 */

void Item::SetDefaults5(BNM::UnityEngine::Object *i, int t) {
    old_SetDefaults5(i, t);
    switch (t) {
        case 4058:
            SetWeaponValues[i](25, 5.0f, 0);
            break;
        case 4722:
            noMelee[i].Set(false);
            damage[i].Set(130);
            break;
        default:
            break;
    }
}

void Item::SetFoodDefaults(BNM::UnityEngine::Object *i, int t) {
    old_SetFoodDefaults(i, t);
    switch (t) {
        case 4010:
            DefaultToFood[i](22, 22, 207, 14400, false, 17);
            rare[i].Set(3);
            value[i].Set(buyPrice[i](0, 0, 70, 5));
            break;
        default:
            break;
    }
}

void Item::SetDefaults3(BNM::UnityEngine::Object *i, int t) {
    old_SetDefaults3(i, t);
    switch (t) {
        case 2881:
            useStyle[i].Set(5);
            useAnimation[i].Set(20);
            useTime[i].Set(80);
            shootSpeed[i].Set(500.0f);
            width[i].Set(20);
            height[i].Set(12);
            damage[i].Set(550);
            shoot[i].Set(79);
            mana[i].Set(100);
            rare[i].Set(8);
            value[i].Set(buyPrice[i](0, 10, 0, 0));
            noMelee[i].Set(true);
            channel[i].Set(true);
            break;
        default:
            break;
    }
}

