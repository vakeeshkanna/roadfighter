package myjava.application.tetris;

/**
 * Block class represents each block square out of which a shape is constructed
 * @author Waqqas Sharif
 */

public class Block
{
	int xIndex,yIndex;
	
	/**
	 * Returns xindex for this block square
	*/
	public int getXIndex()
	{
		return xIndex;
	}
	
	/**
	 * Returns yindex for this block square
	*/
	public int getYIndex()
	{
		return yIndex;
	}
	
	/**
	 * Sets xindex for this block square
	*/
	public void setXIndex(int x)
	{
		xIndex = x;
	}
	
	/**
	 * Sets yindex for this block square
	*/
	public void setYIndex(int y)
	{
		yIndex = y;
	}
}