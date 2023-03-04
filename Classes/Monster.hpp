//
//  Monster.hpp
//  KittyTouch
//
//  Created by 王小兵 on 16/2/28.
//
//

#ifndef Monster_hpp
#define Monster_hpp

#include "cocos2d.h"
#include "cocostudio/CCArmature.h"

enum MonsterType{
    Doge = 0,
    Kitty = 1,
    Bear = 2,
    Chick = 3,
    Rabbit = 4,
    Gorilla = 5,
    Lion = 6,
    Hippo = 7,
};

class MonsterModel
{
public:
    long          gesture;
    MonsterType   monsterType;
    cocos2d::Vec2 bornPosition;
};

class Monster: public cocos2d::Node {
    
public:
    CREATE_FUNC(Monster);
    
    static Monster* create(MonsterModel* model);
    
    virtual bool init() override;
    void dead();
    bool landed();
    
    ~Monster()
    {
        delete mModel;
    }
    
    long getGesture() { return mModel->gesture; }
    
    void playerMainSceneAnimation();
    
private:
    
    void createSwingAnimation();
    void createMonsterAnimation();
    void playFallDownAnimation();
    void update(float dt) override;
    
    cocos2d::Sprite* mMonsterSprite;
    cocos2d::Sprite* mSwingSprite;
    cocostudio::Armature* mArmatureDead;
    MonsterModel* mModel;
    cocos2d::Sprite* mGesture;
    
};

#endif /* Monster_hpp */
