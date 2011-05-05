package myjava.application.tetris;

public class TransformThread extends Thread
{
	private TetrisModel _tm;
	private Shape _cs;
    	
	public TransformThread(TetrisModel tm)
	{
		_tm = tm;
		_cs = tm.getCurrentShape();
	}
	
	public void run()
	{
		while(true)
		{
			if(_tm.getStatus() == Tetris.RUNNING) 
			{
				if(_cs.getState() == Tetris.BLOCK_FALLING)
				{
					_cs.move();
				}
				else
				{
					_tm.checkForLine();
					_cs = _tm.getNextShape();
					_tm.getControl().setShape(_cs);
				}
			}
			try 
			{
				Thread.sleep(_tm._score.getSpeed());
			}
			catch(Exception e) {}
		}
	}
}

