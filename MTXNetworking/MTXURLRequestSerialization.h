//
//  MTXURLRequestSerialization.h
//  MTXNetworking Example
//
//  Created by MTX on 2019/4/14.
//  Copyright © 2019 MTX Software Technology Co.,Ltd. All rights reserved.
//

// AFURLRequestSerialization.h
// Copyright (c) 2011–2016 Alamofire Software Foundation ( http://alamofire.org/ )
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#import <Foundation/Foundation.h>
#import <TargetConditionals.h>

#if TARGET_OS_IOS || TARGET_OS_TV
#import <UIKit/UIKit.h>
#elif TARGET_OS_WATCH
#import <WatchKit/WatchKit.h>
#endif

NS_ASSUME_NONNULL_BEGIN

/*
 FOUNDATION_EXPORT 与 ＃define 都可以用来定义常量
 .h文件：
 FOUNDATION_EXPORT  NSString *const kMyConstantString;
 .m文件：
 NSString *const kMyConstantString = @"hello world";
 
 #define 方法定义常量
 #define kMyConstantString @"hello world"
 
 使用FOUNDATION_EXPORT方法在检测字符串的值是否相等的时候效率更快.
 可以直接使用(myString == kMyConstantString)来比较, 而define则使用的是([myString isEqualToString:kMyConstantString])
 哪个效率更高,显而易见了：
 第一种是直接比较指针地址
 第二种则是一一比较字符串的每一个字符是否相等.
 */
FOUNDATION_EXPORT NSString * MTXPercentEscapedStringFromString(NSString *string);

@interface MTXURLRequestSerialization : NSObject



@end

NS_ASSUME_NONNULL_END
