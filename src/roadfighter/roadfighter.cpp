#include "renderingengine.h"
#include "bluecar.h"

RoadFighter::RoadFighter()
{
	lprintf("\n\nRoadfighter Stats:\n");
	status = GAME_READY;
	rfStatus = TITLE_SCREEN;
	player = NULL;
	playerManager = NULL;
	frameTimer = new Timer();
	tm = NULL;
	bm = NULL;
	nonInteractiveObjManager = NULL;
	interactiveObjManager = NULL;
	Viewport::setInstance(new RoadFighterViewport(VIEWPORT_WIDTH, VIEWPORT_HEIGHT, BOUNDARY_CELL_SIZE));
	showingSuperman = no;
	supermanShownOnce = no;
	titleScreen = NULL;
	courseScreen = NULL;
	screenTopLeft.setXY(GetSystemMetrics(SM_CXSCREEN) * 35 / 100, GetSystemMetrics(SM_CYSCREEN) * 30 / 100);
	initSounds();
	startingFirstStage = yes;
}

RoadFighter::~RoadFighter()
{

	if(frameTimer != NULL)
	{
		delete frameTimer;
		frameTimer = NULL;
	}
}
void RoadFighter::init()
{
	titleScreen = new Screen(ImageInfo("Title Screen",ROADFIGHTER_IMAGES_DIR, "titlescreen.bmp"), VP->buffer, 0, 0, 256, 224);
	titleScreen->setWaitForKeyPress(yes);
}

void RoadFighter::reinit()
{
	showingSuperman = no;
	supermanShownOnce = no;
	setRoadFighterStatus(TITLE_SCREEN);
	showTitleScreen();
	showScoreScreen();
	setRoadFighterStatus(STAGE1_SCREEN);
	loadStage(1);
	loadPlayer();
	loadObjects();
}

void RoadFighter::setViewableArea(int left, int top, int right, int bottom)
{
	RECT rect_list[1];
	rect_list[0].left = left;
	rect_list[0].top = top;
	rect_list[0].right = right;
	rect_list[0].bottom = bottom;

	if(RE->DDrawAttachClipper(VP->buffer, 1, rect_list) == NULL)
		assert(0);
}

void RoadFighter::setCurrentStage(int stage)
{
	currentStage = stage;
}

int RoadFighter::getCurrentStage()
{
	return currentStage;
}

void RoadFighter::showTitleScreen()
{
	titleScreen->display();
	renderFrame();
	RE->flipBuffers(hwnd);
	Sleep(500);
	titleScreen->wait();
}

void RoadFighter::showScoreScreen()
{
	string name = "";
	string image = "";
	switch(getCurrentStage())
	{
	case 1:
		name = "Course Screen 1";
		image = "course1.bmp";
		break;

	case 2:
		name = "Course Screen 2";
		image = "course2.bmp";
		break;

	case 3:
		name = "Course Screen 3";
		image = "course3.bmp";
		break;

	case 4:
		name = "Course Screen 4";
		image = "course4.bmp";
		break;
	}

	if(courseScreen == NULL)
	{
		courseScreen = new Screen(ImageInfo(name, ROADFIGHTER_IMAGES_DIR, image), VP->buffer, 0, 0, 256, 224);
		courseScreen->setWaitForKeyPress(yes);
	}
	else
	{
		courseScreen->reinit(ImageInfo(name, ROADFIGHTER_IMAGES_DIR, image));
	}

	//clear everything
	RE->clearBuffer(RE->DDrawBack);

	courseScreen->display();
	renderFrame();
	RE->flipBuffers(hwnd);
	Sleep(500);
	courseScreen->wait();
}

void RoadFighter::prepareStage(int stageNum)
{
	supermanShownOnce = no;
	setCurrentStage(stageNum);
	setRoadFighterStatus(SCORE_SCREEN);
	showScoreScreen();

	switch(stageNum)
	{
	case 1:
		setRoadFighterStatus(STAGE1_SCREEN);
		break;

	case 2:
		setRoadFighterStatus(STAGE2_SCREEN);
		break;

	case 3:
		setRoadFighterStatus(STAGE3_SCREEN);
		break;

	case 4:
		setRoadFighterStatus(STAGE4_SCREEN);
		break;
	}
	loadStage(stageNum);
	loadPlayer();
	PC->setCurrentStage(stageNum);
	loadObjects();

	if(stageNum != 1)
	{
		player->setCanControl(yes);
		player->setCanDeductFuel(yes);
	}
}

