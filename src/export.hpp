#pragma once

#if defined(_WIN32) || defined(__CYGWIN__)
  #define BOSON_API __declspec(dllexport)
#else
  #define BOSON_API __attribute__((visibility("default")))
#endif
