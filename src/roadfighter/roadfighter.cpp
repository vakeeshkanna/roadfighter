#include "renderingengine.h"
#include "bluecar.h"
#include "path-class.h"

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
	endScreen = NULL;
	screenTopLeft.setXY(GetSystemMetrics(SM_CXSCREEN) * 35 / 100, GetSystemMetrics(SM_CYSCREEN) * 30 / 100);
	initSounds();
	initCredits();
	startingFirstStage = yes;
	stageLoaded = no;
}

RoadFighter::~RoadFighter()
{

	if(frameTimer != NULL)
	{
		delete frameTimer;
		frameTimer = NULL;
	}
}

Logical RoadFighter::resourcesAreTampered()
{
	int i = 0;

	int baseSizeOfAllFiles = 740804, sizeOfAllFiles = 0;
	string imagesDir = ROADFIGHTER_IMAGES_DIR;

	imagesDir += "./*";

	vector<string> names = Path::namesOfFilesInsideADirectory(imagesDir);

	//go through all file names inside the IMAGES_DIR, sum up there sizes and test against the expected size
	for(i = 0; i < names.size(); i++)
	{
		string fileName = names.at(i);
		string filePath = ROADFIGHTER_IMAGES_DIR;
		filePath += "/";
		filePath += fileName;

//		lprintf("%s = %d\n", fileName.c_str(), Path::getFileSize(filePath));

		sizeOfAllFiles += Path::getFileSize(filePath);
	}

	lprintf("%d\n", sizeOfAllFiles);

	return (baseSizeOfAllFiles == sizeOfAllFiles ? 1 : 0);


}
void RoadFighter::init()
{
	titleScreen = new Screen(ImageInfo("Title Screen", ROADFIGHTER_IMAGES_DIR, "titlescreen.bmp"), VP->buffer, 0, 0, 256, 224);
	titleScreen->setWaitForKeyPress(yes);

	endScreen = new Screen(ImageInfo("End Screen", ROADFIGHTER_IMAGES_DIR, "creditsscreen.bmp"), VP->buffer, 0, 0, 256, 224);
}

void RoadFighter::reinit()
{
	startingFirstStage = yes;
	showingSuperman = no;
	setRoadFighterStatus(TITLE_SCREEN);
	showTitleScreen();
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
	if(!OSM->isPlaying(ROADFIGHTER_TITLE_SOUND))
	{
		OSM->play(ROADFIGHTER_TITLE_SOUND, yes, yes);
	}

	//clear everything
	RE->clearBuffer(RE->DDrawBack);

	titleScreen->display();
	RenderingEngine::outputText(VP->buffer, 80, 150, RGB(0, 0, 255), "TM and (C) 2010", 6, 13);
	RenderingEngine::outputText(VP->buffer, 80, 170, RGB(0, 254, 0), "Press (H) for Help", 6, 13);
	RenderingEngine::outputText(VP->buffer, 80, 190, RGB(0, 254, 0), "Press (C) for Credits", 6, 13);
	renderFrame();
	RE->flipBuffers(hwnd);
	Sleep(500);

	while(1)
	{
		//wait for ENTER
		if(KEY_DOWN(VK_RETURN))
		{
			OSM->stopAllSounds();
			break;
		}
		//wait for H
		else if(KEY_DOWN(72))
		{
			setRoadFighterStatus(HELP_SCREEN);
			showHelp();
		}
		//wait for C
		else if(KEY_DOWN(67))
		{
			OSM->stopAllSounds();
			setRoadFighterStatus(CREDITS_SCREEN);
			showCredits(no);
		}
		//wait for ESC
		else if(KEY_DOWN(VK_ESCAPE))
		{
			exit(0);
		}
	}
	//titleScreen->wait();
}

