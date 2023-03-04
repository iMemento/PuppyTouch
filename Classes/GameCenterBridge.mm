//
//  GameCenterBridge.cpp
//
//  Created by yoshidayuki on 2014/05/28.
//
//

#include "GameCenterBridge.h"
#include "GameCenterObjC.h"

void GameCenterBridge::loginGameCenter(){
    [GameCenterObjC loginGameCenter];
}

void GameCenterBridge::postHighScore(std::string key, double score){
    [GameCenterObjC postHighScore:[[[NSString alloc] initWithCString:key.c_str() encoding:NSUTF8StringEncoding] autorelease] score:score];
}

void GameCenterBridge::openRanking(){
    [GameCenterObjC openRanking];
}

void GameCenterBridge::sharePost(int score) {
    [GameCenterObjC sharePost: score];
}
