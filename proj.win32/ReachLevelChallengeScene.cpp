#include "ReachLevelChallengeScene.h"
#include "LayoutController.h"
#include <cstdlib>

using namespace cocos2d;

ReachLevelChallengeScene* ReachLevelChallengeScene::create(GameContext* gameContext, bool showSplashScreen, int challengeIndex
  , int totalEnabledButtons, ChallengePointScoreDefinition challengePointScoreDefinition, int levelToReach)
{
  ReachLevelChallengeScene* scene = new ReachLevelChallengeScene(gameContext, showSplashScreen, challengeIndex, totalEnabledButtons, REACH_LEVEL, challengePointScoreDefinition, levelToReach);
  scene->init();

  return scene;
}

void ReachLevelChallengeScene::onPostInitialize()
{  
  switch (this->m_totalEnabledButtons)
  {
  case 2:
    this->m_buttons = LayoutController::createTwoButtons(this->m_pGameContext, this->m_debugDraw, this->m_anchor, this
      , callfuncO_selector( ReachLevelChallengeScene::buttonTouchEndedCallback )
      , callfuncO_selector( ReachLevelChallengeScene::buttonLoadedCallback )
      , callfuncO_selector( ReachLevelChallengeScene::buttonBlinkCallback )
      , FIRE_ON_TOUCH_BEGAN, DRUMS);      
    break;

  case 3:
        
    this->m_buttons = LayoutController::createThreeButtons(this->m_pGameContext, this->m_debugDraw, this->m_anchor, this
      , callfuncO_selector( ReachLevelChallengeScene::buttonTouchEndedCallback )
      , callfuncO_selector( ReachLevelChallengeScene::buttonLoadedCallback )
      , callfuncO_selector( ReachLevelChallengeScene::buttonBlinkCallback )
      , FIRE_ON_TOUCH_BEGAN, DRUMS);      
    break;

  case 4:
    this->m_buttons = LayoutController::createFourButtons(this->m_pGameContext, this->m_debugDraw, this->m_anchor, this
      , callfuncO_selector( ReachLevelChallengeScene::buttonTouchEndedCallback )
      , callfuncO_selector( ReachLevelChallengeScene::buttonLoadedCallback )
      , callfuncO_selector( ReachLevelChallengeScene::buttonBlinkCallback )
      , FIRE_ON_TOUCH_BEGAN, DRUMS);      
    break;
  }   

  this->m_totalVisibleButtons = this->m_buttons->count();

  CCObject* o;
  CCARRAY_FOREACH(this->m_buttons, o)
  {
    LayoutController::addConsoleButton(m_pGameContext, this, (GameButton*)o);
  }  
}

void ReachLevelChallengeScene::onPreInitialize()
{  
  WildcardButtonDefinition wildcardButtonDefinition1;
  
  wildcardButtonDefinition1.callback = callfuncO_selector(ReachLevelChallengeScene::replaySequenceCallback);
  wildcardButtonDefinition1.callbackTarget = this;
  wildcardButtonDefinition1.text = "REPLAY\nSEQUENCE";
  wildcardButtonDefinition1.totalCoins = COINS_COST_REPLAY_SEQUENCE;
  this->m_wildcardButtonDefinitions.push_back(wildcardButtonDefinition1);

  WildcardButtonDefinition wildcardButtonDefinition2;
  
  wildcardButtonDefinition2.callback = callfuncO_selector(ReachLevelChallengeScene::replaySequenceRemainingCallback);
  wildcardButtonDefinition2.callbackTarget = this;
  wildcardButtonDefinition2.text = "REPLAY\nREMAINING";
  wildcardButtonDefinition2.totalCoins = COINS_COST_SHOW_REMAINING;
  
  this->m_wildcardButtonDefinitions.push_back(wildcardButtonDefinition2);
}

void ReachLevelChallengeScene::onLoadDescriptionPopup()
{  
  /********** DESCRIPTION POPUP **********/  
  m_descriptionPopup = DescriptionPopup::create(
    this->m_pGameContext
    , callfuncO_selector(ReachLevelChallengeScene::newGameCallback)
    , this
    , this->m_challengePointScoreDefinition);
  m_descriptionPopup->setZOrder(SPLASH_ZORDER);
  this->addChild(m_descriptionPopup);
  /********** DESCRIPTION POPUP **********/
}

