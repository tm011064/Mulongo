#ifndef __TYPES_H__
#define __TYPES_H__

#include "cocos2d.h"

using namespace cocos2d;

enum SceneType
{
  MENU_SCENE = 0,
  ARCADE_NORMAL_GAME_SCENE = 1,
  ARCADE_EASY_GAME_SCENE = 2,
  ARCADE_HARD_GAME_SCENE = 3,
  CHALLENGE = 4,
  OPTIONS_SCENE = 5,
  HIGHSCORE_SCENE = 6,
  LOAD_GAME_SCENE = 7
};

enum ChallengeSceneType
{
  REACH_LEVEL = 0,
  REPEAT_ONE_OFF = 1,
  EXACT_LENGTH = 2,
  RHYTHM = 3,
};


enum SceneState
{
  RUNNING_SEQUENCE_ANIMATION = 0,
  AWAITING_INPUT = 1,
  PAUSED = 2,
  LOADING = 3,
  UNLOADED = 4,
  RUNNING_END_OF_GAME_ANIMATION = 5,
};

enum SceneRenderMode
{
  NEW = 0,
  PUSH = 1,
  POP = 2,
  FIRST_RUN = 3,
};

enum ButtonState
{
  UNGRABBED,
  GRABBED,
};

enum ButtonAnimation
{
  IDLE = 0,
  PRESSING = 1,
  PRESSED = 2,
  RELEASING = 3,
  DISABLED = 4,
  BLINK = 5,
};

enum MenuViewType
{
  HOME = 0,
  ARCADE = 1,
  STORY_MODE = 2
};

typedef struct
{
  CCSize size;
  char directory[16];
} ResourceDefinition;

typedef struct {
  std::vector<int> plistIndexes;
  int endIndex;
  int totalLoops;
  SEL_CallFuncO fnpSelectorDelegate;
  int framesPerSecond;
  bool needToRunReverseAnimation;
  bool needToDeleteItself;
} AnimationDefinition;

typedef struct 
{
  CCPoint location;
  float radius;
} Circle;

typedef struct 
{
  long deltaToNextIncreaseInSeconds;
  int totalLifes;
} LifeInfo;

typedef struct 
{
  float totalPoints;
  float totalLevelBonus;
  float totalButtonBonus;
  int level;
  int coinsEarned;
  float totalTimeElapsed;

  int starsEarned;

  float averageButtonBlinkPercentage;
  float averageButtonBlinkStartOffset;
  float averageButtonBlinkDurationOffset;

} GameScore;

typedef struct 
{
  float correctButtonScore;
  float maxTimeBonus;
  float clickTimeThreshold;
  float levelBonus;
  float maxLevelTimeBonus;

  float coinsEarnedMultiplier;

  int mininimumPointsForOneStar;
  int mininimumPointsForTwoStars;
  int mininimumPointsForThreeStars;

  float minimumTotalTimePercentageForOneStar;
  float minimumTotalTimePercentageForTwoStars;
  float minimumTotalTimePercentageForThreeStars;

  ChallengeSceneType challengeSceneType;
  float levelToReach;
  float totalButtons;

} ChallengePointScoreDefinition;

typedef struct 
{
  CCPoint leftBottom;
  CCPoint rightTop;
  ccColor4F borderColor;
  ccColor4F fillColor;
} ColoredRect;


#endif  // __TYPES_H__