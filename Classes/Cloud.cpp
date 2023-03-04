//
//  Cloud.cpp
//  KittyTouch
//
//  Created by 王小兵 on 16/2/25.
//
//

#include "Cloud.hpp"
using namespace cocos2d;

bool Cloud::init()
{
    if (!cocos2d::Node::init())
        return false;
    
    mCloudSprite = cocos2d::Sprite::create("ui/cloud.png");
    
    mCloudSprite->setScale(5.0f, 5.0f);
    mCloudSprite->getTexture()->setAliasTexParameters();
    
    addChild(mCloudSprite);
    
    auto action0 = MoveBy::create(0.1f, Vec2(-10.0f, 0.0f));

    
    auto callback = CallFunc::create([&]
    {
        auto pos = getPosition();
        
        if (pos.x < -200.0f)
        {
            auto visibleSize = Director::getInstance()->getVisibleSize();
            setPosition(Vec2(visibleSize.width + 200.0f, pos.y));
        }
    });
    
    auto action = RepeatForever::create(Sequence::create(action0, callback, NULL));
    runAction(action);
    
    return true;
}