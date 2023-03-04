#ifndef __TOUCH_MANAGER__
#define __TOUCH_MANAGER__

#include "cocos2d.h"
#include "GeometricRecognizer.h"

USING_NS_CC;
using namespace FingerRecognizer;

typedef void (Ref::*Touch_ClickCallBack)( int state, Point* pos );
#define Clickcallback_selector(_SELECTOR) (Touch_ClickCallBack)(&_SELECTOR)
typedef void (Ref::*Touch_GestureCallBack)( int state );
#define Gesturecallback_selector(_SELECTOR) (Touch_GestureCallBack)(&_SELECTOR)

class CTouchManager : public Layer
{

public:
    static CTouchManager* create( Ref* pTarget, Touch_ClickCallBack callbackClick, Touch_GestureCallBack callbackGesture);
    
    bool onTouchBegan(Touch *pTouch, Event *pEvent);
    void onTouchEnded(Touch *pTouch, Event *pEvent);
    void onTouchMoved(Touch *pTouch, Event *pEvent);
    void onTouchCancelled(Touch *pTouch, Event *pEvent);

private:
    Point* m_p2dEndTouch;
    int m_nInputKind;
    long m_lClickedTime;
    bool m_bClicked;
    GeometricRecognizer* m_pGeometricRecognizer;
    Node* m_pEffectRoot;
    Path2D m_p2dPath;
    bool init( Ref* pTarget, Touch_ClickCallBack callbackClick, Touch_GestureCallBack callbackGesture );
    void Click(float fTime);
    void DoubleClick();
    long MilliSecondNow();
    void FingerJudge();
    void FingerRecord(Touch* pTouch );
    void DoSomething();
    void EffectFollow(Touch* pTouch );
    void ClearEffectRoot(float fTime);

    Ref* m_pTarget;
    Touch_ClickCallBack m_pClickCallBack;
    Touch_GestureCallBack m_pGestureCallBack;
    
    cocos2d::Vec2 m_pDrawPosition;
    cocos2d::DrawNode* m_pDrawNode;
    cocos2d::DrawNode* m_pDrawNodeForeground;
};

#endif