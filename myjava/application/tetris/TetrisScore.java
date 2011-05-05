package myjava.application.tetris;

import com.waqqas.gp.Score;

/**
 * Responsible for Tetris Score
 * @author Waqqas Sharif
 */

public class TetrisScore extends Score
{
	static final int NUM_LEVELS = 40;
	static final int ONE_LINE = 100, TWO_LINES = 300, THREE_LINES = 700, FOUR_LINES = 1500;
	int numLines,speed;
	
	TetrisScore(int s, int nl, int l,int sp)
	{
		super(s,l);
		numLines = nl;
		speed = sp;
	}
	
	/**
	 * Returns number of lines
	*/
	public int getNumLines()
	{
		return numLines;
	}
	
	@Override
	public int getLevel()
	{
		for(int i = 0;i < NUM_LEVELS;i++) {
			if(getNumLines() > ((i + 1) * 10) && getNumLines() < ((i + 1) * 10) + 10) {
				setLevel(i + 1);
				break;
			}
		}
		return level;
	}
	
	/**
	 * Returns speed for falling blocks
	*/
	public int getSpeed()
	{
		return speed;
	}
	
	/**
	 * Sets current level
	*/
	public void setNumLines(int nl)
	{
		numLines = nl;
	}
	
	/**
	 * Sets speed for falling blocks
	*/
	public void setSpeed(int s)
	{
		speed = s;
	}
}