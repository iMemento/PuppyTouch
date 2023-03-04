//
//  GameManager.hpp
//  KittyTouch
//
//  Created by 王小兵 on 16/3/19.
//
//

#ifndef GameManager_hpp
#define GameManager_hpp

#include <stdio.h>
#include "Monster.hpp"
class GameManager
{
public:
    static GameManager* shareManager();
    
    long getCurrentSocre() { return mCurrentScore; }
    
    long getBestScore(){ return cocos2d::UserDefault::getInstance()->getIntegerForKey("BestScore", 0); }
    
    void setBestScore(long value)
    {
        cocos2d::UserDefault::getInstance()->setIntegerForKey("BestScore", value);
        cocos2d::UserDefault::getInstance()->flush();
    }
    
    void startGame();
    
    void update(float dt);
    
    void gameOver();
    
    void addScore(int score);
    
    void recognizeGesture(int kind);
    
    void restartGame();
    
    void addMonster(Monster* monster){ mMonsters.push_back(monster); }
    
    void removeMonster(Monster* monster)
    {
        for (auto iter = mMonsters.begin(); iter != mMonsters.end();)
        {
            if (*iter == monster)
            {
                iter = mMonsters.erase(iter);
            }
            else
            {
                ++iter;
            }
        }
    }
    
    
    std::vector<Monster*> getAllMonsters()
    {
        return mMonsters;
    }
    
    cocos2d::Vec2 getRandormPos();
    
    float getMonsterFallDownSpeed(){ return mMonsterFallDownSpeed; };
    
private:
    GameManager();
    ~GameManager();
    
    void                    createMonster(long monsterType, long gesture, cocos2d::Vec2 pos);
    
    static GameManager*     sInstance;
    long                    mCurrentScore;
    std::vector<Monster*>   mMonsters;
    bool                    mGameOver = false;
    float                   mTimer = 0.0f;
    float                   mCurrentMonsterX = 0.0f;
    float                   mInterval = 2.0f;
    float                   mLastBornPosX = 0.0f;
    int                     mCombo = 0;
    
    float                   mMonsterFallDownSpeed;
};
#endif /* GameManager_hpp */
