package myjava.application.tetris;

import javax.swing.BorderFactory;
import javax.swing.JPanel;
import java.awt.Graphics;
import java.awt.Color;
import java.awt.Dimension;

/**
 * Responsible for displaying the Next Shape panel
 * @author Waqqas Sharif
 */
public class TetrisNextShapePanel extends JPanel
{
	TetrisModel _tm;
	TetrisNextShapePanel(TetrisModel tm)
	{
		_tm = tm;
		setBorder(BorderFactory.createTitledBorder("Next"));
	}	
	
	public Dimension getPreferredSize() 
	{
    	return new Dimension(100,120);
    }
	
	public void paintComponent(Graphics g)
	{
		super.paintComponent(g);  
		Color c = null;
		for(int i = 0;i < _tm._ns.getNextY();i++) 
		{
			for(int j = 0;j < _tm._ns.getNextX();j++)
			{
				if(_tm._ns.nextArr[i][j] > 0)
				{
					switch(_tm._ns.nextArr[i][j]) 
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
					g.fillRect((j * 20),(i * 20) + 20,17,17);
				}
			}
		}	
	}
}
