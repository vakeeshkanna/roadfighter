package tetris.src;

class Score
{
	static final int NUM_LEVELS = 40;
	int score,numLines,level,speed;
	
	
	Score(int s, int nl, int l,int sp)
	{
		score = s;
		numLines = nl;
		level = l;
		speed = sp;
	}
	
	public int getScore()
	{
		return score;
	}
	
	public int getNumLines()
	{
		return numLines;
	}
	
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
	
	public int getSpeed()
	{
		return speed;
	}
	
	public void setScore(int s)
	{
		score = s;
	}
	
	public void setNumLines(int nl)
	{
		numLines = nl;
	}
	
	public void setLevel(int l)
	{
		level = l;
	}
	
	public void setSpeed(int s)
	{
		speed = s;
	}
}