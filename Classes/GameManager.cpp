//
//  GameManager.cpp
//  KittyTouch
//
//  Created by 王小兵 on 16/3/19.
//
//

#include "GameManager.hpp"
#include "GameConstants.h"
USING_NS_CC;

GameManager* GameManager::sInstance = NULL;

GameManager* GameManager::shareManager()
{
    if (sInstance == NULL)
    {
        sInstance = new GameManager();
    }
    return sInstance;
}

GameManager::GameManager()
{
    
}

GameManager::~GameManager()
{
    
}

void GameManager::startGame()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    mCurrentScore = 0;
    mGameOver = false;
    mTimer = 0.0f;
    mInterval = 2.0f;
    mLastBornPosX = visibleSize.width / 2.0f;
    mMonsterFallDownSpeed = -24.0f;
    
    mMonsters.clear();
    
    auto  pos = Vec2(random(visibleSize.width / 2 - 400, visibleSize.width / 2 + 400), GameConstants::SCENE_HEIGHT);
    createMonster(0, 2, pos);
}


void GameManager::update(float dt)
{
    if (mGameOver) return;
    
    auto count = random(1, 2);
    
    if (mCurrentScore > 30 && mCurrentScore < 90)
    {
        mMonsterFallDownSpeed = -28.0f;
        mInterval = 1.8f;
        
        auto probability = random(0.0f, 1.0f);
        if (probability < 0.05f)
            count = 2;
    }
    else if (mCurrentScore < 200)
    {
        mMonsterFallDownSpeed = -30.0f;
        mInterval = 1.5f;
        
        auto probability = random(0.0f, 1.0f);
        if (probability < 0.1f)
            count = random(2, 3);
    }
    else if (mCurrentScore < 300)
    {
        mMonsterFallDownSpeed = -35.0f;
        mInterval = 1.2f;
        
        auto probability = random(0.0f, 1.0f);
        if (probability < 0.12f)
            random(2, 3);
    }
    else if (mCurrentScore < 500)
    {
        mMonsterFallDownSpeed = -40.0f;
        mInterval = 1.0f;
        auto probability = random(0.0f, 1.0f);
        if (probability < 0.14f)
            random(2, 3);
    }
    else if (mCurrentScore < 1000)
    {
        mMonsterFallDownSpeed = -45.0f;
        mInterval = 0.8f;
        auto probability = random(0.0f, 1.0f);
        if (probability < 0.15f)
            count = 3;
    }
    else
    {
        mMonsterFallDownSpeed = -55.0f;
        mInterval = 0.6f;
        auto probability = random(0.0f, 1.0f);
        if (probability < 0.16f)
            count = 3;
    }
    
    mTimer += dt;
    
    if (mTimer > mInterval)
    {
        mTimer = 0.0f;
        
        for (auto i = 0; i < count; ++i)
        {
            auto monster = random(0, 7);
            
            auto gesture = random(1, 8);
            
            if (mCurrentScore > 30 && mCurrentScore < 90)
            {
                auto probability = random(0.0f, 1.0f);
                if (probability < 0.1f)
                {
                    gesture = random(9, 10);
                }
            }
            else if (mCurrentScore < 200)
            {
                auto probability = random(0.0f, 1.0f);
                {
                    gesture = random(1, 8);
                }
                
                if (probability < 0.1f)
                {
                    gesture = random(9, 10);
                }
            }
            else if (mCurrentScore < 300)
            {
                auto probability = random(0.0f, 1.0f);
                {
                    gesture = random(1, 8);
                }
                
                if (probability < 0.12f)
                {
                    gesture = random(9, 10);
                }
            }
            else if (mCurrentScore < 500)
            {
                auto probability = random(0.0f, 1.0f);
                {
                    gesture = random(1, 8);
                }
                
                if (probability < 0.15f)
                {
                    gesture = random(9, 10);
                }

            }
            else if (mCurrentScore < 1000)
            {
                auto probability = random(0.0f, 1.0f);
                {
                    gesture = random(1, 8);
                }
                
                if (probability < 0.2f)
                {
                    gesture = random(9, 10);
                }
                
                if (probability < 0.1f)
                {
                    gesture = 13;
                }

            }
            else
            {
                auto probability = random(0.0f, 1.0f);

                {
                    gesture = random(1, 8);
                }
                
                if (probability < 0.2f)
                {
                    gesture = random(9, 10);
                }
                
                if (probability < 0.1f)
                {
                    gesture = 13;
                }

            }

            auto bornPosition = getRandormPos();
            createMonster(monster, gesture, bornPosition);
            mLastBornPosX = bornPosition.x;
        }
        
    }
    
    for (auto iter = mMonsters.begin(); iter != mMonsters.end(); ++iter)
    {
        if (*iter == NULL) continue;
        
        if ((*iter)->landed())
        {
            gameOver();
            break;
        }
    }
}

Vec2 GameManager::getRandormPos()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto bornPosition = Vec2(0.0f, GameConstants::SCENE_HEIGHT);
    
    if (mLastBornPosX > visibleSize.width / 2)
    {
        bornPosition.x = random(200.0f, mLastBornPosX - 200.0f);
    }
    else
    {
        bornPosition.x = random(mLastBornPosX + 200.0f, visibleSize.width - 200.0f);
    }
    return bornPosition;
}

void GameManager::gameOver()
{
    mGameOver = true;
    for(auto iter = mMonsters.begin(); iter != mMonsters.end(); ++iter)
    {
        (*iter)->stopAllActions();
    }
    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("Game_Over", NULL);
}

void GameManager::addScore(int score)
{
    mCurrentScore += score;
    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("Refresh_Score", NULL);
}

void GameManager::restartGame()
{
    for (auto iter = mMonsters.begin(); iter != mMonsters.end(); ++iter)
    {
        (*iter)->removeFromParent();
    }
    
    mMonsters.clear();
    startGame();
    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("Refresh_Score", NULL);
}


void GameManager::recognizeGesture(int kind)
{
    std::vector<Monster*> hitMonsters;
    
    for (auto iter = mMonsters.begin(); iter != mMonsters.end();  ++iter)
    {
        if (*iter == NULL) continue;
        
        if ((*iter)->getGesture() == kind)
        {
            hitMonsters.push_back((*iter));
        }
    }

    if (hitMonsters.size() > 1)
    {
        mCombo = hitMonsters.size();
        Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("Show_Combo", &mCombo);
    }
    
    for (auto iter = hitMonsters.begin(); iter != hitMonsters.end(); ++iter)
    {
        mCurrentMonsterX = (*iter)->getPosition().x;
        Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("Hit_Monster", &mCurrentMonsterX);
        (*iter)->dead();
        addScore(1);
    }
}

void GameManager::createMonster(long monsterType, long gesture, cocos2d::Vec2 pos)
{
    auto model = new MonsterModel();
    model->monsterType = (MonsterType)monsterType;
    model->gesture = gesture;
    model->bornPosition = pos;
    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("New_Monster_Born", model);
}
