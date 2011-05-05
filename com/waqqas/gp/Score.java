package com.waqqas.gp;

/**
 * Base class for all sort of scores
 * @author Waqqas Sharif
 */

public class Score
{
	protected int score,level;
	
	public Score(int s,int l)
	{
		score = s;
		level = l;
	}
	
	/**
	 * Returns current score
	*/
	public int getScore()
	{
		return score;
	}
	
	/**
	 * Returns current level
	*/
	public int getLevel()
	{
		return level;
	}
	
	/**
	 * Sets score
	*/
	public void setScore(int s)
	{
		score = s;
	}
	
	/**
	 * Sets current level
	*/
	public void setLevel(int l)
	{
		level = l;
	}
}