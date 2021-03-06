#ifndef __BASEFILEUTILS_H__
#define __BASEFILEUTILS_H__

#include "cocos2d.h"
#include "stdlib.h"

using namespace cocos2d;

class BaseFileUtils : public cocos2d::CCFileUtils
{  
protected:  

public:
  BaseFileUtils()
    : cocos2d::CCFileUtils()
  { }
  ~BaseFileUtils() { }

  std::vector<std::string> getNumberedKeys(const char *key);
  std::vector<std::string> getNumberedResourceFiles(const char *key);
  std::string valueForKey(std::string key);
};
#endif  // __BASEFILEUTILS_H__