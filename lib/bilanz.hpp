# pragma once

#define LOGGING_LEVEL_1

#ifndef NDEBUG
 #include <iostream>
 #include <cassert>
#endif

#ifndef NDEBUG
 #define DEBUG(x) do { \
   { std::cerr << x << std::endl; } \
 } while (0)
 #define DEBUG2(x) do { \
   std::cerr << #x << ": " << x << std::endl; } \
   while (0)
 #define DEBUG3(x) do { \
   std::cerr << #x << ": " << x << " " << __PRETTY_FUNCTION__ << std::endl; } \
   while (0)
#else
 #define DEBUG(x) ((void)0)
#endif
