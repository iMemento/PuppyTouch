//
//  GameScene.cpp
//  KingsFingers
//
//  Created by 王小兵 on 15/10/24.
//
//

#include "GameScene.h"
#include "cocostudio/CCArmature.h"
#include "CTouchManager.h"
#include "Cloud.hpp"
#include "Monster.hpp"
#include "GameManager.hpp"
#include "GameConstants.h"
#include "MainScene.hpp"
#include "GameCenterBridge.h"
#include <SimpleAudioEngine.h>

USING_NS_CC;
using namespace ui;

struct Constants{
public:
    static const int touchLayerTag = 10086;
    static const int guideTag = 10087;
    static const int princessMoveActionTag = 10088;
};

Scene* GameScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // add touch
    auto touch = CTouchManager::create(layer, Clickcallback_selector(GameScene::clickCallback),
                                       Gesturecallback_selector(GameScene::gestureCallback));
    touch->setTag(Constants::touchLayerTag);
    
    scene->addChild(touch);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    // super init first
    if ( !LayerColor::initWithColor(Color4B(136, 221, 238, 255)) )
    {
        return false;
    }
    
    addEventListener();
    
    createBackground();
    
    createPlayerRole();
    
    auto delay = DelayTime::create(0.5f);
    auto callback = CallFunc::create([&]
     {
         this->startGame();
         CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("music/bgm.m4a", true);
     });

    runAction(Sequence::create(delay, callback, NULL));
    return true;
}


void GameScene::createBackground()
{
    cocostudio::ArmatureDataManager::getInstance()->addArmatureFileInfo("effect/guaiwu_texiao01.ExportJson");
    cocostudio::ArmatureDataManager::getInstance()->addArmatureFileInfo("effect/xinshouyindao.ExportJson");
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    auto ground = LayerColor::create(Color4B(205, 202, 160, 255));
    ground->setPosition(0.0f, 0.0f);
    ground->setContentSize(Size(visibleSize.width, visibleSize.height * 0.1f));
    addChild(ground, GameZOrder::BG_ZORDER);
    
    auto grass = Sprite::create("ui/grass.png");
    grass->setPosition(visibleSize.width * 0.5f, visibleSize.height * 0.1f);
    grass->setScale(10.0f, 10.0f);
    grass->setAnchorPoint(Vec2(0.5f, 0.0f));
    grass->getTexture()->setAliasTexParameters();
    addChild(grass, GameZOrder::BG_ZORDER);
    
    ground = LayerColor::create(Color4B(104, 80, 29, 255));
    ground->setPosition(0.0f, visibleSize.height * 0.095f);
    ground->setContentSize(Size(visibleSize.width, 10.0f));
    addChild(ground, GameZOrder::BG_ZORDER);
    
    grass = Sprite::create("ui/ground.png");
    grass->setPosition(visibleSize.width * 0.5f, visibleSize.height * 0.1f);
    grass->setScale(10.0f, 10.0f);
    grass->setAnchorPoint(Vec2(0.5f, 0.0f));
    grass->getTexture()->setAliasTexParameters();
    addChild(grass, GameZOrder::BG_ZORDER);
    
    // create cloud
    auto cloud = Cloud::create();
    cloud->setPosition(visibleSize.width * 0.4f, visibleSize.height * 0.9f);
    addChild(cloud, GameZOrder::BG_ZORDER);
    
    auto cloud1 = Cloud::create();
    cloud1->setPosition(visibleSize.width * 0.6f, visibleSize.height * 0.7f);
    addChild(cloud1, GameZOrder::BG_ZORDER);
    
    auto cloud2 = Cloud::create();
    cloud2->setPosition(visibleSize.width * 0.8f, visibleSize.height * 0.8f);
    addChild(cloud2, GameZOrder::BG_ZORDER);
    
    // create score
    mScore = GameNumber::create(0);
    mScore->setPosition(visibleSize.width * 0.5f, visibleSize.height * 0.7f);
    addChild(mScore, GameZOrder::SUMMARIZE_UI_ZORDER);
    
    // gamover ui
    mGameOverUI = Node::create();
    mGameOverUI->setPosition(visibleSize.width * 0.5f, visibleSize.height * 0.5f);
    addChild(mGameOverUI, GameZOrder::SUMMARIZE_UI_ZORDER);
    
    // guide
    auto guide = cocostudio::Armature::create("xinshouyindao");
    guide->getAnimation()->playWithIndex(0);
    guide->setPosition(visibleSize.width * 0.5f, visibleSize.height * 0.5f);
    guide->setTag(Constants::guideTag);
    addChild(guide, GameZOrder::PLAYER_ZORDER);
    
    // combo
    mComboNode = Node::create();
    mComboNode->setPosition(visibleSize.width * 0.5f - 20, visibleSize.height * 0.6f);
    addChild(mComboNode, GameZOrder::SUMMARIZE_UI_ZORDER);
    auto spr = Sprite::create("ui/COMBO.png");
    spr->getTexture()->setAliasTexParameters();
    spr->setScale(5, 5);
    
    mComboNumber = GameNumber::create(3);
    mComboNumber->setScale(7);
    mComboNumber->setPosition(200, 0.0f);
    mComboNode->addChild(mComboNumber);
    mComboNode->addChild(spr);
    mComboNode->setVisible(false);
}

