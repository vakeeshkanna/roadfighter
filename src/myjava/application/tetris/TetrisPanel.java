package myjava.application.tetris;

import javax.swing.BorderFactory;
import javax.swing.JPanel;
import java.awt.Graphics;
import java.awt.Color;
import java.awt.Dimension;

/**
 * Responsible for displaying the Tetris panel on which the game is displayed
 * @author Waqqas Sharif
 */

public class TetrisPanel extends JPanel
{
	TetrisModel _tm;
	
	TetrisPanel(TetrisModel tm)
	{
		_tm = tm;
		setBorder(BorderFactory.createTitledBorder("Tetris"));
	}	
	
	public Dimension getPreferredSize() 
	{
    	return new Dimension(350,400);
    }
	
	public void paintComponent(Graphics g)
	{
		super.paintComponent(g);  
		Color c = null;
		g.setColor(Color.BLACK);
		g.drawRect(20,20,301,401);
		for(int i = 0;i < _tm._grid.getAY() - _tm._grid.Y_OFFSET;i++) 
		{
			for(int j = 0;j < _tm._grid.getAX();j++)
			{
				if(_tm._grid.mapArray[i + _tm._grid.Y_OFFSET][j] > 0)
				{
					switch(_tm._grid.mapArray[i + _tm._grid.Y_OFFSET][j]) 
					{
						case 1:
							c = Tetris.C_RED;
						break;
						
						case 2:
							c = Tetris.C_BLUE;
						break;
						
						case 3:
							c = Tetris.C_GREEN;
						break;
						
						case 4:
							c = Tetris.C_TEAL;
						break;
						
						case 5:
							c = Tetris.C_ORANGE;
						break;
						
						case 6:
							c = Tetris.C_GREY;
						break;
						
						case 7:
							c = Tetris.C_PURPLE;
						break;
					}
					g.setColor(c);
					g.fillRect((j * 20) + 22,(i * 20) + 22,17,17);
				}
				else 
				{
					g.setColor(Color.WHITE);
					g.fillRect((j * 20) + 22,(i * 20) + 22,17,17);
				}
			}
		}		

		if(_tm.getStatus() == Tetris.GAME_OVER) 
		{
			g.setColor(Color.BLACK);
			for(int i = 0; i < 9;i++) 
			{
				if(i == 4)
					continue;
				g.fillRect(60 + (i * 20) + 30,210,17,17);
			}
			g.setColor(Color.RED);
			g.drawString("G",91,225);
			g.drawString("a",114,225);
			g.drawString("m",132,225);
			g.drawString("e",152,225);
			g.drawString("O",192,225);
			g.drawString("v",214,225);
			g.drawString("e",234,225);
			g.drawString("r",254,225);
		}
		
		if(_tm.getStatus() == Tetris.PAUSED) 
		{
			g.setColor(Color.BLACK);
			for(int i = 0; i < 7;i++) 
				g.fillRect(80 + (i * 20) + 30,210,17,17);
			g.setColor(Color.RED);
			g.drawString("P",113,225);
			g.drawString("a",134,225);
			g.drawString("u",154,225);
			g.drawString("s",174,225);
			g.drawString("e",194,225);
			g.drawString("d",214,225);
			g.drawString("!",235,225);
		}
	}
}
