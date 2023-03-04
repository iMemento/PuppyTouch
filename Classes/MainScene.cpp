//
//  MainScene.cpp
//  KittyTouch
//
//  Created by 王小兵 on 16/4/17.
//
//

#include "MainScene.hpp"
#include "GameConstants.h"
#include "Cloud.hpp"
#include "Monster.hpp"
#include "GameScene.h"
#include "GameCenterBridge.h"
#include <SimpleAudioEngine.h>

USING_NS_CC;
using namespace ui;

Scene* MainScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = MainScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MainScene::init()
{
    // super init first
    if ( !LayerColor::initWithColor(Color4B(136, 221, 238, 255)) )
    {
        return false;
    }
    
    createBackground();
    
    return true;
}

MainScene::~MainScene()
{
    
}

void MainScene::createBackground()
{
    cocostudio::ArmatureDataManager::getInstance()->addArmatureFileInfo("effect/guaiwu_texiao01.ExportJson");
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    auto ground = LayerColor::create(Color4B(205, 202, 160, 255));
    ground->setPosition(0.0f, 0.0f);
    ground->setContentSize(Size(visibleSize.width, visibleSize.height * 0.3f));
    addChild(ground, GameZOrder::BG_ZORDER);
    
    auto grass = Sprite::create("ui/grass.png");
    grass->setPosition(visibleSize.width * 0.5f, visibleSize.height * 0.3f);
    grass->setScale(10.0f, 10.0f);
    grass->setAnchorPoint(Vec2(0.5f, 0.0f));
    grass->getTexture()->setAliasTexParameters();
    addChild(grass, GameZOrder::BG_ZORDER);
    
    ground = LayerColor::create(Color4B(104, 80, 29, 255));
    ground->setPosition(0.0f, visibleSize.height * 0.295f);
    ground->setContentSize(Size(visibleSize.width, 10.0f));
    addChild(ground, GameZOrder::BG_ZORDER);
    
    grass = Sprite::create("ui/ground.png");
    grass->setPosition(visibleSize.width * 0.5f, visibleSize.height * 0.3f);
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
    
    auto node = Node::create();
    addChild(node);
    node->setPosition(visibleSize.width * 0.5f, visibleSize.height * 0.5f);
    
    // game title
    auto title = Sprite::create("ui/GameTitle.png");
    title->getTexture()->setAliasTexParameters();
    title->setPosition(0, visibleSize.height * 0.2f);
    title->setScale(10);
    node->addChild(title);
    
    auto swing = createSwingAnimation();
    swing->setPosition(visibleSize.width * 0.2f, visibleSize.height * 0.2f);
    node->addChild(swing);
    
    swing = createSwingAnimation();
    swing->setPosition(-visibleSize.width * 0.2f, visibleSize.height * 0.32f);
    node->addChild(swing);
    
    auto nodeAction = Sequence::create(
                                   MoveBy::create(0.6f, Vec2(0.0f, 30.0f)),
                                       //DelayTime::create(0.4f),
                                   MoveBy::create(0.6f, Vec2(0.0f, -30.0f)),
                                       DelayTime::create(4.0f),
                                   NULL);
    node->runAction(RepeatForever::create(nodeAction));
    
    
    auto model = new MonsterModel();
    model->monsterType = Doge;
    model->gesture = 1;

    auto catHead = Monster::create(model);
    catHead->setPosition(visibleSize.width * 0.5f, visibleSize.height * 0.5f);
    catHead->playerMainSceneAnimation();
    // addChild(catHead);
    
    
    auto startButton = MenuItemImage::create("ui/startButton.png", "ui/startButton.png",
                                              CC_CALLBACK_1(MainScene::startGame, this));
    startButton->getSelectedImage()->setScale(0.9f);
    
    
    auto rankButton = MenuItemImage::create("ui/rankButton.png", "ui/rankButton.png",
                                             CC_CALLBACK_1(MainScene::checkRank, this));
    
    rankButton->getSelectedImage()->setScale(0.9f);
    
    
    startButton->setPosition(visibleSize.width * 0.27f, visibleSize.height * 0.2f);
    rankButton->setPosition(visibleSize.width * 0.73f, visibleSize.height * 0.2f);
    
    auto starMenu = Menu::create(startButton, rankButton, NULL);
    starMenu->setPosition(Point::ZERO);
    addChild(starMenu, 1);
    
    startButton->setScale(2.0f);
    rankButton->setScale(2.0f);
    
    
    // create princess
    auto princess = Sprite::create("ui/princess0.png");
    princess->setPosition(visibleSize.width * 0.5f - 50.0f, visibleSize.height * 0.3f);
    princess->setScale(20.0f, 20.0f);
    princess->setAnchorPoint(Vec2(0.5f, 0.0f));
    princess->getTexture()->setAliasTexParameters();
    
    
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
    princess->runAction(RepeatForever::create(Sequence::create(action, DelayTime::create(2.0f), NULL)));
    
    
    addChild(princess, GameZOrder::PLAYER_ZORDER);

}

Node* MainScene::createSwingAnimation()
{
    auto sprite = Sprite::create("ui/swing0.png");
    
    
    sprite->setScale(8.0f, 8.0f);
    
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
    sprite->runAction(RepeatForever::create(action));
    return sprite;
}

void MainScene::startGame(Ref* sender)
{
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("music/menu_click_06.wav");
    auto scene = GameScene::createScene();
    auto trans = TransitionFade::create(0.3f, scene);
    Director::getInstance()->replaceScene(trans);
}
void MainScene::checkRank(Ref* sender)
{
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("music/menu_click_06.wav");
    GameCenterBridge::openRanking();
}