void GameScene::createPlayerRole()
{
    // create princess
    auto visibleSize = Director::getInstance()->getVisibleSize();
    mPrincess = Sprite::create("ui/princess0.png");
    mPrincess->setPosition(visibleSize.width * 0.5f, visibleSize.height * 0.1f);
    mPrincess->setScale(15.0f, 15.0f);
    mPrincess->setAnchorPoint(Vec2(0.5f, 0.0f));
    mPrincess->getTexture()->setAliasTexParameters();
    
    
    auto animation = Animation::create();
    auto path = "ui/princess%d.png";
    
    for(auto i = 0; i< 4; ++i)
    {
        char szName[100] = {0};
        sprintf(szName, path, i);
        
        animation->addSpriteFrameWithFile(szName);
    }
    
    for (auto i = 0; i < animation->getFrames().size(); ++i)
    {
        auto frame = animation->getFrames().at(i);
        frame->getSpriteFrame()->getTexture()->setAliasTexParameters();
    }
    
    animation->setDelayPerUnit(1.0f / 10.0f);
    animation->setRestoreOriginalFrame(true);
    auto action = Animate::create(animation);
    mPrincess->runAction(RepeatForever::create(Sequence::create(action, DelayTime::create(3.0f), NULL)));

    
    addChild(mPrincess, GameZOrder::PLAYER_ZORDER);
}


void GameScene::startGame()
{
    GameManager::shareManager()->startGame();
    this->scheduleUpdate();
}

void GameScene::createMonster(MonsterModel* model)
{
    auto monster = Monster::create(model);
    addChild(monster, GameZOrder::MONSTER_ZORDER);
    GameManager::shareManager()->addMonster(monster);
}

void GameScene::hitMonster(float x)
{
    mPrincess->stopActionByTag(Constants::princessMoveActionTag);
    auto diatance = x - mPrincess->getPosition().x;
    auto action = MoveBy::create(0.3f, Vec2(diatance, 0.0f));
    action->setTag(Constants::princessMoveActionTag);
    mPrincess->runAction(action);
}


void GameScene::update(float dt)
{
    GameManager::shareManager()->update(dt);
}

void GameScene::gestureCallback(int kind)
{
    printf("type is : %d", kind);
    GameManager::shareManager()->recognizeGesture(kind);
    if (kind == 2)
    {
        removeChildByTag(Constants::guideTag);
    }
}

