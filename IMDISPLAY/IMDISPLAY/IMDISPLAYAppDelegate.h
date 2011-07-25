//
//  IMDISPLAYAppDelegate.h
//  IMDISPLAY
//
//  Created by 陆 泽夫 on 11-7-25.
//  Copyright 2011年 SES. All rights reserved.
//

#import <UIKit/UIKit.h>

@class MainViewController;

@interface IMDISPLAYAppDelegate : NSObject <UIApplicationDelegate>

@property (nonatomic, retain) IBOutlet UIWindow *window;

@property (nonatomic, retain) IBOutlet MainViewController *mainViewController;

@end
