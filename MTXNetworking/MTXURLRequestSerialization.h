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

//URL中文等特殊字符转码
/*stringByAddingPercentEscapesUsingEncoding(只对 `#%^{}[]|\"<> 加空格共14个字符编码，不包括”&?”等符号), ios9将淘汰，建议用stringByAddingPercentEncodingWithAllowedCharacters方法  */
//_logo = [autoString(value) stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
//_logo = [autoString(value) stringByAddingPercentEncodingWithAllowedCharacters:[NSCharacterSet URLQueryAllowedCharacterSet]];

/*
 CFStringRef encodedCFString = CFURLCreateStringByAddingPercentEscapes(kCFAllocatorDefault,
 (__bridge CFStringRef) url,
 nil,
 CFSTR("?!@#$^&%*+,:;='\"`<>()[]{}/\\| "),
 kCFStringEncodingUTF8);
 NSString *encodedString1 = [[NSString alloc] initWithString:(__bridge_transfer NSString*) encodedCFString];
 */

/*
 NSString *charactersToEscape = @"?!@#$^&%*+,:;='\"`<>()[]{}/\\| ";
 NSCharacterSet *allowedCharacters = [[NSCharacterSet characterSetWithCharactersInString:charactersToEscape] invertedSet];
 NSString *encodedUrl = [url stringByAddingPercentEncodingWithAllowedCharacters:allowedCharacters];
 */

/*
 NSString *urlString = @"http://192.168.0.125:10122/vue/#/punch/limitactive?id=9";
 NSString *escapedUrlString1 = MTXPercentEscapedStringFromString(urlString);
 NSLog(@"escapedUrlString1: %@", escapedUrlString1);
 // escapedUrlString1: http%3A//192.168.0.125%3A10122/vue/%23/punch/limitactive?id%3D9
 
 NSString *escapedUrlString2 = [urlString stringByAddingPercentEncodingWithAllowedCharacters:[NSCharacterSet URLQueryAllowedCharacterSet]];
 NSLog(@"escapedUrlString2: %@", escapedUrlString2);
 // escapedUrlString2: http://192.168.0.125:10122/vue/%23/punch/limitactive?id=9
 
 NSString *escapedUrlString3 = [urlString stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
 NSLog(@"escapedUrlString3: %@", escapedUrlString3);
 // escapedUrlString3: http://192.168.0.125:10122/vue/%23/punch/limitactive?id=9
 
 NSString *decodedString1 = [escapedUrlString1 stringByReplacingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
 NSLog(@"decodedString1: %@", decodedString1);
 // decodedString1: http://192.168.0.125:10122/vue/#/punch/limitactive?id=9
 
 NSString *decodedString2  =
 (__bridge_transfer NSString *)CFURLCreateStringByReplacingPercentEscapesUsingEncoding(NULL,
 (__bridge CFStringRef)escapedUrlString1,
 CFSTR(""),
 CFStringConvertNSStringEncodingToEncoding(NSUTF8StringEncoding));
 NSLog(@"decodedString2: %@", decodedString2);
 // decodedString2: http://192.168.0.125:10122/vue/#/punch/limitactive?id=9
 */

/*
 - (nullable NSString *)stringByAddingPercentEscapesUsingEncoding:(NSStringEncoding)enc API_DEPRECATED("Use -stringByAddingPercentEncodingWithAllowedCharacters: instead, which always uses the recommended UTF-8 encoding, and which encodes for a specific URL component or subcomponent since each URL component or subcomponent has different rules for what characters are valid.", macos(10.0,10.11), ios(2.0,9.0), watchos(2.0,2.0), tvos(9.0,9.0));
 
 - (nullable NSString *)stringByReplacingPercentEscapesUsingEncoding:(NSStringEncoding)enc API_DEPRECATED("Use -stringByRemovingPercentEncoding instead, which always uses the recommended UTF-8 encoding.", macos(10.0,10.11), ios(2.0,9.0), watchos(2.0,2.0), tvos(9.0,9.0));
 */

/**
 Returns a percent-escaped string following RFC 3986 for a query string key or value.
 RFC 3986 states that the following characters are "reserved" characters.
 - General Delimiters: ":", "#", "[", "]", "@", "?", "/"
 - Sub-Delimiters: "!", "$", "&", "'", "(", ")", "*", "+", ",", ";", "="
 
 In RFC 3986 - Section 3.4, it states that the "?" and "/" characters should not be escaped to allow
 query strings to include a URL. Therefore, all "reserved" characters with the exception of "?" and "/"
 should be percent-escaped in the query string.
 
 @param string The string to be percent-escaped.
 
 @return The percent-escaped string.
 */
FOUNDATION_EXPORT NSString * MTXPercentEscapedStringFromString(NSString *string);

/**
 A helper method to generate encoded url query parameters for appending to the end of a URL.
 
 @param parameters A dictionary of key/values to be encoded.
 
 @return A url encoded query string
 */
FOUNDATION_EXPORT NSString * MTXQueryStringFromParameters(NSDictionary *parameters);

NS_ASSUME_NONNULL_END
