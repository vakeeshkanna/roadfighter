package myjava.application.tetris;

import java.awt.*;
import javax.swing.*;

/**
 * Application Eexcutable for Tetris, it defines some constants such as  which are used throughout the application by other classes
 * @author Waqqas Sharif
 */

public class Tetris extends JPanel
{
	static final int SIZE_X = 15, SIZE_Y = 24, NUM_SHAPES = 7;
	static final int IDLE = 0,READY = 1, RUNNING = 2, PAUSED = 3, GAME_OVER = 4;
	static final int DEFAULT_SPEED = 600;
	static final int KEY_UP = 0, KEY_DOWN = 1;
	static final int BLOCK_FALLING = 1;
	static final int P_KEY_CODE = 80;
	static final Color C_RED = new Color(255,12,44),
					   C_BLUE = new Color(30,135,240),
					   C_GREEN = new Color(0,128,0),
					   C_TEAL = new Color(63,128,128),
					   C_ORANGE = new Color(255,128,0),
					   C_GREY = new Color(128,128,128),
					   C_PURPLE = new Color(128,0,128);
	
	public static void main(String[] args) 
	{
		TetrisModel tm = new TetrisModel();
		TetrisView tv = new TetrisView(tm);
		TetrisController tc = new TetrisController(tm,tv);
		TransformThread tt = new TransformThread(tm);
	
		while(true) 
		{
			if(tm.getStatus() == READY) 
			{
				tm.setStatus(RUNNING);
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