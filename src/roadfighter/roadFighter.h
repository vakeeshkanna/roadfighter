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
#include "credit-class.h"

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
	HELP_SCREEN,
	CREDITS_SCREEN
};

enum roadfighterSounds
{
	//roadfigher
	ROADFIGHER_INTRO_SOUND,
	ROADFIGHER_STAGE_CLEAR_SOUND,
	ROADFIGHER_GAME_CLEAR_SOUND,
	ROADFIGHER_GAME_OVER_SOUND,
	ROADFIGHER_PAUSE_SOUND,
	ROADFIGHTER_START_PREPARE_SOUND,
	ROADFIGHTER_START_GO_SOUND,

	//player
	ROADFIGHER_BRAKES_SOUND,
	ROADFIGHTER_ENGINE_RUNNING_SOUND,
	ROADFIGHER_ENGINE_LOW_SOUND,
	ROADFIGHER_ENGINE_HIGH_SOUND,
	ROADFIGHER_FUEL_LOW_SOUND,
	ROADFIGHER_BONUS_CAR_TAKEN_SOUND,
	ROADFIGHER_SUPERMAN_BONUS_SOUND,

	//truck
	ROADFIGHER_TRUCK_SOUND,

	//cars
	ROADFIGHER_COLLISION_SOUND,
	ROADFIGHER_CAR_DESTORY_SOUND,
	ROADFIGHER_CAR_SLIPPING_SOUND,

	//ending
	ROADFIGHTER_ENDING_SOUND
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
	Logical resourcesAreTampered();
	virtual void init();
	virtual void initSounds();
	virtual void initCredits();
	virtual void reinit();
	virtual void processNextFrame();
	virtual void updateGameLogic(unsigned int ms);
	virtual void processUserInput(double milliseconds);
	virtual void applyGameLogic(double milliseconds);
	virtual void applyGameAI(double milliseconds);
	virtual void applyCollisionDetection();
	virtual void applyGamePhysics(double milliseconds);
	virtual void renderFrame();
	virtual Logical paint();
	virtual Logical draw();
	virtual Logical present();
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
	void showBorder();
	void showCredits(Logical gameCompleted);
	void showHelp();
	Logical isShowingSuperman();
	void setStartingFirstStage(Logical starting);
	Logical isStartingFirstStage();
	void startFirstStage();
	void setSkipCurrentFrame(Logical skip);
	Logical isSkipCurrentFrame();
	void setStageLoaded(Logical loaded);
	Logical isStageLoaded();

	RoadFighterViewport *vp;
	Screen *titleScreen, *courseScreen, *endScreen;

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
	Credit *credits;
	Logical showingSuperman, supermanShownOnce;
	Logical startingFirstStage;
	Logical skipCurrentFrame;
	Logical stageLoaded;

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
