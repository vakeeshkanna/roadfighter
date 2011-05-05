package myjava.application.tetris;

public class Grid
{
	int aX,aY;
	int mapArray[][];
	int Y_OFFSET = 4;
	
	public Grid(int ay,int ax)
	{
		aX = ax;
		aY = ay;
		mapArray = new int [aY][aX];
	}
	
	public void init()
	{
		int i,j;
		for(i = 0; i < aY; i++) {
			for(j = 0; j < aX; j++) { 
				mapArray[i][j] = 0;
			}
		}
		
	}
	
	public int getAX()
	{
		return aX;
	}
	
	public int getAY()
	{
		return aY;
	}
	
	public void setAX(int ax)
	{
		aX = ax;
	}
	
	public void setAY(int ay)
	{
		aY = ay;
	}
}