void RoadFighter::loadStage(int stageNum)
{
	currentStage = stageNum;

	if(bm == NULL)
	{
		bm = new BackGroundManager();
		bm->load(currentStage);
	}
	else
	{
		bm->load(currentStage);
	}

	if(tm == NULL)
	{
		tm = new TrackManager();
		tm->load(currentStage);
	}
	else
	{
		tm->load(currentStage);
	}

	vp = (RoadFighterViewport*)VP;
	vp->setTrack(tm->currentTrack);
	vp->init(bm->currentBackGround->getNumCellsY() * CELL_SIZE);
}

void RoadFighter::loadObjects()
{
	if(nonInteractiveObjManager == NULL)
	{
		nonInteractiveObjManager  = new RoadFighterNonInteractiveObjectManager();
		nonInteractiveObjManager->init();
	}
	else
	{
		nonInteractiveObjManager->reinit();
	}

	if(interactiveObjManager == NULL)
	{
		interactiveObjManager  = new RoadFighterInteractiveObjectManager();
		interactiveObjManager->init();
		interactiveObjManager->spawn();
	}
	else
	{
		interactiveObjManager->setCurrentStage(getCurrentStage());
		interactiveObjManager->reinit();
	}

}

void RoadFighter::loadPlayer()
{
	if(playerManager == NULL)
	{
		playerManager = new PlayerManager();
		playerManager->init();
		player = playerManager->getPlayer();
		lprintf("player numframes = %d\n",player->getNumFrames());
		control = new Control(player);
	}
	else
	{
		playerManager->reinit();
	}
}

void RoadFighter::setGameStatus(gameStatus gs)
{
	status = gs;
}

gameStatus RoadFighter::getGameStatus()
{
	return status;
}

void RoadFighter::setRoadFighterStatus(roadfigherStatus rfs)
{
	rfStatus = rfs;
}

roadfigherStatus RoadFighter::getRoadFighterStatus()
{
	return rfStatus;
}

//Top level method that displays everything
void RoadFighter::showHUD()
{
	showProgress();
	showScore();
	showSpeed();
	showFuel();
	showSuperman();

	//border around the viewport
	RE->drawLine(RE->getPrimary(), screenTopLeft.getX() - BORDER_GAP, screenTopLeft.getY() - BORDER_GAP, screenTopLeft.getX() + STAGE_WIDTH * SCALE_FACTOR + BORDER_GAP, screenTopLeft.getY() - BORDER_GAP, C_WHITE);
	RE->drawLine(RE->getPrimary(), screenTopLeft.getX() - BORDER_GAP, screenTopLeft.getY() + STAGE_HEIGHT * SCALE_FACTOR + BORDER_GAP, screenTopLeft.getX() + STAGE_WIDTH * SCALE_FACTOR + BORDER_GAP, screenTopLeft.getY() + STAGE_HEIGHT * SCALE_FACTOR + BORDER_GAP, C_WHITE);
	RE->drawLine(RE->getPrimary(), screenTopLeft.getX() - BORDER_GAP, screenTopLeft.getY() - BORDER_GAP, screenTopLeft.getX() - BORDER_GAP, screenTopLeft.getY() + STAGE_HEIGHT * SCALE_FACTOR + BORDER_GAP, C_WHITE);
	RE->drawLine(RE->getPrimary(), screenTopLeft.getX() + STAGE_WIDTH * SCALE_FACTOR + BORDER_GAP, screenTopLeft.getY() - BORDER_GAP, screenTopLeft.getX() + STAGE_WIDTH * SCALE_FACTOR + BORDER_GAP, screenTopLeft.getY() + STAGE_HEIGHT * SCALE_FACTOR + BORDER_GAP, C_WHITE);
}

