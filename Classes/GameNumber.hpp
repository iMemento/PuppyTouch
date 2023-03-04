//
//  GameNumber.hpp
//  KingsFingers
//
//  Created by 王小兵 on 15/11/28.
//
//

#ifndef GameNumber_hpp
#define GameNumber_hpp

#include "cocos2d.h"
#include "cocostudio/CCArmature.h"

class GameNumber: public cocos2d::Node
{
public:
    static GameNumber* create(int number, bool big = true);
    void setNumber(int number, bool big = true);
private:
    virtual bool init() override;
    int mNumber;
    bool mBig = true;
};

#endif /* GameNumber_hpp */
