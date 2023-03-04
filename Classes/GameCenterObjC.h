//
//  GameCenterObjC.h
//
//  Created by yoshidayuki on 2014/05/28.
//
//

#import <Foundation/Foundation.h>
#import <GameKit/GameKit.h>

@interface GameCenterObjC : NSObject
+(void) loginGameCenter;
+(void) openRanking;
+(void) postHighScore:(NSString*)key score:(int)score;
+(void) sharePost:(int)score;
@end
