package tetris.src;

import java.util.*;

abstract class Shape
{
	Grid g;
	Block b[];
	String name;
	int state,numBlocks,numFrames,currentFrame,color,gameStatus;
	int nextX,nextY,nextArr[][];
	public abstract void init();
	public abstract void move();
	public abstract void left();
	public abstract void right();
	public abstract void rotate();
	public abstract void setNextArr();
    
	public Shape()
	{
		state = 1;
		nextX = 5;
		nextY = 4;
		nextArr= new int [nextY][nextX];
		gameStatus = 0;
	}

	public String getName()
	{
		return name;
	}
	
	public int getState()
	{
		return state;
	}
	
	public int getNumBlocks()
	{
		return numBlocks;
	}
	
	public int getNumFrames()
	{
		return numFrames;
	}
	
	public int getCurrentFrame()
	{
		return currentFrame;
	}
	
	public int getColor()
	{
		return color;
	}
	
	public int getNextX()
	{
		return nextX;
	}
	
	public int getNextY()
	{
		return nextY;
	}	
	
	public void setName(String s)
	{
		name = s;
	}
	
	public void setState(int s)
	{
		state = s;
	}
	
	public void setNumBlocks(int nb)
	{
		numBlocks = nb;
	}
	
	public void setNumFrames(int nf)
	{
		numFrames = nf;
	}
	
	public void setCurrentFrame(int cf)
	{
		currentFrame = cf;
	}
	
	public void setColor(int c)
	{
		color = c;
	}
	
	public void initNextArr()
	{
		for(int i = 0;i < getNextY();i++) {
			for(int j = 0;j < getNextY();j++) {
				nextArr[i][j] = 0;
			}
		}
	}
	public void randFrame()
	{
		Random rand = new Random();
		currentFrame = 1 + rand.nextInt(getNumFrames());
	}
	
	public void randColor()
	{
		Random rand = new Random();
		setColor(1 + rand.nextInt(7));
	}
}