void RoadFighter::showProgress()
{
	//won't matter whether we use Background curRow or Track curRow
	//Compute ProgressCar position first w.r.t stage % completed
	int totalPixelsStage = bm->currentBackGround->getNumCellsY() * CELL_SIZE;
	int totalPixelsProgress = FINISH_FLAG_Y - START_FLAG_Y;
	int pixelsTravelledStage = (int)vp->getCurPixLine();
	int stageCompletedPercentage = (pixelsTravelledStage * 100) / totalPixelsStage;
	int progressCarY = (stageCompletedPercentage * totalPixelsProgress) / 100;
	interactiveObjManager->setStageCompletedPercentage(100 - stageCompletedPercentage);
	nonInteractiveObjManager->getProgressCar()->setY(progressCarY);

//#define DO_LOG_ON_SCREEN
#ifdef DO_LOG_ON_SCREEN
	//1
	RenderingEngine::outputText(VP->buffer,100,25,C_RED,pixelsTravelledStage,5,12);
	RenderingEngine::outputText(VP->buffer,125,25,C_WHITE,vp->getCurRow(),5,12);
	RenderingEngine::outputText(VP->buffer,150,25,C_BLUE,vp->getTotalRows(),5,12);
	//2
	RenderingEngine::outputText(VP->buffer,100,50,C_RED,(int)player->getXPosWC(),5,12);
	RenderingEngine::outputText(VP->buffer,125,50,C_WHITE,(int)vp->track->guardRail[vp->getTotalRows() - player->getCurRow()].left.getX(),5,12);
	RenderingEngine::outputText(VP->buffer,150,50,C_BLUE,(int)vp->getTotalRows() - player->getCurRow(),5,12);
	//3
	RenderingEngine::outputText(VP->buffer,100,75,C_RED,(int)player->getCurRow(),5,12);
	RenderingEngine::outputText(VP->buffer,125,75,C_WHITE,(int)vp->getTotalRows() - player->getCurRow(),5,12);
	RenderingEngine::outputText(VP->buffer,150,75,C_BLUE,(int)player->getWCCurRow(),5,12);
	//4
	RenderingEngine::outputText(VP->buffer,100,100,C_RED,(int)player->getXPosWC(),5,12);
	RenderingEngine::outputText(VP->buffer,125,100,C_WHITE,(int)vp->track->guardRail[vp->getTotalRows() - player->getCurRow()].left.getX() + 9 * BOUNDARY_CELL_SIZE,5,12);
	RenderingEngine::outputText(VP->buffer,150,100,C_BLUE,(int)vp->getTotalRows() - player->getCurRow(),5,12);
	//5
	RenderingEngine::outputText(VP->buffer,100,125,C_RED,(int)player->getYPosWC(),5,12);
	RenderingEngine::outputText(VP->buffer,125,125,C_WHITE,(int)vp->track->guardRail[vp->getTotalRows() - player->getCurRow()].left.getY(),5,12);
#endif
}

void RoadFighter::showScore()
{
	int x = 205, y = 25;
	RenderingEngine::outputText(VP->buffer, x,y,C_WHITE,"1P",5,14);

	x = 205, y = 40;
	RenderingEngine::outputText(VP->buffer, x,y,C_WHITE,player->getScore(),5,14,"%06d");
}

void RoadFighter::showSpeed()
{
	int x = 210, y = 90;
	RenderingEngine::outputText(VP->buffer, x,y,C_WHITE,player->getSpeed(),5,14,"%03d");

	x = 233, y = 90;
	RenderingEngine::outputText(VP->buffer, x,y,C_WHITE,"km/h",4,15);
}

void RoadFighter::showFuel()
{
	int x = 205, y = 150;
	RenderingEngine::outputText(VP->buffer, x,y,C_WHITE,"FUEL",5,9);

	//border around FUEL
	RE->drawLine(VP->buffer, x - 1, y, x + 27, y, C_RED);
	RE->drawLine(VP->buffer, x - 1, y + 8, x + 27, y + 8, C_RED);
	RE->drawLine(VP->buffer, x - 1, y, x - 1, y + 8, C_RED);
	RE->drawLine(VP->buffer, x + 27, y, x + 27, y + 8, C_RED);

	x = 225, y = 163;
	if(player->isRunningLowOnFuel())
	{
		RenderingEngine::outputText(VP->buffer, x,y,C_MAGENTA,player->getFuel(),5,14,"%03d");
	}
	else
	{
		RenderingEngine::outputText(VP->buffer, x,y,C_WHITE,player->getFuel(),5,14,"%03d");
	}
}

