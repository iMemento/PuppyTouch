//
//  GameCenterObjC.m
//
//  Created by yoshidayuki on 2014/05/28.
//
//

#import "GameCenterObjC.h"

@implementation GameCenterObjC

+(void) loginGameCenter{
    //for iOS6+
    if(floor(NSFoundationVersionNumber) >= NSFoundationVersionNumber_iOS_6_0){
        GKLocalPlayer* player = [GKLocalPlayer localPlayer];
        UIViewController* rootController = [UIApplication sharedApplication].keyWindow.rootViewController;
        player.authenticateHandler = ^(UIViewController* ui, NSError* error )
        {
            if( nil != ui )
            {
                NSLog(@"Need to login");
                [rootController presentViewController:ui animated:YES completion:nil];
            }
            else if( player.isAuthenticated )
            {
                NSLog(@"Authenticated");
            }
            else
            {
                NSLog(@"Failed");
            }
        };
    }
}

+(void) openRanking{
    GKLocalPlayer* localPlayer = [GKLocalPlayer localPlayer];
    if(localPlayer.authenticated)
    {
        UIViewController* rootController = [UIApplication sharedApplication].keyWindow.rootViewController;
        
        GKLeaderboardViewController* leaderboardController = [[GKLeaderboardViewController alloc] init];
        leaderboardController.timeScope = GKLeaderboardTimeScopeAllTime;
        leaderboardController.gameCenterDelegate = (id<GKGameCenterControllerDelegate>)rootController;
        
        leaderboardController.viewState = GKGameCenterViewControllerStateLeaderboards;
        
        leaderboardController.leaderboardIdentifier = [NSString stringWithFormat:@"%s", "PuppyTouchBestScore"];
        [rootController presentViewController:leaderboardController animated:YES completion: nil];
        [leaderboardController release];
    }
    else{
        UIAlertView* alertView = [[UIAlertView alloc] initWithTitle:@"" message:@"Player not signed in" delegate:nil cancelButtonTitle:@"OK" otherButtonTitles: nil];
        [alertView show];
        [alertView release];
    }
}

+(void) postHighScore:(NSString*)key score:(int)score{
    GKLocalPlayer *localPlayer = [GKLocalPlayer localPlayer];
    if([localPlayer isAuthenticated])
    {
        GKScore *gkScore = [[GKScore alloc]initWithLeaderboardIdentifier:key];
        gkScore.value = score;
        
        [gkScore reportScoreWithCompletionHandler:^(NSError *error) {
            if(error)
            {
                NSLog(@"Error : %@",error);
            }
            else
            {
                NSLog(@"Sent highscore.");
            }
        }];
    }
    else
    {
        NSLog(@"Gamecenter not authenticated.");
    }
}

+(void) sharePost:(int)score {
    NSString* shareString = [NSString stringWithFormat:@"I scored %d in the game 'Puppy Touch'!", score];
    
    UIViewController* rootController = [UIApplication sharedApplication].keyWindow.rootViewController;
    UIGraphicsBeginImageContext(rootController.view.bounds.size);
    [rootController.view drawViewHierarchyInRect:rootController.view.bounds afterScreenUpdates:YES];
    // [rootController.view.layer renderInContext:UIGraphicsGetCurrentContext()];
    UIImage *img = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    
    UIImage *shareImage = [UIImage imageWithCGImage:CGImageCreateWithImageInRect([img CGImage], rootController.view.bounds)];
    NSURL* shareUrl = [NSURL URLWithString:@"http://itunes.apple.com/app/id1104761965?mt=8"];
    
    NSArray* activityItems = [NSArray arrayWithObjects:shareString, shareImage, shareUrl, nil];
    UIActivityViewController* activityViewController = [[UIActivityViewController alloc] initWithActivityItems:activityItems applicationActivities:nil];
    activityViewController.modalTransitionStyle = UIModalTransitionStyleCoverVertical;
    
    if ([activityViewController respondsToSelector:sel_registerName("popoverPresentationController")])
    {
        activityViewController.popoverPresentationController.sourceView = rootController.view;
    }
    [rootController presentViewController:activityViewController animated:YES completion:nil];
    [activityViewController release];
}
@end