void ReachLevelChallengeScene::startNewGame()
{  
  if (this->m_wildcardPopup->isVisible())
    this->m_wildcardPopup->hide();
  if (this->m_gameScorePopup->isVisible())
    this->m_gameScorePopup->hide();
    
  this->m_buttonSequence.clear();
  this->m_buttonSequenceIndex = 0;
  
  this->m_gameScore.starsEarned = 0;
  this->m_gameScore.level = 0;
  this->m_gameScore.totalButtonBonus = 0;
  this->m_gameScore.totalLevelBonus = 0;
  this->m_gameScore.totalPoints= 0;
  this->m_gameScore.totalTimeElapsed = 0;

  this->m_topBar->setScore(0);
  this->m_topBar->setLevel(1);
  
  runSequenceAnimation(true, 0, -1);
}

void ReachLevelChallengeScene::runSequenceAnimation(bool doAddButton, int startIndex, int endIndex)
{
  this->m_sceneState = RUNNING_SEQUENCE_ANIMATION;

  this->unschedule(schedule_selector(ReachLevelChallengeScene::update));
  
  if (doAddButton)
  {
    std::srand(time(NULL));
    GameButton* button = (GameButton*)m_buttons->objectAtIndex(rand() % this->m_totalVisibleButtons);
    while (!button->getIsEnabled())
    {
      button = NULL;
      button = (GameButton*)m_buttons->objectAtIndex(rand() % this->m_totalVisibleButtons);
    }
    m_buttonSequence.push_back(button);
    button = NULL;
  }

  m_buttonSequenceIndex = startIndex;

  this->m_lastStartIndex = startIndex;
  if (endIndex == -1)
    this->m_lastEndIndex = m_buttonSequence.size();
  else
    this->m_lastEndIndex = endIndex;

  float seqLength = (float)m_buttonSequence.size();
  float delay = BLINK_SPEED_NORMAL - (BLINK_SPEED_STEP * seqLength);
  if (delay < BLINK_SPEED_THRESHOLD)
    delay = BLINK_SPEED_THRESHOLD;
  
  this->schedule(schedule_selector(ReachLevelChallengeScene::update), delay);
}

void ReachLevelChallengeScene::update(float delta)
{  
  GameButton* button = m_buttonSequence.at(m_buttonSequenceIndex);
  
  button->playAnimation(BLINK);
  m_buttonSequenceIndex++;

  if (m_buttonSequenceIndex >= this->m_lastEndIndex)
  {
    this->unschedule(schedule_selector(ReachLevelChallengeScene::update));
  }  
}

void ReachLevelChallengeScene::onSequenceBlinkCallback(GameButton* gameButton)
{
  if (this->m_sceneState == RUNNING_END_OF_GAME_ANIMATION)
    return;

  // this method is called when the sequence animation buttons blink
  if (m_buttonSequenceIndex >= this->m_lastEndIndex)
  {
    if (this->m_lastEndIndex == m_buttonSequence.size()
      && this->m_lastStartIndex == 0)
    {
      // this was a normal level run, so reset the time. We don't do that if the player used a wildcard in which
      // case the sequence animation might not have started from zero or the last index is not the last sequence element
      
      // reset timer
      CCTime::gettimeofdayCocos2d(this->m_lastButtonPressedTime, NULL);
    }

    // animation has finished, now we allow input again
    this->m_buttonSequenceIndex = this->m_lastStartIndex;
    this->m_sceneState = AWAITING_INPUT;
  }
}

