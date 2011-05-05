package myjava.application.tetris;

import java.util.Random;

public class TetrisModel
{
	int _status;
	Grid _grid;
	Shape _cs,_ns;
	Control _control;
	TetrisScore _score;

	TetrisModel()
	{
		_status = Tetris.IDLE;
		init();
	}
	
	public void setStatus(int s)
	{
		_status = s;
	}
	
	public int getStatus()
	{
		return _status;
	}
	
	public void init()
	{
		_grid = new Grid(Tetris.SIZE_Y,Tetris.SIZE_X);
		_grid.init();
		
		_ns = getNewShape(_grid);
		_cs = getNewShape(_grid);
		
		_control = new Control(_cs);
		_score = new TetrisScore(0,0,0,Tetris.DEFAULT_SPEED);
	}
	
	public void setCurrentShape(Shape s)
	{
		_cs = s;
	}
	
	public Shape getCurrentShape()
	{
		return _cs;
	}
	
	public Shape getNextShape()
	{
		_cs = _ns;
		_ns = getNewShape(_grid);
		return _cs;
	}
	
	private Shape getNewShape(Grid g)
	{
		Random rand = new Random();
		int shapeNum = 1 + rand.nextInt(Tetris.NUM_SHAPES);
		Shape ns = null;
		switch(shapeNum)
		{
			case 1:
				ns = new Line(g);
			break;
			
			case 2:
				ns = new Box(g);
			break;
			
			case 3:
				ns = new Perpendicular(g);
			break;
			
			case 4:
				ns = new Ell(g);
			break;
			
			case 5:
				ns = new Zee(g);
			break;
			
			case 6:
				ns = new EllInvert(g);
			break;
			
			case 7:
				ns = new ZeeInvert(g);
			break;
		}
		return ns;
	}
	
	public void setControl(Control c)
	{
		_control = c;
	}
	
	public Control getControl()
	{
		return _control;
	}
	
	private int getLimit()
	{
		int i,j,limit = _grid.getAY();
		try 
		{
			for(i = 0; i < _grid.getAY(); i++) 
			{
				for(j = 0; j < _grid.getAX(); j++) 
				{
					if(_grid.mapArray[i][j] > 0) 
					{
						limit--;
						break;
					}
				}
			}
		}
		catch(Exception e) {}
		return (_grid.getAY() - limit);
	}
	
	public void checkForLine()
	{
		int i,j,k,limit,count = 0,linesDel = 0;
		limit = _grid.getAY() - getLimit() - 1;
		try {
			for(i = _grid.getAY() - 1; i > limit; i--)
			{
				for(j = 0; j < _grid.getAX(); j++) 
				{
					if(_grid.mapArray[i][j] > 0)
						count++;
				}
				if(count == _grid.getAX()) 
				{
					for(k = 0; k < _grid.getAX(); k++)
						_grid.mapArray[i][k] = 0;
					linesDel++;
				}
				count = 0;
			}
			
			for(int total = 0; total <= linesDel; total++) 
			{
				for(i = _grid.getAY() - 1; i > limit; i--) 
				{
					for(j = 0; j < _grid.getAX(); j++) 
					{
						if(_grid.mapArray[i][j] == 0)
							count++;
					}
					if(count == _grid.getAX()) 
					{
						for(k = 0; k < _grid.getAX(); k++) 
						{
							_grid.mapArray[i][k] = _grid.mapArray[i - 1][k];
							_grid.mapArray[i - 1][k] = 0;
						}
					}
					count = 0;
				}
			}
			
			switch(linesDel)
			{
				case 1:
					_score.setScore(_score.getScore() + TetrisScore.ONE_LINE);
				break;
				
				case 2:
					_score.setScore(_score.getScore() + TetrisScore.TWO_LINES);
				break;
				
				case 3:
					_score.setScore(_score.getScore() + TetrisScore.THREE_LINES);
				break;
				
				case 4:
					_score.setScore(_score.getScore() + TetrisScore.FOUR_LINES);
				break;
				
				default:
				break;
			}

			if(linesDel > 0) {
				_score.setNumLines(_score.getNumLines() + linesDel);
				_score.setSpeed(Tetris.DEFAULT_SPEED - (_score.getLevel() * 15) + 50);
			}
		}
		catch(Exception e) {}
	}
	
	public boolean checkGridFilled()
	{
		boolean filled = false;
		if(_cs.getState() == 0) 
		{
			for(int j = 0; j < _grid.getAX();j++) 
			{
				if(_grid.mapArray[3][j] > 0) 
				{
					filled = true;
					break;
				}
			}
		}
		return filled;
	}
	
	public static void main(String[] args) 
	{
		TetrisModel tm = new TetrisModel();
		TetrisView tv = new TetrisView(tm);
		TetrisController tc = new TetrisController(tm,tv);
		TransformThread tt = new TransformThread(tm);
	
		while(true) 
		{
			if(tm.getStatus() == Tetris.READY) 
			{
				tm.setStatus(Tetris.RUNNING);
				try 
				{
					Thread.sleep(500);
				}
				catch(Exception e) {}
				tm.init();
				if(tt.isAlive())
					tt.stop();
				tt = new TransformThread(tm);
				tt.setPriority(Thread.MAX_PRIORITY);
				tt.start();
			}
			
			while(tm.getStatus() != Tetris.GAME_OVER && tm.getStatus() != Tetris.READY) 
			{
				if(tm.checkGridFilled()) 
				{
					tm.setStatus(Tetris.GAME_OVER);
					tv.repaint();
					break;
				}
				
				if(tm.getStatus() == Tetris.PAUSED) 
				{
					tv.repaint();
					while(tm.getStatus() == Tetris.PAUSED) 
					{
						if(tm.getStatus() == Tetris.RUNNING)
							break;
					}
				}
				tv.repaint();
			}
		}	
	}
}
