//
//  MTXURLRequestSerialization.m
//  MTXNetworking Example
//
//  Created by MTX on 2019/4/14.
//  Copyright © 2019 MTX Software Technology Co.,Ltd. All rights reserved.
//

#import "MTXURLRequestSerialization.h"

#pragma mark - MTXPercentEscapedStringFromString
NSString * MTXPercentEscapedStringFromString(NSString *string) {
//    static NSString * const kMTXCharacterGeneralDelimitersToEncode = @":#[]@"; // does not include "?" or "/" due to RFC 3986 - Section 3.4
//    static NSString * const kMTXCharacterSubDelimitersToEncode = @"!$&'()*+,;=";
    
    //URLQueryAllowedCharacterSet包含："*","~",":","+","$",",",";","-","&","=",".","'","/","(","?","!","@",")","
    //因为URLQueryAllowedCharacterSet中包含上述部分的保留字符,其余不包含的保留字符可以直接忽略，因为默认会进行编码，URLQueryAllowedCharacterSet包含的保留字符不会进行URL编码
    static NSString * const kMTXCharacterGeneralDelimitersToEncode = @":@";
    static NSString * const kMTXCharacterSubDelimitersToEncode = @"!$&'()*+,;=";
    
    //我们需要移除这些保留字符，这样stringByAddingPercentEncodingWithAllowedCharacters会对这些保留字符进行URL编码
    NSMutableCharacterSet * allowedCharacterSet = [[NSCharacterSet URLQueryAllowedCharacterSet] mutableCopy];
    [allowedCharacterSet removeCharactersInString:[kMTXCharacterGeneralDelimitersToEncode stringByAppendingString:kMTXCharacterSubDelimitersToEncode]];
    
    // FIXME: https://github.com/AFNetworking/AFNetworking/pull/3028
    // return [string stringByAddingPercentEncodingWithAllowedCharacters:allowedCharacterSet];
    
    static NSUInteger const batchSize = 50;
    
    NSUInteger index = 0;
    NSMutableString *escaped = @"".mutableCopy;
    
    while (index < string.length) {
        NSUInteger length = MIN(string.length - index, batchSize);
        NSRange range = NSMakeRange(index, length);
        
        // 返回从指定范围（开始索引+长度）界定的子串,range可能会有改变
        // To avoid breaking up character sequences (composedcharacters) such as 👴🏻
        range = [string rangeOfComposedCharacterSequencesForRange:range];
        
        NSString *subString = [string substringWithRange:range];
        NSString *encoded = [subString stringByAddingPercentEncodingWithAllowedCharacters:allowedCharacterSet];
        [escaped appendString:encoded];
        
        index += range.length;
    }
    
    return escaped;
}

@interface MTXQueryStringPair : NSObject

@property (nonatomic, strong) id field;
@property (nonatomic, strong) id value;

- (instancetype)initWithField:(id)field value:(id)value;
- (NSString *)URLEncodedString;

@end

@implementation MTXQueryStringPair

- (instancetype)initWithField:(id)field value:(id)value {
    if (self = [super init]) {
        self.field = field;
        self.value = value;
    }
    return self;
}

- (NSString *)URLEncodedString {
    if (!self.value || [self.value isEqual:[NSNull null]]) {
        return MTXPercentEscapedStringFromString([self.field description]);
    } else {
        return [NSString stringWithFormat:@"%@=%@", MTXPercentEscapedStringFromString([self.field description]), MTXPercentEscapedStringFromString([self.value description])];
    }
}

@end

#pragma mark - MTXQueryStringFromParameters

FOUNDATION_EXPORT NSArray <MTXQueryStringPair *> * MTXQueryStringPairsFromDictionary(NSDictionary *dictionary);
FOUNDATION_EXPORT NSArray <MTXQueryStringPair *> * MTXQueryStringPairsFromKeyAndValue(NSString *key, id value);

NSString * MTXQueryStringFromParameters(NSDictionary *parameters) {
    NSMutableArray *mutablePairs = [NSMutableArray array];
    [MTXQueryStringPairsFromDictionary(parameters) enumerateObjectsUsingBlock:^(MTXQueryStringPair * _Nonnull pair, NSUInteger idx, BOOL * _Nonnull stop) {
        [mutablePairs addObject:[pair URLEncodedString]];
    }];
    return [mutablePairs componentsJoinedByString:@"&"];
}

NSArray <MTXQueryStringPair *> * MTXQueryStringPairsFromDictionary(NSDictionary *dictionary) {
    return MTXQueryStringPairsFromKeyAndValue(nil, dictionary);
}

// 递归函数
NSArray <MTXQueryStringPair *> * MTXQueryStringPairsFromKeyAndValue(NSString *key, id value) {
    NSMutableArray *mutableQueryStringComponents = [NSMutableArray array];
    NSSortDescriptor *sortDescriptor = [NSSortDescriptor sortDescriptorWithKey:@"description" ascending:YES selector:@selector(compare:)];
    if ([value isKindOfClass:[NSDictionary class]]) {
        // Sort dictionary keys to ensure consistent ordering in query string, which is important when deserializing potentially ambiguous sequences, such as an array of dictionaries
        NSDictionary *dict = value;
        [[dict.allKeys sortedArrayUsingDescriptors:@[sortDescriptor]] enumerateObjectsUsingBlock:^(id  _Nonnull nestedKey, NSUInteger idx, BOOL * _Nonnull stop) {
            id nestedValue = [dict valueForKey:nestedKey];
            if (nestedValue) {
                [mutableQueryStringComponents addObjectsFromArray:MTXQueryStringPairsFromKeyAndValue((key ? [NSString stringWithFormat:@"%@[%@]", key, nestedKey] : nestedKey), nestedValue)];
            }
        }];
    } else if ([value isKindOfClass:[NSArray class]]) {
        NSArray *array = value;
        [array enumerateObjectsUsingBlock:^(id  _Nonnull nestedValue, NSUInteger idx, BOOL * _Nonnull stop) {
            [mutableQueryStringComponents addObjectsFromArray:MTXQueryStringPairsFromKeyAndValue([NSString stringWithFormat:@"%@[]", key], nestedValue)];
        }];
    } else if ([value isKindOfClass:[NSSet class]]) {
        NSSet *set = value;
        [[set sortedArrayUsingDescriptors:@[sortDescriptor]] enumerateObjectsUsingBlock:^(id  _Nonnull nestedValue, NSUInteger idx, BOOL * _Nonnull stop) {
            [mutableQueryStringComponents addObjectsFromArray:MTXQueryStringPairsFromKeyAndValue(key, nestedValue)];
        }];
    } else {
        [mutableQueryStringComponents addObject:[[MTXQueryStringPair alloc] initWithField:key value:value]];
    }
    return mutableQueryStringComponents;
}

