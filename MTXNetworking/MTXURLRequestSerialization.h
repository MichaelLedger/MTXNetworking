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

#pragma mark - URL中文、特殊符号和空格的转码 (当组件中的普通数据包含保留字符时，需要对其进行URL编码,而不是对整个Url进行编码！！！)
/*
 通常如果一样东西需要编码，说明这样东西并不适合传输。原因多种多样，如Size过大，包含隐私数据，对于Url来说，之所以要进行编码，是因为Url中有些字符会引起歧义。
 
 例如Url参数字符串中使用key=value键值对这样的形式来传参，键值对之间以&符号分隔，如/s?q=abc&ie=utf-8。如果你的value字符串中包含了=或者&，那么势必会造成接收Url的服务器解析错误，因此必须将引起歧义的&和=符号进行转义，也就是对其进行编码。
 
 又如，Url的编码格式采用的是ASCII码，而不是Unicode，这也就是说你不能在Url中包含任何非ASCII字符，例如中文。否则如果客户端浏览器和服务端浏览器支持的字符集不同的情况下，中文可能会造成问题。
 
 Url编码的原则就是使用安全的字符（没有特殊用途或者特殊意义的可打印字符）去表示那些不安全的字符。
 
 预备知识：URI是统一资源标识的意思，通常我们所说的Url只是URI的一种。典型Url的格式如上面所示。下面提到的Url编码，实际上应该指的是URI编码。
 
 foo://example.com:8042/over/there?name=ferret#nose
 
  \_/ \______________/ \________/\_________/ \__/
   |         |              |         |        |
 scheme     authority        path     query   fragment
 */

/*
 哪些字符需要编码?
 RFC3986文档规定，Url中只允许包含英文字母（a-zA-Z）、数字（0-9）、-_.~4个特殊字符以及所有保留字符。RFC3986文档对Url的编解码问题做出了详细的建议，指出了哪些字符需要被编码才不会引起Url语义的转变，以及对为什么这些字符需要编码做出了相应的解释。
 
 US-ASCII字符集中没有对应的可打印字符：Url中只允许使用可打印字符。US-ASCII码中的10-7F字节全都表示控制字符，这些字符都不能直接出现在Url中。同时，对于80-FF字节（ISO-8859-1），由于已经超出了US-ACII定义的字节范围，因此也不可以放在Url中。
 
 保留字符：Url可以划分成若干个组件，协议、主机、路径等。有一些字符（:/?#[]@）是用作分隔不同组件的。例如:冒号用于分隔协议和主机，/用于分隔主机和路径，?用于分隔路径和查询参数，等等。还有一些字符（!$&'()*+,;=）用于在每个组件中起到分隔作用的，如=用于表示查询参数中的键值对，&符号用于分隔查询多个键值对。当组件中的普通数据包含这些特殊字符时，需要对其进行编码。
 
 RFC3986中指定了以下字符为保留字符：! * ' ( ) ; : @ & = + $ , / ? # [ ]
 
 不安全字符：还有一些字符，当他们直接放在Url中的时候，可能会引起解析程序的歧义。这些字符被视为不安全字符，原因有很多。
 
 空格：Url在传输的过程，或者用户在排版的过程，或者文本处理程序在处理Url的过程，都有可能引入无关紧要的空格，或者将那些有意义的空格给去掉。
 引号以及<>：引号和尖括号通常用于在普通文本中起到分隔Url的作用
 #：通常用于表示书签或者锚点
 %：百分号本身用作对不安全字符进行编码时使用的特殊字符，因此本身需要编码
 {}|\^[]`~：某一些网关或者传输代理会篡改这些字符
 需要注意的是，对于Url中的合法字符，编码和不编码是等价的，但是对于上面提到的这些字符，如果不经过编码，那么它们有可能会造成Url语义的不同。因此对于Url而言，只有普通英文字符和数字，特殊字符$-_.+!*'()还有保留字符，才能出现在未经编码的Url之中。其他字符均需要经过编码之后才能出现在Url中。
 
 但是由于历史原因，目前尚存在一些不标准的编码实现。例如对于~符号，虽然RFC3986文档规定，对于波浪符号~，不需要进行Url编码，但是还是有很多老的网关或者传输代理会。
 */

/*
 如何对Url中的非法字符进行编码?
 
 Url编码通常也被称为百分号编码（Url Encoding，also known as percent-encoding），是因为它的编码方式非常简单，使用%百分号加上两位的字符——0123456789ABCDEF——代表一个字节的十六进制形式。Url编码默认使用的字符集是US-ASCII。例如a在US-ASCII码中对应的字节是0x61，那么Url编码之后得到的就是%61，我们在地址栏上输入http://g.cn/search?q=%61%62%63，实际上就等同于在google上搜索abc了。又如@符号在ASCII字符集中对应的字节为0x40，经过Url编码之后得到的是%40。
 
 对于非ASCII字符，需要使用ASCII字符集的超集进行编码得到相应的字节，然后对每个字节执行百分号编码。对于Unicode字符，RFC文档建议使用utf-8对其进行编码得到相应的字节，然后对每个字节执行百分号编码。如"中文"使用UTF-8字符集得到的字节为0xE4 0xB8 0xAD 0xE6 0x96 0x87，经过Url编码之后得到"%E4%B8%AD%E6%96%87"。
 
 如果某个字节对应着ASCII字符集中的某个非保留字符，则此字节无需使用百分号表示。例如"Url编码"，使用UTF-8编码得到的字节是0x55 0x72 0x6C 0xE7 0xBC 0x96 0xE7 0xA0 0x81，由于前三个字节对应着ASCII中的非保留字符"Url"，因此这三个字节可以用非保留字符"Url"表示。最终的Url编码可以简化成"Url%E7%BC%96%E7%A0%81" ，当然，如果你用"%55%72%6C%E7%BC%96%E7%A0%81"也是可以的。
 */

/*stringByAddingPercentEscapesUsingEncoding(只对 `#%^{}[]|\"<> 加空格共14个字符编码，不包括”&?”等符号), ios9将淘汰，建议用stringByAddingPercentEncodingWithAllowedCharacters方法  */
/*
 - (nullable NSString *)stringByAddingPercentEscapesUsingEncoding:(NSStringEncoding)enc API_DEPRECATED("Use -stringByAddingPercentEncodingWithAllowedCharacters: instead, which always uses the recommended UTF-8 encoding, and which encodes for a specific URL component or subcomponent since each URL component or subcomponent has different rules for what characters are valid.", macos(10.0,10.11), ios(2.0,9.0), watchos(2.0,2.0), tvos(9.0,9.0));
 
 - (nullable NSString *)stringByReplacingPercentEscapesUsingEncoding:(NSStringEncoding)enc API_DEPRECATED("Use -stringByRemovingPercentEncoding instead, which always uses the recommended UTF-8 encoding.", macos(10.0,10.11), ios(2.0,9.0), watchos(2.0,2.0), tvos(9.0,9.0));
 */

/*
 保留字符:
 
 URLUserAllowedCharacterSet      "#%/:<>?@[\]^`
 
 URLPasswordAllowedCharacterSet  "#%/:<>?@[\]^`{|}
 
 URLHostAllowedCharacterSet      "#%/<>?@\^`{|}
 
 URLPathAllowedCharacterSet      "#%;<>?[\]^`{|}
 
 URLQueryAllowedCharacterSet     "#%<>[\]^`{|}
 
 URLFragmentAllowedCharacterSet  "#%<>[\]^`{|}
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
