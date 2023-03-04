//
//  Monster.cpp
//  KittyTouch
//
//  Created by 王小兵 on 16/2/28.
//
//

#include "Monster.hpp"
#include "GameConstants.h"
#include "GameManager.hpp"
#include <SimpleAudioEngine.h>

using namespace cocos2d;

Monster* Monster::create(MonsterModel* model)
{
    auto monster = Monster::create();
    monster->setPosition(model->bornPosition);
    
    monster->mModel = model;
    
    monster->createSwingAnimation();
    monster->createMonsterAnimation();
    monster->playFallDownAnimation();
    
    return monster;
}

bool Monster::init()
{
    if (!Node::init())
        return false;
    
    return true;
}

void Monster::createSwingAnimation()
{
    mSwingSprite = Sprite::create("ui/swing0.png");
    
    
    mSwingSprite->setScale(8.0f, 8.0f);
    
    auto animation = Animation::create();
    for(auto i = 0; i< 4; ++i)
    {
        char szName[100] = {0};
        sprintf(szName, "ui/swing%0d.png", i);
        
        animation->addSpriteFrameWithFile(szName);
    }
    
    for (auto i = 0; i < animation->getFrames().size(); ++i) {
        auto frame = animation->getFrames().at(i);
        frame->getSpriteFrame()->getTexture()->setAliasTexParameters();
    }
    
    animation->setDelayPerUnit(0.40f / 4.0f);
    animation->setRestoreOriginalFrame(true);
    auto action = Animate::create(animation);
    mSwingSprite->runAction(RepeatForever::create(action));
    mSwingSprite->setPosition(Vec2(0.0f, 124.0f));
    addChild(mSwingSprite);
}

void Monster::createMonsterAnimation()
{
    auto monsterSprName = "monster/flyDoge0.png";
    auto monsterSprStr = "monster/flyDoge%0d.png";
    auto hasAnim = false;
    
    switch (mModel->monsterType)
    {
        case Kitty:
            hasAnim = true;
            monsterSprName = "monster/flyCat0.png";
            monsterSprStr = "monster/flyCat%0d.png";
            break;
            
        case Chick:
            hasAnim = true;
            monsterSprName = "monster/chick0.png";
            monsterSprStr = "monster/chick%0d.png";
            break;

        case Bear:
            hasAnim = true;
            monsterSprName = "monster/flyBear0.png";
            monsterSprStr = "monster/flyBear%0d.png";
            break;
        case Rabbit:
            hasAnim = true;
            monsterSprName = "monster/rabbit0.png";
            monsterSprStr = "monster/rabbit%0d.png";

            break;
            
            
        case Gorilla:
            monsterSprName = "monster/gorilla0.png";

            break;
        case Lion:
            monsterSprName = "monster/lion0.png";

            break;
        case Hippo:
            monsterSprName = "monster/hippo0.png";

            break;
        default:
            break;
    }
    
    mMonsterSprite = Sprite::create(monsterSprName);
    mMonsterSprite->getTexture()->setAliasTexParameters();
    
    mMonsterSprite->setScale(10.0f, 10.0f);
    
    if (hasAnim) {
        auto animation = Animation::create();
        for(auto i = 0; i< 2; ++i)
        {
            char szName[100] = {0};
            sprintf(szName, monsterSprStr, i);
            
            animation->addSpriteFrameWithFile(szName);
        }
        
        for (auto i = 0; i < animation->getFrames().size(); ++i) {
            auto frame = animation->getFrames().at(i);
            frame->getSpriteFrame()->getTexture()->setAliasTexParameters();
        }
        
        animation->setDelayPerUnit(2.0f / 2.0f);
        animation->setRestoreOriginalFrame(true);
        auto action = Animate::create(animation);
        mMonsterSprite->runAction(RepeatForever::create(action));
    }
    
    addChild(mMonsterSprite);
    
    // create gesture
    std::string res = std::string("gestures/") + ConvertToString(mModel->gesture) + ".png";
    mGesture = Sprite::create(res);
    mGesture->setPosition(Vec2(0.0f, 200.0f));
    
    addChild(mGesture);

    // create dead animation
    mArmatureDead = cocostudio::Armature::create("guaiwu_texiao01");
    mArmatureDead->setVisible(false);
    mArmatureDead->getAnimation()->stop();
    addChild(mArmatureDead);
}


void Monster::playFallDownAnimation()
{
    auto action = MoveBy::create(0.1f, Vec2(0.0f, GameManager::shareManager()->getMonsterFallDownSpeed()));
    auto falldown = RepeatForever::create(action);
    falldown->setTag(2222);
    runAction(falldown);
}

void Monster::playerMainSceneAnimation()
{
    mGesture->setVisible(false);
    stopActionByTag(2222);
    auto action = MoveBy::create(1.0f, Vec2(0.0f, -20.0f));
    auto sequence = Sequence::create(action, MoveBy::create(1.0f, Vec2(0.0f, 20.0f)), NULL);
    runAction(RepeatForever::create(sequence));
}

void Monster::dead()
{
    GameManager::shareManager()->removeMonster(this);
    mSwingSprite->setVisible(false);
    mMonsterSprite->setVisible(false);
    mGesture->setVisible(false);
    
    mArmatureDead->setVisible(true);
    mArmatureDead->getAnimation()->stop();
    mArmatureDead->getAnimation()->play("siwang", -1, false);
    
    auto action0 = DelayTime::create(1.0f);
    
    
    auto callback = CallFunc::create([&]
                                     {
                                         this->removeFromParent();
                                     });
    
    auto action1 = RepeatForever::create(Sequence::create(action0, callback, NULL));

    runAction(action1);
    
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("music/5313.mp3");
    
}
bool Monster::landed()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    if ((getPosition().y - mMonsterSprite->getTexture()->getContentSize().height * 0.5f * 10.0f) <= visibleSize.height * 0.095f)
        return true;
    return false;
}

void Monster::update(float dt)
{
    
}