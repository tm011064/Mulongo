#ifndef __LAYOUTCONTROLLER_H__
#define __LAYOUTCONTROLLER_H__

#include "cocos2d.h"
#include "GameEngineIncludes.h"

using namespace cocos2d;

class LayoutController
{  
public:
    
  LayoutController() { }
  ~LayoutController() { }

  static void addConsoleButton(GameContext* gameContext, CCNode* target, GameButton* gameButton);
  static void addBackground(GameContext* gameContext, CCNode* target, int zOrder);
      
  static CCArray* createOneButton(GameContext* gameContext, DebugDraw* debugDraw, CCPoint anchor
    , CCNode *pTarget
    , SEL_CallFuncO touchEndedDelegate, SEL_CallFuncO preLoadDelegate, SEL_CallFuncO blinkEndedDelegate
    , GameButtonTouchMode gameButtonTouchMode, GameButtonSoundMode gameButtonSoundMode);

  static CCArray* createTwoButtons(GameContext* gameContext, DebugDraw* debugDraw, CCPoint anchor
    , CCNode *pTarget
    , SEL_CallFuncO touchEndedDelegate, SEL_CallFuncO preLoadDelegate, SEL_CallFuncO blinkEndedDelegate
    , GameButtonTouchMode gameButtonTouchMode, GameButtonSoundMode gameButtonSoundMode);
  
  static CCArray* createThreeButtons(GameContext* gameContext, DebugDraw* debugDraw, CCPoint anchor
    , CCNode *pTarget
    , SEL_CallFuncO touchEndedDelegate, SEL_CallFuncO preLoadDelegate, SEL_CallFuncO blinkEndedDelegate
    , GameButtonTouchMode gameButtonTouchMode, GameButtonSoundMode gameButtonSoundMode);
    
  static CCArray* createFourButtons(GameContext* gameContext, DebugDraw* debugDraw, CCPoint anchor
    , CCNode *pTarget
    , SEL_CallFuncO touchEndedDelegate, SEL_CallFuncO preLoadDelegate, SEL_CallFuncO blinkEndedDelegate
    , GameButtonTouchMode gameButtonTouchMode, GameButtonSoundMode gameButtonSoundMode);
    
  static CCArray* createFourButtons(GameContext* gameContext, DebugDraw* debugDraw, CCPoint anchor
    , CCNode *pTarget
    , SEL_CallFuncO touchEndedDelegate, SEL_CallFuncO preLoadDelegate, SEL_CallFuncO blinkEndedDelegate
    , GameButtonTouchMode gameButtonTouchMode, GameButtonSoundMode gameButtonSoundMode
    , ccColor3B buttonColor1, ccColor3B buttonColor2, ccColor3B buttonColor3, ccColor3B buttonColor4);

private:
  static CCArray* createGameButtons(GameContext* gameContext, DebugDraw* debugDraw, CCPoint anchor
    , CCNode *pTarget
    , SEL_CallFuncO touchEndedDelegate, SEL_CallFuncO preLoadDelegate, SEL_CallFuncO blinkEndedDelegate
    , GameButtonTouchMode gameButtonTouchMode, GameButtonSoundMode gameButtonSoundMode
    , ccColor3B buttonColor1, ccColor3B buttonColor2, ccColor3B buttonColor3, ccColor3B buttonColor4
    , int totalDisabledButtons);

};
#endif  // __LAYOUTCONTROLLER_H__