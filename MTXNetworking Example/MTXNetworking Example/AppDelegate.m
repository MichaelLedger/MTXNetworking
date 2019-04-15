//
//  AppDelegate.m
//  MTXNetworking Example
//
//  Created by MTX on 2019/4/13.
//  Copyright © 2019 MTX Software Technology Co.,Ltd. All rights reserved.
//

#import "AppDelegate.h"
#import "MTXCompatibilityMacros.h"
#import "MTXURLRequestSerialization.h"

@interface AppDelegate ()

@end

@implementation AppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    // Override point for customization after application launch.
    
    NSLog(@"MTX_CAN_USE_AT_AVAILABLE: %d", MTX_CAN_USE_AT_AVAILABLE);
    NSLog(@"MTX_CAN_INCLUDE_SESSION_TASK_METRICS: %d", MTX_CAN_INCLUDE_SESSION_TASK_METRICS);
    
    NSString *string = @"😯你😯😯";
    NSRange range = [string rangeOfComposedCharacterSequencesForRange:NSMakeRange(0, 4)];
    NSString * result = [string substringWithRange:range];
    NSLog(@"location:%ld\nlength:%ld\nresult:%@", range.location, range.length, result);
    /*
     location:0
     length:5
     result:😯你😯
     */
    
    NSString *urlString = @"http://192.168.0.125:10122/vue/#/punch/limitactive";
    NSString *escapedUrlString = MTXPercentEscapedStringFromString(urlString);
    NSLog(@"escapedUrlString: %@", escapedUrlString);
    /*
     escapedUrlString: http%3A//192.168.0.125%3A10122/vue/%23/punch/limitactive
     */
    
    NSDictionary *params = @{@"dyId"    : @(888888),
                             @"userId"  : @(13232),
                             @"userName": @"MTXNetworking",
                             @"userLogo": @"https://timgsa.baidu.com/timg?image&quality=80&size=b9999_10000&sec=1555320233263&di=5493da76bc87cf7d985c877fc0e0027b&imgtype=0&src=http%3A%2F%2Fwerkstette.dk%2Fwp-content%2Fuploads%2F2015%2F09%2FEntertainment_Weekly_Photographer_Marc_Hom_British_Actor_Charlie_Hunnam_as_King_Arthur_Retouch_Werkstette10-770x841.jpg",
                             @"content" : @"TEST-Cotent"
                             };
    NSString *queryString = MTXQueryStringFromParameters(params);
    NSLog(@"queryString: %@", queryString);
    /*
     queryString: content=TEST-Cotent&dyId=888888&userId=13232&userLogo=https%3A//timgsa.baidu.com/timg?image%26quality%3D80%26size%3Db9999_10000%26sec%3D1555320233263%26di%3D5493da76bc87cf7d985c877fc0e0027b%26imgtype%3D0%26src%3Dhttp%253A%252F%252Fwerkstette.dk%252Fwp-content%252Fuploads%252F2015%252F09%252FEntertainment_Weekly_Photographer_Marc_Hom_British_Actor_Charlie_Hunnam_as_King_Arthur_Retouch_Werkstette10-770x841.jpg&userName=MTXNetworking
     */
    
    return YES;
}


- (void)applicationWillResignActive:(UIApplication *)application {
    // Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
    // Use this method to pause ongoing tasks, disable timers, and invalidate graphics rendering callbacks. Games should use this method to pause the game.
}


- (void)applicationDidEnterBackground:(UIApplication *)application {
    // Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later.
    // If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
}


- (void)applicationWillEnterForeground:(UIApplication *)application {
    // Called as part of the transition from the background to the active state; here you can undo many of the changes made on entering the background.
}


- (void)applicationDidBecomeActive:(UIApplication *)application {
    // Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
}


- (void)applicationWillTerminate:(UIApplication *)application {
    // Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
    // Saves changes in the application's managed object context before the application terminates.
    [self saveContext];
}


#pragma mark - Core Data stack

@synthesize persistentContainer = _persistentContainer;

- (NSPersistentContainer *)persistentContainer  API_AVAILABLE(ios(10.0)){
    // The persistent container for the application. This implementation creates and returns a container, having loaded the store for the application to it.
    @synchronized (self) {
        if (_persistentContainer == nil) {
            _persistentContainer = [[NSPersistentContainer alloc] initWithName:@"MTXNetworking_Example"];
            [_persistentContainer loadPersistentStoresWithCompletionHandler:^(NSPersistentStoreDescription *storeDescription, NSError *error) {
                if (error != nil) {
                    // Replace this implementation with code to handle the error appropriately.
                    // abort() causes the application to generate a crash log and terminate. You should not use this function in a shipping application, although it may be useful during development.
                    
                    /*
                     Typical reasons for an error here include:
                     * The parent directory does not exist, cannot be created, or disallows writing.
                     * The persistent store is not accessible, due to permissions or data protection when the device is locked.
                     * The device is out of space.
                     * The store could not be migrated to the current model version.
                     Check the error message to determine what the actual problem was.
                    */
                    NSLog(@"Unresolved error %@, %@", error, error.userInfo);
                    abort();
                }
            }];
        }
    }
    
    return _persistentContainer;
}

#pragma mark - Core Data Saving support

- (void)saveContext {
    NSManagedObjectContext *context = self.persistentContainer.viewContext;
    NSError *error = nil;
    if ([context hasChanges] && ![context save:&error]) {
        // Replace this implementation with code to handle the error appropriately.
        // abort() causes the application to generate a crash log and terminate. You should not use this function in a shipping application, although it may be useful during development.
        NSLog(@"Unresolved error %@, %@", error, error.userInfo);
        abort();
    }
}

@end
