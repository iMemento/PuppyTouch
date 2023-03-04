//
//  GameScene.hpp
//  KingsFingers
//
//  Created by 王小兵 on 15/10/24.
//
//

#ifndef GameScene_h
#define GameScene_h

#include "cocos2d.h"
#include "cocostudio/CCArmature.h"
#include "Monster.hpp"
#include "GameNumber.hpp"

class GameScene : public cocos2d::LayerColor
{
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();

    CREATE_FUNC(GameScene);
    
    ~GameScene();

private:
    
    void startGame();
    
    void update(float dt);
    
    void createBackground();
    
    void createPlayerRole();
    
    void createMonster(MonsterModel* model);
    
    void hitMonster(float x);
    
    void clickCallback(int kind, cocos2d::Point* pos){};
    
    void gestureCallback(int kind);

    void createScoreboardUI();
    
    void restartGame(Ref* sender);
    void sharePost(Ref* sender);
    void showCombo(int num);
    void addEventListener();
    
    cocos2d::Sprite* mPrincess;
    
    GameNumber* mScore;
    cocos2d::Node* mGameOverUI;
    cocos2d::Node* mComboNode;
    GameNumber* mComboNumber;
    
};

#endif /* GameScene_h */
