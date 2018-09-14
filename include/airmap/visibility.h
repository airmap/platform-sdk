//
//  visibility.h
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#ifndef AIRMAP_VISIBILITY_H_
#define AIRMAP_VISIBILITY_H_

#if defined(__GNUC__) || defined(__clang__)
#define AIRMAP_EXPORT __attribute__((visibility("default")))
#elif defined(_MSC_VER)
#define AIRMAP_EXPORT __declspec(dllexport)
#else
#pragma message("unknown compiler - default AIRMAP_EXPORT to empty")
#define AIRMAP_EXPORT
#endif

#endif  // AIRMAP_VISIBILITY_H_