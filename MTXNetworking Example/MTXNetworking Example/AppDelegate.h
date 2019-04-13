//
//  AppDelegate.h
//  MTXNetworking Example
//
//  Created by MTX on 2019/4/13.
//  Copyright © 2019 MTX Software Technology Co.,Ltd. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <CoreData/CoreData.h>

@interface AppDelegate : UIResponder <UIApplicationDelegate>

@property (strong, nonatomic) UIWindow *window;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunguarded-availability"
@property (readonly, strong) NSPersistentContainer *persistentContainer;
 #pragma clang diagnostic pop

- (void)saveContext;


@end

