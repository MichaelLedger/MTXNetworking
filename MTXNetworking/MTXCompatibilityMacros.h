//
//  MTXCompatibilityMacros.h
//  MTXNetworking Example
//
//  Created by MTX on 2019/4/13.
//  Copyright © 2019 MTX Software Technology Co.,Ltd. All rights reserved.
//

// AFCompatibilityMacros.h
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


#ifndef MTXCompatibilityMacros_h
#define MTXCompatibilityMacros_h

#ifdef API_UNAVAILABLE
    #define MTX_API_UNAVAILABLE(x) API_UNAVAILABLE(x)
#else
    #define MTX_API_UNAVAILABLE(x)
#endif  // API_UNAVAILABLE

/*
 2015 年 clang 本身增加 API 版本检查功能，通过 -Wpartial-availability 这个 flag 可以打开，后续版本把 flag 变成了 -Wunguarded-availability。打开之后，clang 会把所有在低版本 iOS 上调用高版本 API 的情况用 warning 暴露出来
 
 Xcode中的Clang静态检查提供了iOS低版本调用高版本API检查的功能，可以在开发过程中提前发现问题，避免导致兼容性问题。
 具体实现如下：
 Xcode中进入工程的Build Settings页面，在“Other C Flags”和“Other C++ Flags”中增加“-Wunguarded-availability”
 
 设置好之后，如果误调用了高版本API，Clang会检测到并报出警告。
 
 为避免警告过多而忽视，可以将设置修改为“-Werror-unguarded-availability”，一旦iOS低版本调用了高版本API则会编译报错。
 
 针对pod需要在 podspec 中添加 compiler_flags
 如果确认代码是安全的，例如使用了respondsToSelector等保护，可以用 #pragma clang diagnostic ignored 压掉警告，或者 @available 、 API_AVAILABLE(ios(10.0)) 直接解决：
 
 #pragma clang diagnostic push
 #pragma clang diagnostic ignored "-Wunguarded-availability"
 NSString *str = @"Hello world!";
 [str containsString:@"I'm back!"];
 #pragma clang diagnostic pop
 
 if (@available(iOS 8.0, *)) {
 NSString *str = @"Hello world!";
 [str containsString:@"I'm back!"];
 }
 */
#if __has_warning("-Wunguarded-availability-new")
    #define MTX_CAN_USE_AT_AVAILABLE 1
#else
    #define MTX_CAN_USE_AT_AVAILABLE 0
#endif

#if ((__IPHONE_OS_VERSION_MAX_ALLOWED && __IPHONE_OS_VERSION_MAX_ALLOWED < 100000) || (__MAC_OS_VERSION_MAX_ALLOWED && __MAC_OS_VERSION_MAX_ALLOWED < 101200) || (__WATCH_OS_MAX_VERSION_ALLOWED && __WATCH_OS_MAX_VERSION_ALLOWED < 30000) || (__TV_OS_MAX_VERSION_ALLOWED && __TV_OS_MAX_VERSION_ALLOWED < 100000))
    #define MTX_CAN_INCLUDE_SESSION_TASK_METRICS 0
#else
    #define MTX_CAN_INCLUDE_SESSION_TASK_METRICS 1
#endif

#endif /* MTXCompatibilityMacros_h */