void RoadFighter::showScoreScreen()
{
	int score = 0;
	Screen dummyScreen;
	dummyScreen.setWaitForKeyPress(yes);
	string courseMsg = "COURSE       ";
	string scoreMsg = "HI       ";
	char buffer[100];

	itoa(getCurrentStage(), buffer, 10);
	courseMsg += buffer;

	if(player == NULL)
	{
		score = 0;
	}
	else
	{
		score = player->getScore();
	}

	if(getCurrentStage() == 1)
	{
		score = 0;
	}

	RE->clearBuffer(VP->buffer);
	RE->clearAllInternalBuffer();
	RenderingEngine::outputText(VP->buffer, 90, 90, C_WHITE, courseMsg, 6, 13);
	RenderingEngine::outputText(VP->buffer, 90, 120, C_WHITE, scoreMsg, 6, 13);
	RenderingEngine::outputText(VP->buffer, 126, 120, C_WHITE, score, 6, 13, "%06d");

	VPBufferToDXBuffer();
	RE->flipBuffers(hwnd);
	Sleep(500);
	dummyScreen.wait();


}

void RoadFighter::prepareStage(int stageNum)
{
	supermanShownOnce = no;
	showingSuperman = no;
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
		interactiveObjManager->setCurrentStage(getCurrentStage());
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
		player = playerManager->getPlayer();
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
	showBorder();
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
			WSM->play(ROADFIGHER_SUPERMAN_BONUS_SOUND, yes, no);
			player->setScore(player->getScore() + 1000);
		}
	}

	if(WSM->isPlaying(ROADFIGHER_SUPERMAN_BONUS_SOUND))
	{
		RenderingEngine::outputText(VP->buffer, superman->getX(), 5, C_WHITE, "1000", 5, 14, TRANSPARENT);
	}
}

void RoadFighter::showBorder()
{
	//border around the viewport
	RE->drawLine(RE->getPrimary(), screenTopLeft.getX() - BORDER_GAP, screenTopLeft.getY() - BORDER_GAP, screenTopLeft.getX() + STAGE_WIDTH * SCALE_FACTOR + BORDER_GAP, screenTopLeft.getY() - BORDER_GAP, C_WHITE);
	RE->drawLine(RE->getPrimary(), screenTopLeft.getX() - BORDER_GAP, screenTopLeft.getY() + STAGE_HEIGHT * SCALE_FACTOR + BORDER_GAP, screenTopLeft.getX() + STAGE_WIDTH * SCALE_FACTOR + BORDER_GAP, screenTopLeft.getY() + STAGE_HEIGHT * SCALE_FACTOR + BORDER_GAP, C_WHITE);
	RE->drawLine(RE->getPrimary(), screenTopLeft.getX() - BORDER_GAP, screenTopLeft.getY() - BORDER_GAP, screenTopLeft.getX() - BORDER_GAP, screenTopLeft.getY() + STAGE_HEIGHT * SCALE_FACTOR + BORDER_GAP, C_WHITE);
	RE->drawLine(RE->getPrimary(), screenTopLeft.getX() + STAGE_WIDTH * SCALE_FACTOR + BORDER_GAP, screenTopLeft.getY() - BORDER_GAP, screenTopLeft.getX() + STAGE_WIDTH * SCALE_FACTOR + BORDER_GAP, screenTopLeft.getY() + STAGE_HEIGHT * SCALE_FACTOR + BORDER_GAP, C_WHITE);
}

void RoadFighter::showCredits(Logical gameCompleted)
{
	int i = 0, j =0;
	int gapBetweenNames = 20;
	int y = 10;

	vector<string> keys = credits->getKeys();

	string currentKey = "";

	Screen dummyScreen;
	dummyScreen.setWaitForKeyPress(yes);

	endScreen->display();
	OSM->play(ROADFIGHTER_ENDING_SOUND, yes, no);

	for(i = 0; i < keys.size(); i++)
	{
		currentKey = keys.at(i);
		y = 10;
		int numValuesForThisKey = credits->getNumValuesForAKey(currentKey);
		RE->clearBuffer(VP->buffer);
		RE->clearAllInternalBuffer();
		endScreen->display();
		showBorder();
		RenderingEngine::outputText(VP->buffer, 10, y, CREF_WHITE, currentKey, 6, 16);

		for(j = 0; j < numValuesForThisKey; j++)
		{
			RenderingEngine::outputText(VP->buffer, 24, y + (j + 1) * 20, CREF_BLUE, credits->getValue(currentKey, j).c_str(), 5, 14);
		}
		VPBufferToDXBuffer();
		RE->flipBuffers(hwnd);
		Sleep(10000);
	}

	while(OSM->isPlaying(ROADFIGHTER_ENDING_SOUND))
	{
		//wait for Q
		if(KEY_DOWN(81))
		{
			OSM->stopAllSounds();
			break;
		}
	}
	reinit();
	//setStageLoaded(no);
}