void GameScene::createScoreboardUI()
{
    // gameover title
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto gameover = Sprite::create("ui/GameOver.png");
    gameover->getTexture()->setAliasTexParameters();
    gameover->setPosition(0.0f, visibleSize.height * 0.6f);
    gameover->setScale(9.0f, 9.0f);
    mGameOverUI->addChild(gameover);
    auto action = MoveBy::create(0.3f, Vec2(0.0f, -550.0f));
    gameover->runAction(action);
    
    // score board
    auto board = Sprite::create("ui/ScoreBoard.png");
    board->getTexture()->setAliasTexParameters();
    board->setPosition(0.0f, -visibleSize.height * 0.7f);
    board->setScale(2.40f, 2.40f);
    mGameOverUI->addChild(board);
    action = MoveBy::create(0.3f, Vec2(0.0f, 1400.0f));
    auto num = GameNumber::create(GameManager::shareManager()->getCurrentSocre(), false);
    auto size = board->getTexture()->getContentSize();
    num->setPosition(size.width * 0.8f, size.height * 0.55f);
    board->addChild(num);
    
    num = GameNumber::create(GameManager::shareManager()->getBestScore(), false);
    num->setPosition(size.width * 0.8f, size.height * 0.2f);
    board->addChild(num);
    
    board->runAction(Sequence::create(DelayTime::create(0.3f), action, NULL));
    
    // buttons
    auto node = Node::create();
    node->setPosition(0.0f, -visibleSize.height * 0.6f);
    mGameOverUI->addChild(node);
    
    // restart button
    auto resartButton = MenuItemImage::create("ui/returnButton.png", "ui/returnButton.png",
                                          CC_CALLBACK_1(GameScene::restartGame, this));
    resartButton->getSelectedImage()->setScale(0.9f);
    
    // share button
    auto shareButton = MenuItemImage::create("ui/shareButton.png", "ui/shareButton.png",
                                              CC_CALLBACK_1(GameScene::sharePost, this));

    shareButton->getSelectedImage()->setScale(0.9f);
    
    
    resartButton->setPosition(-visibleSize.width * 0.23, 0.0f);
    shareButton->setPosition(visibleSize.width * 0.23f, 0.0f);
    
    auto starMenu = Menu::create(resartButton, shareButton, NULL);
    starMenu->setPosition(Point::ZERO);
    node->addChild(starMenu, 1);
    
    resartButton->setScale(2.0f);
    shareButton->setScale(2.0f);
    
    action = MoveBy::create(0.3f, Vec2(0.0f, 840.0f));
    node->runAction(Sequence::create(DelayTime::create(0.6f), action, NULL));


}

void GameScene::addEventListener()
{
    // add monster born listener
    getEventDispatcher()->addCustomEventListener("New_Monster_Born", [this](EventCustom* event)
     {
         createMonster((MonsterModel*)event->getUserData());
     });
    
    // add monster attacked listener
    getEventDispatcher()->addCustomEventListener("Hit_Monster", [this](EventCustom* event)
     {
         auto x = *(float*)event->getUserData();
         hitMonster(x);
     });
    
    // add monster attacked listener
    getEventDispatcher()->addCustomEventListener("Show_Combo", [this](EventCustom* event)
     {
         auto x = *(int*)event->getUserData();
         showCombo(x);
     });

    
    getEventDispatcher()->addCustomEventListener("Refresh_Score", [this](EventCustom* event)
     {
         mScore->setNumber(GameManager::shareManager()->getCurrentSocre());
     });
    
    getEventDispatcher()->addCustomEventListener("Game_Over", [this](EventCustom* event)
     {
         CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("music/deploy_timer_ding_03.wav");
         auto currentScore = GameManager::shareManager()->getCurrentSocre();
         if (currentScore > GameManager::shareManager()->getBestScore())
         {
             GameManager::shareManager()->setBestScore(currentScore);
             GameCenterBridge::postHighScore("PuppyTouchBestScore", currentScore);
         }
         
         CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
         
         createScoreboardUI();
         this->getScene()->removeChildByTag(Constants::touchLayerTag);
     });

}

GameScene::~GameScene()
{
    getEventDispatcher()->removeCustomEventListeners("New_Monster_Born");
    getEventDispatcher()->removeCustomEventListeners("Hit_Monster");
    getEventDispatcher()->removeCustomEventListeners("Refresh_Score");
    getEventDispatcher()->removeCustomEventListeners("Game_Over");
    getEventDispatcher()->removeCustomEventListeners("Show_Combo");
}

void GameScene::restartGame(Ref* sender)
{
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("music/menu_click_06.wav");
    auto scene = MainScene::createScene();
    auto trans = TransitionFade::create(0.3f, scene);
    Director::getInstance()->replaceScene(trans);
}

void GameScene::showCombo(int num)
{
    mComboNode->setVisible(true);
    mComboNumber->setNumber(num);
    
    auto delay = DelayTime::create(1.0f);
    auto callback = CallFunc::create([&]
     {
         mComboNode->setVisible(false);
     });
    
    runAction(Sequence::create(delay, callback, NULL));
}

void GameScene::sharePost(cocos2d::Ref *sender)
{
    GameCenterBridge::sharePost(GameManager::shareManager()->getCurrentSocre());
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("music/menu_click_06.wav");
}
