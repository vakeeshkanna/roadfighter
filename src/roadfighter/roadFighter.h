#ifndef ROADFIGHTER_H
#define ROADFIGHTER_H
#include "game-class.h"
#include "playercar.h"
#include "playermanager.h"
#include "viewport.h"
#include "roadfighterviewport.h"
#include "stage.h"
#include "string-class.h"
#include "array-class.h"
#include "control.h"
#include "timer-class.h"
#include "trackmanager.h"
#include "backgroundmanager.h"
#include "noninteractive-object.h"
#include "roadfighter-non-interactive-object-manager.h"
#include "roadfighter-interactive-object-manager.h"
#include "screen-class.h"

enum gameStatus
{
	GAME_READY,
	GAME_RUNNING,
	GAME_OVER,
	GAME_PAUSED
};

enum roadfigherStatus
{
	TITLE_SCREEN,
	SCORE_SCREEN,
	STAGE1_SCREEN,
	STAGE2_SCREEN,
	STAGE3_SCREEN,
	STAGE4_SCREEN,
	END_SCREEN,
	CREDITS_SCREEN
};

enum roadfighterSounds
{
	//roadfigher
	ROADFIGHER_INTRO,
	ROADFIGHER_STAGE_CLEAR,
	ROADFIGHER_GAME_CLEAR,
	ROADFIGHER_GAMEOVER,
	ROADFIGHER_PAUSE,
	ROADFIGHTER_START_PREPARE,
	ROADFIGHTER_START_GO,

	//player
	ROADFIGHER_BRAKES,
	ROADFIGHTER_ENGINE_SOUND_RUNNING,
	ROADFIGHER_ENGINE_SOUND_LOW,
	ROADFIGHER_ENGINE_SOUND_HIGH,
	ROADFIGHER_FUEL_LOW,
	ROADFIGHER_BONUS_CAR_TAKEN,
	ROADFIGHER_SUPERMAN_BONUS,

	//truck
	ROADFIGHER_TRUCK,

	//cars
	ROADFIGHER_COLLISION,
	ROADFIGHER_CAR_DESTORY,
	ROADFIGHER_CAR_SLIPPING
};

enum roadfighterStages
{
	ROADFIGHTER_STAGE_1 = 1,
	ROADFIGHTER_STAGE_2,
	ROADFIGHTER_STAGE_3,
	ROADFIGHTER_STAGE_4
};

#define ROADFIGHTER_MAPS_DIR "../maps/"
#define ROADFIGHTER_IMAGES_DIR "../images/"
#define ROADFIGHTER_SOUNDS_DIR "../sounds/"

using namespace std;

class RoadFighter : public Game
{
public:
	Timer *frameTimer;
	int currentStage;

	RoadFighter();
	~RoadFighter();
	virtual void init();
	virtual void initSounds();
	virtual void reinit();
	virtual void processUserInput(double milliseconds);
	virtual void applyGameLogic(double milliseconds);
	virtual void applyGameAI(double milliseconds);
	virtual void applyCollisionDetection();
	virtual void applyGamePhysics(double milliseconds);
	virtual void renderFrame();
	void VPBufferToDXBuffer();
	virtual void processPlayerState();
	void setViewableArea(int left, int top, int right, int bottom);

	Point getScreenTopLeft();
	void setCurrentStage(int stage);
	int getCurrentStage();
	void setGameStatus(gameStatus);
	gameStatus getGameStatus();
	void setRoadFighterStatus(roadfigherStatus);
	roadfigherStatus getRoadFighterStatus();
	PlayerCar* getPlayer();

	void showHUD();
	void prepareStage(int stage);
	void loadStage(int stage);
	void loadObjects();
	void loadPlayer();
	void showTitleScreen();
	void showScoreScreen();
	void showEndScreen();
	void showCreditsScreen();
	Logical isShowingSuperman();
	void setStartingFirstStage(Logical starting);
	Logical isStartingFirstStage();
	void startFirstStage();
	void setSkipCurrentFrame(Logical skip);
	Logical isSkipCurrentFrame();

	RoadFighterViewport *vp;
	Screen *titleScreen, *courseScreen;

private:
	gameStatus status;
	roadfigherStatus rfStatus;
	Point screenTopLeft;
	PlayerCar *player;
	Control *control;
	TrackManager *tm;
	BackGroundManager *bm;
	PlayerManager *playerManager;
	RoadFighterInteractiveObjectManager *interactiveObjManager;
	RoadFighterNonInteractiveObjectManager *nonInteractiveObjManager;
	Logical showingSuperman, supermanShownOnce;
	Logical startingFirstStage;
	Logical skipCurrentFrame;

	void showProgress();
	void showScore();
	void showSpeed();
	void showFuel();
	void showSuperman();
	void spawnCar();
	void spawnBonusCar();
	void spawnMainHole();
};
#endif