void RoadFighter::showSuperman()
{
	Logical supermanCanBeShown;
	if(playerManager->getPlayer()->isFlawless() && !supermanShownOnce)
	{
		supermanCanBeShown = yes;
	}
	else
	{
		supermanCanBeShown = no;
	}

	if(supermanCanBeShown && interactiveObjManager->getStageCompletedPercentage() > 75)
	{
		showingSuperman = yes;
		supermanShownOnce = yes;
	}

	NonInteractiveObject *superman = nonInteractiveObjManager->getSuperman();

	if(showingSuperman && getCurrentStage() != ROADFIGHTER_STAGE_2)
	{
		double supermanCurrentY = superman->getY();
		if(superman->getY() > -35)
		{
			superman->setY(supermanCurrentY - 1);
		}
		else
		{
			showingSuperman = no;
			SM->play(ROADFIGHER_SUPERMAN_BONUS, yes, no);
			player->setScore(player->getScore() + 1000);
		}
	}

	if(SM->isPlaying(ROADFIGHER_SUPERMAN_BONUS))
	{
		RenderingEngine::outputText(VP->buffer, superman->getX(), 5, C_WHITE, "1000", 5, 14, TRANSPARENT);
	}
}

void RoadFighter::processUserInput(double milliseconds)
{
	if(player->isCanControl())
	{
		if(KEY_DOWN(VK_UP) && !KEY_DOWN(VK_SPACE) && !player->isTankEmpty())
		{
			control->accelerateA();
		}
		else if(KEY_DOWN(VK_DOWN) && !KEY_DOWN(VK_SPACE) && !player->isTankEmpty())
		{
			control->accelerateB();
		}
		else
		{
			control->brake();
		}

		if(KEY_DOWN(VK_LEFT))
		{
			control->moveLeft();
		}
		else if(KEY_DOWN(VK_RIGHT))

		{
			control->moveRight();
		}

		if(KEY_DOWN(VK_SPACE))
		{
			control->brake();
		}
	}

	//if sliding check look for counter-steering
	if(player->isSliding() && (player->getYPosWC() - player->getLastY()) > 35 && (abs(player->getLastX() - player->getXPosWC())) < 3)
	{
		if(player->getSlideDirection() == DIRECTION_LEFT)
		{
			if(KEY_DOWN(VK_RIGHT) && KEY_DOWN(VK_DOWN))
			{
				player->straightenCar();
			}
		}
		else if(player->getSlideDirection() == DIRECTION_RIGHT)
		{
			if(KEY_DOWN(VK_LEFT) && KEY_DOWN(VK_DOWN))
			{
				player->straightenCar();
			}
		}
	}

	//keycode for P is 80
	if(KEY_DOWN(80))
	{
		SM->stopAllSounds();
		Logical on = yes;
		Timer pauseTimer;
		Logical pauseTimerInit = no;

		SM->play(ROADFIGHER_PAUSE, no, no);

		while(1)
		{
			if(KEY_DOWN(VK_ESCAPE))
			{
				exit(0);
			}

			if(!pauseTimerInit)
			{
				pauseTimer.init();
				pauseTimer.forceLowResTimer();
				pauseTimer.start();
				pauseTimerInit = yes;
			}

			if(pauseTimer.getTicks() > 500)
			{
				if(on)
				{
					//blink pause
					RenderingEngine::outputText(VP->buffer, 205, 190, C_WHITE,"PAUSE",5,12);
					VPBufferToDXBuffer();
					RE->flipBuffers(hwnd);
				}
				else
				{
					renderFrame();
					RE->flipBuffers(hwnd);
				}
				on = !on;
				pauseTimer.start();
			}

			//keycode for P is 80
			if(KEY_DOWN(80))
			{
				break;
			}
		}
		SM->play(ROADFIGHER_PAUSE, no, no);
		setSkipCurrentFrame(yes);
	}
}

void RoadFighter::applyGameLogic(double milliseconds)
{
	playerManager->update();
	interactiveObjManager->update();
}

void RoadFighter::applyGameAI(double milliseconds)
{

}
void RoadFighter::applyCollisionDetection()
{
	playerManager->checkCollision(interactiveObjManager->carObjects);
	playerManager->checkCollision(interactiveObjManager->otherObjects);
}

