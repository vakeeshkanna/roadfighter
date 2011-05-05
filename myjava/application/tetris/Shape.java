package myjava.application.tetris;

import java.util.Random;

/**
 * Base class for all sort of shapes for tetris
 * @author Waqqas Sharif
 */

public abstract class Shape
{
	Grid g;
	Block b[];
	String name;
	int state,numBlocks,numFrames,currentFrame,color,gameStatus;
	int nextX,nextY,nextArr[][];
	
	/**
	 * Initializes each block of current shape
	*/
	public abstract void init();
	/**
	 * Current shapes is falling down
	*/
	public abstract void move();
	/**
	 * Moves current shape left
	*/
	public abstract void left();
	/**
	 * Moves current shape right
	*/
	public abstract void right();
	/**
	 * Rotate current shape
	*/
	public abstract void rotate();
	/**
	 * Set nextArr for current shape
	*/
	public abstract void setNextArr();
    
	public Shape()
	{
		state = 1;
		nextX = 5;
		nextY = 4;
		nextArr= new int [nextY][nextX];
		gameStatus = 0;
	}

	/**
	 * Returns name
	*/
	public String getName()
	{
		return name;
	}
	
	/**
	 * Returns state
	*/
	public int getState()
	{
		return state;
	}
	
	/**
	 * Returns numBlocks
	*/
	public int getNumBlocks()
	{
		return numBlocks;
	}
	
	/**
	 * Returns numFrames
	*/
	public int getNumFrames()
	{
		return numFrames;
	}
	
	/**
	 * Returns currentFrame
	*/
	
	public int getCurrentFrame()
	{
		return currentFrame;
	}
	
	/**
	 * Returns color
	*/
	public int getColor()
	{
		return color;
	}
	
	/**
	 * Returns nextX
	*/
	public int getNextX()
	{
		return nextX;
	}
	
	/**
	 * Returns nextY
	*/
	public int getNextY()
	{
		return nextY;
	}	
	
	/**
	 * Sets name
	*/
	public void setName(String s)
	{
		name = s;
	}
	
	/**
	 * Sets state
	*/
	public void setState(int s)
	{
		state = s;
	}
	/**
	 * Sets numBlocks
	*/
	public void setNumBlocks(int nb)
	{
		numBlocks = nb;
	}
	/**
	 * Sets name
	*/
	public void setNumFrames(int nf)
	{
		numFrames = nf;
	}
	/**
	 * Sets currentFrame
	*/
	public void setCurrentFrame(int cf)
	{
		currentFrame = cf;
	}
	
	/**
	 * Sets color
	*/
	public void setColor(int c)
	{
		color = c;
	}
	
	/**
	 * Initializes nextArr
	*/
	public void initNextArr()
	{
		for(int i = 0;i < getNextY();i++) {
			for(int j = 0;j < getNextY();j++) {
				nextArr[i][j] = 0;
			}
		}
	}
	
	/**
	 * Randomizes Frame
	*/
	public void randFrame()
	{
		Random rand = new Random();
		currentFrame = 1 + rand.nextInt(getNumFrames());
	}
	
	/**
	 * Randomizes Color
	*/
	public void randColor()
	{
		Random rand = new Random();
		setColor(1 + rand.nextInt(7));
	}
}