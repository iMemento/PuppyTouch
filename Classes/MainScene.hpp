//
//  MainScene.hpp
//  KittyTouch
//
//  Created by 王小兵 on 16/4/17.
//
//

#ifndef MainScene_hpp
#define MainScene_hpp

#include "cocos2d.h"
#include "cocostudio/CCArmature.h"

class MainScene : public cocos2d::LayerColor
{
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    CREATE_FUNC(MainScene);
    
    ~MainScene();
    
private:
    void createBackground();
    Node* createSwingAnimation();
    void startGame(Ref* sender);
    void checkRank(Ref* sender);
};
#endif /* MainScene_hpp */
