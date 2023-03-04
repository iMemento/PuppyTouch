#include "CTouchManager.h"

CTouchManager* CTouchManager::create(Ref* pTarget, Touch_ClickCallBack callbackClick, Touch_GestureCallBack callbackGesture )
{
    CTouchManager* mTouchManager = new CTouchManager();
    if( mTouchManager && mTouchManager->init( pTarget, callbackClick, callbackGesture) ){
        mTouchManager->autorelease();
        return mTouchManager;
    }
    else{
        delete mTouchManager;
        return NULL;
    }
    return NULL;
}

bool CTouchManager::init( Ref* pTarget, Touch_ClickCallBack callbackClick, Touch_GestureCallBack callbackGesture )
{
    if ( !Layer::init() )
    {
        return false;
    }
    m_pClickCallBack = callbackClick;
    m_pGestureCallBack = callbackGesture;
    m_pTarget = pTarget;

    m_p2dEndTouch = new Point();
    m_pGeometricRecognizer = new GeometricRecognizer();
    m_pGeometricRecognizer->loadTemplates();
    m_bClicked = false;
    m_pEffectRoot = Node::create();
    this->addChild( m_pEffectRoot,10 );
    
    auto touchListener=EventListenerTouchOneByOne::create();
    
    touchListener->onTouchBegan = [=](Touch *touch,Event *event){
        return onTouchBegan(touch, event);
    };
    
    touchListener->onTouchMoved = [=](Touch *touch,Event *event){
        onTouchMoved(touch, event);
    };
    
    touchListener->onTouchEnded = [=](Touch *touch,Event *event){
        onTouchEnded(touch, event);
    };
    
    touchListener->onTouchCancelled = [=](Touch *touch,Event *event){
        onTouchCancelled(touch, event);
    };
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);


    return true;
}

/*void CTouchManager::registerWithTouchDispatcher()
{
    auto touchListener=EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan=[=](Touch *touch,Event *event){
        return true;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    
}*/
bool CTouchManager::onTouchBegan(Touch *pTouch, Event *pEvent)
{
    ParticleSystemQuad* mpEffect = ParticleSystemQuad::create("TouchEffect/TouchEffect.plist");
    unschedule(schedule_selector(CTouchManager::ClearEffectRoot));
    m_pEffectRoot->removeAllChildren();
    m_pEffectRoot->addChild(mpEffect);
    mpEffect->setPosition( 0, 0 );
    EffectFollow( pTouch );
    m_lClickedTime = MilliSecondNow();
    
    m_pDrawPosition = pTouch->getLocation();
    
    m_pDrawNode = cocos2d::DrawNode::create();
    addChild(m_pDrawNode, 0);
    
    m_pDrawNodeForeground = cocos2d::DrawNode::create();
    addChild(m_pDrawNodeForeground, 1);
    
    return true;
}
void CTouchManager::onTouchMoved(Touch *pTouch, Event *pEvent){
    EffectFollow( pTouch );
    FingerRecord( pTouch );
    
    cocos2d::Vec2 tmpPoint = pTouch->getLocation();
    m_pDrawNode->drawSegment(m_pDrawPosition, tmpPoint, 10, Color4F::GRAY);
    m_pDrawNodeForeground->drawSegment(m_pDrawPosition, tmpPoint, 10, Color4F::RED);
    
    m_pDrawPosition = tmpPoint;

}
void CTouchManager::onTouchEnded(Touch *pTouch, Event *pEvent){
    
    FingerJudge();
    DoSomething();
    
    m_pDrawNodeForeground->removeFromParent();
    
    if (m_pDrawNode != NULL) {
        auto delay = DelayTime::create(0.2f);
        auto callback0 = CallFuncN::create([&] (Node* node){
            ((DrawNode*)node)->setColor(Color3B::BLUE);
        });
        auto delay1 = DelayTime::create(0.2f);
        
        auto callback = CallFuncN::create([&] (Node* node){
            node->removeFromParent();
        });
        m_pDrawNode->runAction(Sequence::create(delay, callback0, delay1, callback, NULL));
    }
    
    scheduleOnce(schedule_selector(CTouchManager::ClearEffectRoot), 0.2f);
    
    /* Point mTmp = pTouch->getLocationInView();
    m_p2dEndTouch->x = mTmp.x;
    m_p2dEndTouch->y = mTmp.y;
    if( MilliSecondNow() - m_lClickedTime < 200 ){
        if ( m_bClicked ) {
            DoubleClick();
        }
        else
        {
            scheduleOnce(schedule_selector(CTouchManager::Click), 0.2f);
            m_bClicked = true;
            return;
        }
    }
    
    DoSomething(); */
}
void CTouchManager::onTouchCancelled(Touch *pTouch, Event *pEvent)
{
    m_bClicked = false;
    m_p2dPath.clear();
}

void CTouchManager::Click( float fTime )
{
    if( m_bClicked ){
        m_nInputKind = E_INPUT_KIND_ONE_CLICK;
        m_bClicked = false;
        DoSomething();
    }
}

void CTouchManager::DoubleClick()
{
    m_bClicked = false;
    m_nInputKind = E_INPUT_KIND_DOUBLE_CLICK;
}

long CTouchManager::MilliSecondNow()
{
    struct timeval now;
    gettimeofday(&now, NULL);
    return (now.tv_sec * 1000 + now.tv_usec / 1000); 
}

void CTouchManager::FingerJudge()
{
    if( m_p2dPath.size() < 1 ){
        m_nInputKind = E_INPUT_KIND_VERTICAL;
        return;
    }
    RecognitionResult mResult = m_pGeometricRecognizer->recognize(m_p2dPath);
    m_nInputKind = mResult.name;
}

void CTouchManager::FingerRecord(Touch* pTouch)
{
    Point mLocation = pTouch->getLocationInView();
    Point2D mPoint2DTmp;
    mPoint2DTmp.x = mLocation.x;
    mPoint2DTmp.y = mLocation.y;
    m_p2dPath.push_back(mPoint2DTmp);
}

void CTouchManager::DoSomething()
{
    if( m_nInputKind == E_INPUT_KIND_ONE_CLICK || m_nInputKind == E_INPUT_KIND_DOUBLE_CLICK ){
        (m_pTarget->*m_pClickCallBack)( m_nInputKind, m_p2dEndTouch );
    }
    else{
         (m_pTarget->*m_pGestureCallBack)( m_nInputKind );
    }
    m_p2dPath.clear();
}

void CTouchManager::EffectFollow( Touch* pTouch )
{
    Point mTmp = pTouch->getLocation();
    m_pEffectRoot->setPosition( mTmp.x, mTmp.y );
}

void CTouchManager::ClearEffectRoot(float fTime)
{
    m_pEffectRoot->removeAllChildren();
}






