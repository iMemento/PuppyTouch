//
//  GameNumber.cpp
//  KingsFingers
//
//  Created by 王小兵 on 15/11/28.
//
//

#include "GameNumber.hpp"
#include "GameConstants.h"

using namespace cocos2d;

GameNumber* GameNumber::create(int nubmer, bool big)
{
    GameNumber *ret = new (std::nothrow) GameNumber();
    ret->mNumber = nubmer;
    ret->mBig = big;
    
    if (ret && ret->init())
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;

}

bool GameNumber::init()
{
    setNumber(mNumber, mBig);
    setScale(mBig ? 10 : 4);
    return true;
}

void GameNumber::setNumber(int number, bool big)
{
    removeAllChildren();
    mBig = big;
    mNumber = number;
    std::string str = ConvertToString(number);
    auto pos = Vec2((str.length() -1) * (big ? 8.0f : 5.0f) * (-0.5f), 0.0f);
    for (auto i = 0; i < str.length(); ++i)
    {
        auto number = str[i];
        
        auto sprite = Sprite::create((big ? "numbers/L" : "numbers/S") + ConvertToString(number) + ".png");
        sprite->getTexture()->setAliasTexParameters();
        
        addChild(sprite);
        sprite->setPosition(pos);
        printf("(%f,%f)\n",pos.x, pos.y);
        pos.x += big ? 8.0f : 5.0f;
    }
}