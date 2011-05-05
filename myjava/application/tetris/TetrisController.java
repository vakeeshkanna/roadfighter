package myjava.application.tetris;

import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;

public class TetrisController
{
	TetrisModel _model;
	TetrisView _view;
	int _keyStatus;
	
	TetrisController(TetrisModel tm, TetrisView tv)
	{
		_model = tm;
		_view = tv;
		_keyStatus = Tetris.KEY_UP;
		
		_view.addKeyListener(new KeyAdapter()
		{
			public void keyReleased(KeyEvent ke)
			{
				if(ke.getKeyCode() == KeyEvent.VK_UP)
				{
					_keyStatus = Tetris.KEY_UP;
				} 
			}
	
			public void keyPressed(KeyEvent ke)
			{
				if(_model.getStatus() == Tetris.RUNNING)
				{
					if(ke.getKeyCode() == KeyEvent.VK_UP)
					{
						if(_keyStatus == Tetris.KEY_UP && _model.getCurrentShape().getState() == Tetris.BLOCK_FALLING) 
						{
							_model.getControl().rotate();
							_keyStatus = Tetris.KEY_DOWN;
						}
					} 
					
					if(ke.getKeyCode() == KeyEvent.VK_LEFT) 
					{
						if(_model.getCurrentShape().getState() == Tetris.BLOCK_FALLING)
							_model.getControl().left();
					}
				
					if(ke.getKeyCode() == KeyEvent.VK_RIGHT) 
					{
						if(_model.getCurrentShape().getState() == Tetris.BLOCK_FALLING)
							_model.getControl().right();
					}
		
					if(ke.getKeyCode() == KeyEvent.VK_DOWN) 
					{
						if(_model.getCurrentShape().getState() == Tetris.BLOCK_FALLING)
							_model.getControl().down();
					} 
				}
				
				//P key for pause
				if(ke.getKeyCode() == Tetris.P_KEY_CODE) 
				{
					if(_model.getStatus() == Tetris.RUNNING)
						_model.setStatus(Tetris.PAUSED);
					else if(_model.getStatus() == Tetris.PAUSED)
						_model.setStatus(Tetris.RUNNING);
				}
			}
		});
		
		_view.addWindowListener(new WindowAdapter()
		{
			public void windowClosing(WindowEvent we)
			{
				System.exit(0);
			}
		});
	}	
}