void ReachLevelChallengeScene::onCorrectButtonPressed()
{
  this->m_hasUserStartedGame = true;
  this->m_lastButtonPressed->playAnimation(BLINK, false); 

  float deltaTime = .0f;
  float bonus = 0;

  if ( this->m_buttonSequenceIndex > 0 )
  {
    deltaTime = updateTimeVal(this->m_lastButtonPressedTime);

    if (deltaTime < this->m_challengePointScoreDefinition.clickTimeThreshold)
    {
      bonus = this->m_challengePointScoreDefinition.maxTimeBonus * (1 - deltaTime / this->m_challengePointScoreDefinition.clickTimeThreshold); 
      bonus = (int)bonus - (int)bonus % 10;
      m_gameScore.totalButtonBonus += bonus;
      m_currentLevelPoints += bonus;
    }
  }
  else
  {
    m_currentLevelPoints = 0;
    CCTime::gettimeofdayCocos2d(this->m_firstUserSequencePressedTime, NULL);
  }
  this->m_buttonSequenceIndex++;

  m_currentLevelPoints += bonus;
  m_currentLevelPoints = (int)m_currentLevelPoints - (int)m_currentLevelPoints % 10;
    
  m_gameScore.level = m_buttonSequence.size();

  if (m_buttonSequenceIndex >= m_gameScore.level)
  {// correct, new animation   
    
    deltaTime = updateTimeVal(this->m_firstUserSequencePressedTime);
    float levelTimeThreshold = this->m_challengePointScoreDefinition.clickTimeThreshold * m_gameScore.level;
    if (deltaTime < levelTimeThreshold)
    {
      bonus = this->m_challengePointScoreDefinition.maxLevelTimeBonus * (1 - deltaTime / levelTimeThreshold);  
      bonus = (int)bonus - (int)bonus % 10;
      m_gameScore.totalLevelBonus += bonus;
      m_gameScore.totalPoints += bonus;
      m_currentLevelPoints += bonus;
    }

    m_currentLevelPoints += this->m_challengePointScoreDefinition.levelBonus;
    m_currentLevelPoints = (int)m_currentLevelPoints - (int)m_currentLevelPoints % 10;
    
    this->m_gameScore.totalPoints += m_currentLevelPoints;
    m_currentLevelPoints = .0f;

    m_topBar->setScore((long)this->m_gameScore.totalPoints);
    
    float mark = bonus / this->m_challengePointScoreDefinition.maxLevelTimeBonus;    
    if (mark > .92f)
      this->playConsoleLabelAnimation("PERFECT", 1);
    else if (mark > .88f)
      this->playConsoleLabelAnimation("GREAT", 2);
    else if (mark > .75)
      this->playConsoleLabelAnimation("GOOD", 3);
    else
      this->playConsoleLabelAnimation("CORRECT", 4);
    
    if (m_buttonSequenceIndex == m_levelToReach)
    {
      this->m_sceneState = RUNNING_END_OF_GAME_ANIMATION;
      this->m_buttonSequenceIndex = 0;
  
      this->m_gameScore.coinsEarned = round( (float)m_gameScore.level * m_challengePointScoreDefinition.coinsEarnedMultiplier );
      this->m_pGameContext->setTotalCoins(this->m_pGameContext->getTotalCoins() + m_gameScore.coinsEarned);           
            
      this->updateChallengeInfo(&this->m_challengePointScoreDefinition);
      m_pGameContext->setGameScore( m_gameScore );
        
      if ( m_gameScore.starsEarned > 0)
      {
        this->m_pGameContext->setTotalLifes(this->m_pGameContext->getTotalLifes() + 1);      
        this->m_showWildcardScoreInfo = false;
      
        // Highscore
        switch (this->m_totalEnabledButtons)
        {
        case 2:
          if (m_gameScore.totalPoints > m_pGameContext->getHighscoreMemory2b())
            this->m_pGameContext->setHighscoreMemory2b(m_gameScore.totalPoints);
          if (m_gameScore.level > m_pGameContext->getHighscoreMemory2bTotal())
            this->m_pGameContext->setHighscoreMemory2bTotal(m_gameScore.level);
          break;
        case 3:
          if (m_gameScore.totalPoints > m_pGameContext->getHighscoreMemory3b())
            this->m_pGameContext->setHighscoreMemory3b(m_gameScore.totalPoints);
          if (m_gameScore.level > m_pGameContext->getHighscoreMemory3bTotal())
            this->m_pGameContext->setHighscoreMemory3bTotal(m_gameScore.level);
          break;
        case 4:
          if (m_gameScore.totalPoints > m_pGameContext->getHighscoreMemory4b())
            this->m_pGameContext->setHighscoreMemory4b(m_gameScore.totalPoints);
          if (m_gameScore.level > m_pGameContext->getHighscoreMemory4bTotal())
            this->m_pGameContext->setHighscoreMemory4bTotal(m_gameScore.level);
          break;
        }
      }

      this->playBlinkButtonsAnimation(2, .25f, .8f);
      this->scheduleOnce(schedule_selector(ReachLevelChallengeScene::showGameScorePopupCallback), 2.0f);
    }     
    else
    {
      // we have the button index and scene state check on the buttonCallback method which is used to
      // determine whether we should start a new animation or input session. We have to set the state
      // and sequence index here so this method doesn't get confused with rapid multi touch events
      // happening between timer callbacks.
      this->m_sceneState = RUNNING_SEQUENCE_ANIMATION;
      this->m_buttonSequenceIndex = 0;

      m_topBar->setLevel(m_gameScore.level + 1);      
      this->scheduleOnce(schedule_selector(ReachLevelChallengeScene::runSequenceAnimationTimerCallback), .32f);
    }
  }
}

