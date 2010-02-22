package tetris.src;

import java.net.*;
import java.io.*;
import java.util.*;

class TransformThread extends Thread
{
	Tetris t;
	Grid g;
	Shape cs;
    	
	public TransformThread(Tetris ts,Grid grid)
	{
		t = ts;
		g = grid;
		cs = t.getCurrentShape();
	}
	
	public void run()
	{
		while(true)
		{
			if(t.getStatus() == Tetris.RUNNING) {
				if(cs.getState() == 1)
					cs.move();
				else {
					t.checkForLine();
					cs = t.getNextShape();
					t.setControl(cs);
				}
				
			}
			try {
				Thread.sleep(t.sc.getSpeed());
			}
			catch(Exception e) {}
		}
	}
}

