#ifndef GAME_CLASS_H
#define GAME_CLASS_H

class Game
{
	virtual void init() = 0;
	virtual void initSounds() = 0;
	virtual void processUserInput(double milliseconds) = 0;
	virtual void applyGameLogic(double milliseconds) = 0;
	virtual void applyGameAI(double milliseconds) = 0;
	virtual void applyCollisionDetection() = 0;
	virtual void applyGamePhysics(double milliseconds) = 0;
	virtual void renderFrame() = 0;
	virtual void processPlayerState() = 0;


//	void startGame() = 0;
//	void exitGame() = 0;
//	void pauseGame() = 0;
//	void showTitle() = 0;
//	void showGameover() = 0;
//	void showContinue() = 0;
//	void showCredits() = 0;
//	void showDemo() = 0;
};
#endif