void ReachLevelChallengeScene::runSequenceAnimationTimerCallback(float dt)
{      
  runSequenceAnimation(true, 0, -1);
}

void ReachLevelChallengeScene::onIncorrectButtonPressed()
{
  this->m_hasUserStartedGame = true; // just in case the user got the first click wrong...
  this->m_sceneState = RUNNING_END_OF_GAME_ANIMATION;

  this->m_gameScore.coinsEarned = round( (float)m_gameScore.level * m_challengePointScoreDefinition.coinsEarnedMultiplier );
  this->m_pGameContext->setTotalCoins(this->m_pGameContext->getTotalCoins() + m_gameScore.coinsEarned);

  m_topBar->setScore((long)this->m_gameScore.totalPoints);
  m_pGameContext->setGameScore( m_gameScore );
    
  float wrongDelay = 1.6f;
  float correctBlinkDelay = 2.3f;
  this->m_eogTotalWrongButtonBlinks = 3;
  this->m_eogElaspedTime = 0;
  this->m_eogTargetTime = .8f;
  this->m_eogTargetTimeLastButton = .55f;
  this->m_eogElapsedTimeWrongButton = -wrongDelay;
      
  this->m_showWildcardScoreInfo = true;
  this->m_wildcardScoreInfoLeft = "Buttons";
  char str[64];        
  sprintf(str, "%i / %i", m_buttonSequence.size() - 1, m_levelToReach);
  this->m_wildcardScoreInfoRight = str;  

  m_lastButtonPressed->playAnimation(PRESSED);

  this->schedule(schedule_selector(ReachLevelChallengeScene::eogGrayOutButtons), 0.021f); // framerate: 1/48
  this->schedule(schedule_selector(ReachLevelChallengeScene::eogGrayOutLastButton), 0.021f, -1, wrongDelay); // framerate: 1/48
  this->schedule(schedule_selector(ReachLevelChallengeScene::eogReleaseLastButton), 0.021f, 0, wrongDelay); // framerate: 1/48
  this->schedule(schedule_selector(ReachLevelChallengeScene::eogBlinkCorrectButton), 0.2f, -1, correctBlinkDelay); // framerate: 1/48    
}

void ReachLevelChallengeScene::replaySequenceCallback(CCObject* pSender)
{   
  int totalCoins = this->m_pGameContext->getTotalCoins();
  if (totalCoins >= COINS_COST_REPLAY_SEQUENCE)
  {
    totalCoins -= COINS_COST_REPLAY_SEQUENCE;
    this->m_pGameContext->setTotalCoins(totalCoins);
    
    this->m_gameScorePopup->hide();

    runSequenceAnimation(false, 0, -1);
  }
  else
  {
    this->m_gameScorePopup->showMoreCoinsPanel();
  }
}
void ReachLevelChallengeScene::replaySequenceRemainingCallback(CCObject* pSender)
{
  int totalCoins = this->m_pGameContext->getTotalCoins();
  if (totalCoins >= COINS_COST_SHOW_REMAINING)
  {
    totalCoins -= COINS_COST_SHOW_REMAINING;
    this->m_pGameContext->setTotalCoins(totalCoins);
    
    this->m_gameScorePopup->hide();
    
    runSequenceAnimation(false, m_buttonSequenceIndex, -1);
  }
  else
  {
    this->m_gameScorePopup->showMoreCoinsPanel();
  }  
}