void RoadFighter::showHelp()
{
	RE->clearBuffer(VP->buffer);
	RE->clearAllInternalBuffer();

	//RE->drawLine(RE->getPrimary(), screenTopLeft.getX() - BORDER_GAP, screenTopLeft.getY() - BORDER_GAP, screenTopLeft.getX() + STAGE_WIDTH * SCALE_FACTOR + BORDER_GAP, screenTopLeft.getY() - BORDER_GAP, C_WHITE);
	showBorder();
	RenderingEngine::outputText(VP->buffer, 10, 10, CREF_WHITE, "How to play:", 5, 14);
	RenderingEngine::outputText(VP->buffer, 10, 30, CREF_RED, "Use Left or Right arrow to steer.", 5, 14);
	RenderingEngine::outputText(VP->buffer, 10, 50, CREF_RED, "Use UP arrow key for Speed Mode A.", 5, 14);
	RenderingEngine::outputText(VP->buffer, 10, 70, CREF_RED, "Use DOWN arrow key for Speed Mode B.", 5, 14);
	RenderingEngine::outputText(VP->buffer, 10, 90, CREF_GREEN, "While sliding do countersteering by pressing either,", 5, 14);
	RenderingEngine::outputText(VP->buffer, 10, 110, CREF_GREEN, "UP or DOWN arrow + Opposite Direction key.", 5, 14);
	RenderingEngine::outputText(VP->buffer, 10, 130, CREF_BLUE, "Press (P) to Pause and Unpause game.", 5, 14);
	RenderingEngine::outputText(VP->buffer, 10, 150, CREF_BLUE, "Press (Q) anytime to go to Title screen.", 5, 14);
	RenderingEngine::outputText(VP->buffer, 10, 170, CREF_BLUE, "Press (ESC) on Title screen to quit the game.", 5, 14);
	VPBufferToDXBuffer();
	RE->flipBuffers(hwnd);

	while(1)
	{
		//wait for Q
		if(KEY_DOWN(81))
		{
			break;
		}
	}
	reinit();
}

const unsigned int SCREEN_REFRESH_RATE(1000/60);

void RoadFighter::processNextFrame()
{
	// The wise C++ programmer would put these nasty static variables in
	// a nice class.

	static DWORD currTick = 0;        // time right now
	static DWORD lastUpdate = 0;      // previous time
	static DWORD lastDraw = 0;        // last time the game rendered
	static bool runFullSpeed = false; // set to true if you want to run full speed

	// Figure how much time has passed since the last time
	currTick = timeGetTime();

	updateGameLogic(currTick - lastUpdate);
	lastUpdate = currTick;

	// It is time to draw ?
	if( runFullSpeed || ( (currTick - lastDraw) > SCREEN_REFRESH_RATE) )
	{
		if(paint())
		{
			// record the last successful paint
			lastDraw = currTick;
		}
	}

	//return S_OK;

}

