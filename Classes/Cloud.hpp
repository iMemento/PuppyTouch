//
//  Cloud.hpp
//  KittyTouch
//
//  Created by 王小兵 on 16/2/25.
//
//

#ifndef Cloud_hpp
#define Cloud_hpp


#include "cocos2d.h"

class Cloud: public cocos2d::Node {

public:
    CREATE_FUNC(Cloud);
    
    virtual bool init() override;
    
private:
    cocos2d::Sprite* mCloudSprite;
};

#endif /* Cloud_hpp */