void RoadFighter::applyGamePhysics(double milliseconds)
{

}

void RoadFighter::VPBufferToDXBuffer()
{
	RECT dstRect;
	dstRect.top = screenTopLeft.getY();
	dstRect.left = screenTopLeft.getX();
	dstRect.bottom = (dstRect.top + 224 * 2);
	dstRect.right = (dstRect.left + 256 * 2);
	RE->DDrawBack->Blt(&dstRect,VP->buffer,NULL,DDBLTFAST_WAIT | DDBLT_KEYSRC,&RE->ddbltfx);
}

void RoadFighter::renderFrame()
{
	if(getRoadFighterStatus() != TITLE_SCREEN && getRoadFighterStatus() != SCORE_SCREEN)
	{
		//RenderFrame
		//bm->currentBackGround->display();
		bm->currentBackGround->setOffsetXY(0, VP->getCurPixLine());
		bm->currentBackGround->display();
		//vp->display2();
		tm->currentTrack->display();

		showHUD();

		playerManager->display();
		interactiveObjManager->display();
		nonInteractiveObjManager->display();

		if(player->isCompletingStage())
		{
			if(getCurrentStage() == ROADFIGHTER_STAGE_4)
			{
				nonInteractiveObjManager->displayGoal();
			}
			else
			{
				nonInteractiveObjManager->displayCheckpoint();
			}
		}

		if(SM->isPlaying(ROADFIGHER_BONUS_CAR_TAKEN))
		{
			static int lastX = player->getXPosSC(), lastY = player->getYPosSC() - 50;

			RenderingEngine::outputText(VP->buffer, lastX, lastY, C_WHITE, "1000", 5, 14, TRANSPARENT);
		}

		//displaying FPS
		double FPS = 50.0;
		//LONGLONG myTimeElapsed = frameTimer->getTicks();

		//	if(timeElapsedFrame < (1000 / FPS))
		//	{
		//		LONGLONG sleepInterval = (1000.0 / FPS) - timeElapsedFrame;
		//		//	RenderingEngine::outputText(100,500,C_WHITE,sleepInterval,10,15);
		//		Sleep(sleepInterval+100);
		//	}

		int timeElapsed = frameTimer->getTicks();
		int frames = 0;
		if(timeElapsed / 1000.0 > 1000.0)
		{
			frames = 0;
			double fps = frames / (timeElapsed / 1000.0);
		//	RenderingEngine::outputText(VP->buffer, 205, 180, C_WHITE, "FPS", 5, 9);
		//	RenderingEngine::outputText(VP->buffer, 225, 200, C_WHITE, timeElapsed, 5, 14, "%03d");

			//RenderingEngine::outputText(100,400,C_WHITE,fps,10,15);
			//RenderingEngine::outputText(100,500,C_WHITE,frame,10,15);
			frameTimer->start();
		}
		else
			frames++;
	}
	VPBufferToDXBuffer();
}