void RoadFighter::updateGameLogic(unsigned int ms)
{
	//lprintf("ms = %d\n", ms);
	FR->setTimeElapsed(ms);
   //PollInputDevices();
   //HandleUserInput(ms);
	processUserInput(ms);
	applyGameAI(ms);
	applyGameLogic(ms);
	applyCollisionDetection();
	processPlayerState();
   //CalculateAI(ms);
   //ReticulateSplines(ms);
   //RunPhysicsSimulation(ms);
   //PokeAudioSystem(ms);
   //UpdateDisplayList(ms);
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

		//Q to quit to main menu
		if(KEY_DOWN(81))
		{
			WSM->stopAllSounds();
			setStageLoaded(no);
		}
	}

	//if sliding check look for counter-steering
	if(player->isSliding() && !player->isSpinning() && (player->getYPosWC() - player->getLastY()) > 20 && (abs(player->getLastX() - player->getXPosWC())) < 3)
	{
		if(player->AreCounterSteeringButtonsWereReleased())
		{
			if(player->getSlideDirection() == DIRECTION_LEFT)
			{
				if(KEY_DOWN(VK_RIGHT) && KEY_DOWN(VK_DOWN))
				{
					player->straightenCar();
					player->setCounterSteeringButtons(no);
				}
			}
			else if(player->getSlideDirection() == DIRECTION_RIGHT)
			{
				if(KEY_DOWN(VK_LEFT) && KEY_DOWN(VK_DOWN))
				{
					player->straightenCar();
					player->setCounterSteeringButtons(no);
				}
			}
		}
	}

	//keycode for P is 80
	if(KEY_DOWN(80))
	{
		WSM->stopAllSounds();
		Logical on = yes;
		Timer pauseTimer;
		Logical pauseTimerInit = no;

		WSM->play(ROADFIGHER_PAUSE_SOUND, no, no);

		while(1)
		{
			if(KEY_DOWN(VK_ESCAPE))
			{
				showTitleScreen();
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
		WSM->play(ROADFIGHER_PAUSE_SOUND, no, no);
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

		if(WSM->isPlaying(ROADFIGHER_BONUS_CAR_TAKEN_SOUND))
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

Logical RoadFighter::paint()
{
	if(!draw() || !present())
	{
		// insert code to handle drawing problems hereÖsuch as mode changes…
		return no;
	}

	return yes;

}

Logical RoadFighter::draw()
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

		if(WSM->isPlaying(ROADFIGHER_BONUS_CAR_TAKEN_SOUND))
		{
			static int lastX = player->getXPosSC(), lastY = player->getYPosSC() - 50;

			RenderingEngine::outputText(VP->buffer, lastX, lastY, C_WHITE, "1000", 5, 14, TRANSPARENT);
		}
	}
	return yes;
}

Logical RoadFighter::present()
{
	VPBufferToDXBuffer();
	return yes;
}

void RoadFighter::processPlayerState()
{
	//fuel management
	static Timer fuelTimer;
	fuelTimer.forceTickBasedTimer();
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
		WSM->stopAllSounds();

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
		WSM->play(ROADFIGHER_GAME_OVER_SOUND, no, no);
		stageLoaded = no;
		player->setScore(0);
		return;
	}
	int tElapsed = fuelTimer.getTicks();

	// 1 min = 100 fuel
	if(tElapsed > 1000 / (100 / 60) && player->isCanDeductFuel())
	{
		player->deductFuel(1);
		if(player->getCarState() == CAR_RUNNING && player->getSpeed() > 0	)
		{
			if(player->getSpeedMode() == SPEED_MODE_A)
			{
				player->setScore(player->getScore() + 50);
			}
			else
			{
				player->setScore(player->getScore() + 100);
			}
		}
		timerInitialized = no;
	}
	if(player->isRunningLowOnFuel() && player->isCanDeductFuel())
	{
		WSM->play(ROADFIGHER_FUEL_LOW_SOUND, yes, yes);
	}
	else
	{
		if(WSM->isPlaying(ROADFIGHER_FUEL_LOW_SOUND))
		{
			WSM->stop(ROADFIGHER_FUEL_LOW_SOUND);
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
		WSM->stopAllSounds();
		if(getCurrentStage() == ROADFIGHTER_STAGE_4)
		{
			//play game ending sound
			WSM->play(ROADFIGHER_GAME_CLEAR_SOUND, yes, no);
		}
		else
		{
			//play stage clear sound
			WSM->play(ROADFIGHER_STAGE_CLEAR_SOUND, yes, no);
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

	if(WSM->isPlaying(ROADFIGHER_ENGINE_LOW_SOUND))
	{
		WSM->stop(ROADFIGHER_ENGINE_LOW_SOUND);
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

	WSM->play(ROADFIGHER_INTRO_SOUND, no, no);
	WSM->play(ROADFIGHTER_START_PREPARE_SOUND, no, no);
	WSM->play(ROADFIGHTER_START_PREPARE_SOUND, no, no);
	WSM->play(ROADFIGHTER_START_PREPARE_SOUND, no, no);
	WSM->play(ROADFIGHTER_START_GO_SOUND, yes, no);

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

	WSM->play(ROADFIGHER_ENGINE_LOW_SOUND, yes, yes);
	player->setCanControl(yes);
	player->setCanDeductFuel(yes);
	startingFirstStage = no;
}

void RoadFighter::initSounds()
{
	//roadfighter
	WSM->addSound(ROADFIGHER_INTRO_SOUND, ROADFIGHTER_SOUNDS_DIR, "roadfighter_intro");
	WSM->addSound(ROADFIGHER_STAGE_CLEAR_SOUND, ROADFIGHTER_SOUNDS_DIR, "roadfighter_stage_clear");
	WSM->addSound(ROADFIGHER_GAME_CLEAR_SOUND, ROADFIGHTER_SOUNDS_DIR, "roadfighter_game_clear");
	WSM->addSound(ROADFIGHER_GAME_OVER_SOUND, ROADFIGHTER_SOUNDS_DIR, "roadfighter_gameover");
	WSM->addSound(ROADFIGHER_PAUSE_SOUND, ROADFIGHTER_SOUNDS_DIR, "roadfighter_pause");
	WSM->addSound(ROADFIGHTER_START_PREPARE_SOUND, ROADFIGHTER_SOUNDS_DIR, "roadfighter_start_prepare");
	WSM->addSound(ROADFIGHTER_START_GO_SOUND, ROADFIGHTER_SOUNDS_DIR, "roadfighter_start_go");

	//player
	WSM->addSound(ROADFIGHER_BRAKES_SOUND, ROADFIGHTER_SOUNDS_DIR, "roadfighter_brakes");
	WSM->addSound(ROADFIGHTER_ENGINE_RUNNING_SOUND, ROADFIGHTER_SOUNDS_DIR, "roadfighter_engine_sound_running");
	WSM->addSound(ROADFIGHER_ENGINE_LOW_SOUND, ROADFIGHTER_SOUNDS_DIR, "roadfighter_engine_sound_low");
	WSM->addSound(ROADFIGHER_ENGINE_HIGH_SOUND, ROADFIGHTER_SOUNDS_DIR, "roadfighter_engine_sound_high");
	WSM->addSound(ROADFIGHER_FUEL_LOW_SOUND, ROADFIGHTER_SOUNDS_DIR, "roadfighter_fuel_low");
	WSM->addSound(ROADFIGHER_BONUS_CAR_TAKEN_SOUND, ROADFIGHTER_SOUNDS_DIR, "roadfighter_bonus_car_taken");
	WSM->addSound(ROADFIGHER_SUPERMAN_BONUS_SOUND, ROADFIGHTER_SOUNDS_DIR, "roadfighter_superman_bonus");

	//truck
	WSM->addSound(ROADFIGHER_TRUCK_SOUND, ROADFIGHTER_SOUNDS_DIR, "roadfighter_truck");

	//cars
	WSM->addSound(ROADFIGHER_COLLISION_SOUND, ROADFIGHTER_SOUNDS_DIR, "roadfighter_collision");
	WSM->addSound(ROADFIGHER_CAR_DESTORY_SOUND, ROADFIGHTER_SOUNDS_DIR, "roadfighter_car_destroy");
	WSM->addSound(ROADFIGHER_CAR_SLIPPING_SOUND, ROADFIGHTER_SOUNDS_DIR, "roadfighter_car_slipping");

	//title
	OSM->addSound(ROADFIGHTER_TITLE_SOUND, ROADFIGHTER_SOUNDS_DIR, "roadfighter_title");

	//ending
	OSM->addSound(ROADFIGHTER_ENDING_SOUND, ROADFIGHTER_SOUNDS_DIR, "roadfighter_ending");
}

void RoadFighter::initCredits()
{
	credits = new Credit();

	//Credits
	credits->addNewEntry("Credits", "");

	//Original Concept
	credits->addNewEntry("Original Concept", "Waqqas Sharif");

	//Artwork
	credits->addNewEntry("Artwork", "Fahad Yousuf");
	credits->addNewEntry("Artwork", "Afnan Ahmed");

	//AI
	credits->addNewEntry("AI", "Waqqas Sharif");

	//2D Rendering Engine
	credits->addNewEntry("2D Rendering Engine", "Waqqas Sharif");

	//Sound Engine
	credits->addNewEntry("Sound Engine", "Waqqas Sharif");

	//Programming
	credits->addNewEntry("Programming", "Waqqas Sharif");

	//Level Design
	credits->addNewEntry("Level Design", "Waqqas Sharif");

	//Interface Design
	credits->addNewEntry("Interface Design", "Waqqas Sharif");

	//Build Management
	credits->addNewEntry("Build Management", "Waqqas Sharif");

	//Source Management
	credits->addNewEntry("Source Management", "Waqqas Sharif");

	//Debugging
	credits->addNewEntry("Debugging", "Waqqas Sharif");

	//Testing
	credits->addNewEntry("Testing", "Waqqas Sharif");
	credits->addNewEntry("Testing", "Fahad Yousuf");
	credits->addNewEntry("Testing", "Afnan Ahmed");
	credits->addNewEntry("Testing", "Naveed Ghafoor");

	//Produced By
	credits->addNewEntry("Produced By", "Waqqas Sharif");

	//Directed By
	credits->addNewEntry("Directed By", "Waqqas Sharif");

	//MappyLibrary
	credits->addNewEntry("Tiling Library", "Roadfighter uses Mappy Tiling Library");
	credits->addNewEntry("Tiling Library", "http://www.tilemap.co.uk/mappy.php");

	//OGG Library
	credits->addNewEntry("OGG Sound Library", "Roadfighter uses OGG Sound Library ");
	credits->addNewEntry("OGG Sound Library", "http://xiph.org/downloads/");

	//Title Theme
	credits->addNewEntry("Title Theme", "Stigma (GranTurismo PSP OST)");
	credits->addNewEntry("Title Theme", "Performed by Noisia");

	//Ending Theme
	credits->addNewEntry("Ending Theme", "On the run (Forza Motorsport 3 OST)");
	credits->addNewEntry("Ending Theme", "Performed by The Qemist feat. Jenna");

	//Notice
	credits->addNewEntry("Notice", "Roadfighter is a car-based arcade game");
	credits->addNewEntry("Notice", "developed by Konami and released in 1984.");
	credits->addNewEntry("Notice", "This is just a remake done by a Pakistani");
	credits->addNewEntry("Notice", "programmer for fun. Enjoy !!!!");

	//Thanks
	credits->addNewEntry("Thanks for playing !!!!", "Roadfighter is hosted on Google Projects");
	credits->addNewEntry("Thanks for playing !!!!", "http://code.google.com/p/roadfighter/");
	credits->addNewEntry("Thanks for playing !!!!", "Email comments to vickyrare@google.com");


}

void RoadFighter::setSkipCurrentFrame(Logical skip)
{
	skipCurrentFrame = skip;
}

Logical RoadFighter::isSkipCurrentFrame()
{
	return skipCurrentFrame == yes;
}

void RoadFighter::setStageLoaded(Logical loaded)
{
	stageLoaded = loaded;
}

Logical RoadFighter::isStageLoaded()
{
	return stageLoaded == yes;
}