void RoadFighter::processPlayerState()
{
	//fuel management
	static Timer fuelTimer;
	static Logical stageCompletionSoundPlayed = no;
	static Logical timerInitialized = no;
	if(!timerInitialized)
	{
		fuelTimer.start();
		timerInitialized = yes;
	}

	if(player->isTankEmpty() && player->getSpeed() <= 0)
	{
		renderFrame();
		RE->flipBuffers(hwnd);
		SM->stopAllSounds();

		//display EMPTY!
		RenderingEngine::outputText(VP->buffer, 90, 90, C_MAGENTA, "EMPTY", 6, 9);
		VPBufferToDXBuffer();
		RE->flipBuffers(hwnd);
		Sleep(1000);

		//display GAME OVER
		renderFrame();
		RenderingEngine::outputText(VP->buffer, 90, 100, C_WHITE, "GAME", 6, 9);
		RenderingEngine::outputText(VP->buffer, 90, 110, C_WHITE, "OVER", 6, 9);

		VPBufferToDXBuffer();
		RE->flipBuffers(hwnd);
		SM->play(ROADFIGHER_GAMEOVER, no, no);
		reinit();
		startingFirstStage = yes;
	}
	int tElapsed = fuelTimer.getTicks();

	// 1 min = 100 fuel
	if(tElapsed > 1000 / (100 / 60) && player->isCanDeductFuel())
	{
		player->deductFuel(1);
		timerInitialized = no;
	}
	if(player->isRunningLowOnFuel() && player->isCanDeductFuel())
	{
		SM->play(ROADFIGHER_FUEL_LOW, yes, yes);
	}
	else
	{
		if(SM->isPlaying(ROADFIGHER_FUEL_LOW))
		{
			SM->stop(ROADFIGHER_FUEL_LOW);
		}
	}

	if(player->isCompletingStage() && !stageCompletionSoundPlayed)
	{
		//destroy all other cars on screen
		interactiveObjManager->carManager->destroyAllCars();

		while(1)
		{
			if(interactiveObjManager->carManager->allCarsDestroyed())
			{
				break;
			}
			interactiveObjManager->update();
			renderFrame();
			VPBufferToDXBuffer();
			RE->flipBuffers(hwnd);
		}

		renderFrame();
		VPBufferToDXBuffer();
		RE->flipBuffers(hwnd);
		SM->stopAllSounds();
		if(getCurrentStage() == ROADFIGHTER_STAGE_4)
		{
			//play game ending sound
			SM->play(ROADFIGHER_GAME_CLEAR, yes, no);
		}
		else
		{
			//play stage clear sound
			SM->play(ROADFIGHER_STAGE_CLEAR, yes, no);
		}
		stageCompletionSoundPlayed = yes;
	}

	if(player->isStageCompleted())
	{
		stageCompletionSoundPlayed = no;
	}
}

Logical RoadFighter::isShowingSuperman()
{
	return showingSuperman;
}

PlayerCar* RoadFighter::getPlayer()
{
	return player;
}

void RoadFighter::setStartingFirstStage(Logical starting)
{
	startingFirstStage = starting;
}

Logical RoadFighter::isStartingFirstStage()
{
	if(startingFirstStage && getCurrentStage() == ROADFIGHTER_STAGE_1)
	{
		return yes;
	}
	else
	{
		return no;
	}
}

void RoadFighter::startFirstStage()
{
	int i = 0;

	if(SM->isPlaying(ROADFIGHER_ENGINE_SOUND_LOW))
	{
		SM->stop(ROADFIGHER_ENGINE_SOUND_LOW);
	}

	vector < Car *> cars;
	Car *firstCarFromFirstLane, *firstCarFromSecondLane;
	int numCarsAtStartingLine = 7;

	for(i = 0; i < numCarsAtStartingLine; i++)
	{
		Car *car = new BlueCar("Blue Car");
		cars.push_back(car);
	}

	//set positions
	firstCarFromFirstLane = cars.at(0);

	firstCarFromFirstLane->setXPosWC(player->getXPosWC());
	firstCarFromFirstLane->setYPosWC(player->getYPosWC() + 35);

	cars.at(1)->setXPosWC(firstCarFromFirstLane->getXPosWC());
	cars.at(1)->setYPosWC(firstCarFromFirstLane->getYPosWC() + 35);

	cars.at(2)->setXPosWC(firstCarFromFirstLane->getXPosWC());
	cars.at(2)->setYPosWC(firstCarFromFirstLane->getYPosWC() + 70);

	firstCarFromSecondLane = cars.at(3);

	firstCarFromSecondLane->setXPosWC(player->getXPosWC() + 25);
	firstCarFromSecondLane->setYPosWC(player->getYPosWC() + 15);

	cars.at(4)->setXPosWC(firstCarFromSecondLane->getXPosWC());
	cars.at(4)->setYPosWC(firstCarFromSecondLane->getYPosWC() + 35);

	cars.at(5)->setXPosWC(firstCarFromSecondLane->getXPosWC());
	cars.at(5)->setYPosWC(firstCarFromSecondLane->getYPosWC() + 70);

	cars.at(6)->setXPosWC(firstCarFromSecondLane->getXPosWC());
	cars.at(6)->setYPosWC(firstCarFromSecondLane->getYPosWC() + 105);

	Car *currentCar;

	for(i = 0; i < numCarsAtStartingLine ;i++)
	{
		currentCar = cars.at(i);
		currentCar->display();
	}

	VPBufferToDXBuffer();
	RE->flipBuffers(hwnd);

	SM->play(ROADFIGHER_INTRO, no, no);
	SM->play(ROADFIGHTER_START_PREPARE, no, no);
	SM->play(ROADFIGHTER_START_PREPARE, no, no);
	SM->play(ROADFIGHTER_START_PREPARE, no, no);
	SM->play(ROADFIGHTER_START_GO, yes, no);

	Logical notDone = yes;

	while(notDone)
	{
		renderFrame();
		for(i = 0; i < numCarsAtStartingLine ;i++)
		{
			currentCar = cars.at(i);
			currentCar->display();
			if(i == 0 && currentCar->getYPosSC() == NOT_INSIDE_VIEWPORT)
			{
				notDone = no;
				break;
			}
			currentCar->setYPosWC(currentCar->getYPosWC() + 3);
		}
		VPBufferToDXBuffer();
		RE->flipBuffers(hwnd);
	}

	SM->play(ROADFIGHER_ENGINE_SOUND_LOW, yes, yes);
	player->setCanControl(yes);
	player->setCanDeductFuel(yes);
	startingFirstStage = no;
}

void RoadFighter::initSounds()
{
	//roadfighter
	SM->addSound(ROADFIGHER_INTRO, ROADFIGHTER_SOUNDS_DIR, "roadfighter_intro.wav");
	SM->addSound(ROADFIGHER_STAGE_CLEAR, ROADFIGHTER_SOUNDS_DIR, "roadfighter_stage_clear.wav");
	SM->addSound(ROADFIGHER_GAME_CLEAR, ROADFIGHTER_SOUNDS_DIR, "roadfighter_game_clear.wav");
	SM->addSound(ROADFIGHER_GAMEOVER, ROADFIGHTER_SOUNDS_DIR, "roadfighter_gameover.wav");
	SM->addSound(ROADFIGHER_PAUSE, ROADFIGHTER_SOUNDS_DIR, "roadfighter_pause.wav");
	SM->addSound(ROADFIGHTER_START_PREPARE, ROADFIGHTER_SOUNDS_DIR, "roadfighter_start_prepare.wav");
	SM->addSound(ROADFIGHTER_START_GO, ROADFIGHTER_SOUNDS_DIR, "roadfighter_start_go.wav");

	//player
	SM->addSound(ROADFIGHER_BRAKES, ROADFIGHTER_SOUNDS_DIR, "roadfighter_brakes.wav");
	SM->addSound(ROADFIGHTER_ENGINE_SOUND_RUNNING, ROADFIGHTER_SOUNDS_DIR, "roadfighter_engine_sound_running.wav");
	SM->addSound(ROADFIGHER_ENGINE_SOUND_LOW, ROADFIGHTER_SOUNDS_DIR, "roadfighter_engine_sound_low.wav");
	SM->addSound(ROADFIGHER_ENGINE_SOUND_HIGH, ROADFIGHTER_SOUNDS_DIR, "roadfighter_engine_sound_high.wav");
	SM->addSound(ROADFIGHER_FUEL_LOW, ROADFIGHTER_SOUNDS_DIR, "roadfighter_fuel_low.wav");
	SM->addSound(ROADFIGHER_BONUS_CAR_TAKEN, ROADFIGHTER_SOUNDS_DIR, "roadfighter_bonus_car_taken.wav");
	SM->addSound(ROADFIGHER_SUPERMAN_BONUS, ROADFIGHTER_SOUNDS_DIR, "roadfighter_superman_bonus.wav");

	//truck
	SM->addSound(ROADFIGHER_TRUCK, ROADFIGHTER_SOUNDS_DIR, "roadfighter_truck.wav");

	//cars
	SM->addSound(ROADFIGHER_COLLISION, ROADFIGHTER_SOUNDS_DIR, "roadfighter_collision.wav");
	SM->addSound(ROADFIGHER_CAR_DESTORY, ROADFIGHTER_SOUNDS_DIR, "roadfighter_car_destroy.wav");
	SM->addSound(ROADFIGHER_CAR_SLIPPING, ROADFIGHTER_SOUNDS_DIR, "roadfighter_car_slipping.wav");
}

void RoadFighter::setSkipCurrentFrame(Logical skip)
{
	skipCurrentFrame = skip;
}

Logical RoadFighter::isSkipCurrentFrame()
{
	return skipCurrentFrame == yes;
}
