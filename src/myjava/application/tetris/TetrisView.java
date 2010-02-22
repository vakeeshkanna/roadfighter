package myjava.application.tetris;

import java.awt.Color;
import java.awt.Container;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.Menu;
import java.awt.MenuBar;
import java.awt.MenuItem;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import java.awt.GridLayout;
import java.awt.BorderLayout;
import java.awt.Dimension;
import javax.swing.BorderFactory;

public class TetrisView extends JFrame
{
	static final int HUD_X = 500,HUD_Y = 460;
	TetrisModel _model;
	HistoryDialog _hd;
	AboutDialog _ad;
	JLabel nextLineLabel,scoreLabel,levelLabel,linesLabel;

	TetrisView(TetrisModel model)
	{
		_model = model;
		addWindowListener(new WindowAdapter()
		{
			public void windowClosing(WindowEvent we)
			{
				System.exit(0);
			}
		});
		initGUI();
	}
	
	public Dimension getPreferredSize() 
	{
    	return new Dimension(450,490);
    }
	
	public void initGUI()
	{
		setFont(new Font("Dialog",Font.PLAIN,16));
		setTitle("YATC (Yet Another Tetris Clone)");
		setResizable(false);
		
		Container content = getContentPane();
		content.setBackground(Color.lightGray);
		
		_hd = new HistoryDialog(this,"Game History");
		_ad = new AboutDialog(this,"About");
		
		MenuBar mb = new MenuBar();
		setMenuBar(mb);
		
		JPanel scoreArea = new JPanel(new GridLayout(4, 1));
			
		scoreLabel = new JLabel("" + _model._score.getScore(), JLabel.CENTER);
		scoreLabel.setBorder(BorderFactory.createTitledBorder("SCORE"));
		
		levelLabel = new JLabel("Score Current", JLabel.CENTER);
		levelLabel.setBorder(BorderFactory.createTitledBorder("LEVEL"));		
		
		linesLabel = new JLabel("Score Current", JLabel.CENTER);
		linesLabel.setBorder(BorderFactory.createTitledBorder("LINES"));
		
		scoreArea.add(new TetrisNextShapePanel(_model));
		scoreArea.add(scoreLabel);
		scoreArea.add(levelLabel);
		scoreArea.add(linesLabel);
	
		content.add(new TetrisPanel(_model), BorderLayout.CENTER);
	    content.add(scoreArea, BorderLayout.EAST);

	    MenuItem newGame,endGame,pauseGame,history,abt;
		
		Menu file = new Menu("File");
		file.add(newGame = new MenuItem("New Game..."));
		file.add(endGame = new MenuItem("Exit Game..."));
		
		Menu gamestate =  new Menu("Game State");
		gamestate.add(pauseGame = new MenuItem("Pause Game..."));
		
		Menu about = new Menu("About");
		about.add(history = new MenuItem("History..."));
		about.add(abt = new MenuItem("About..."));
		
		mb.add(file);
		mb.add(gamestate);
		mb.add(about);
		
		newGame.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent ae)
			{
				String arg = (String)ae.getActionCommand();
				if(arg.equals("New Game..."))
					_model.setStatus(Tetris.READY);
			}
		});
		
		endGame.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent ae)
			{
				String arg = (String)ae.getActionCommand();
				if(arg.equals("Exit Game..."))
					System.exit(0);
			}
		});
		
		pauseGame.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent ae)
			{
				String arg = (String)ae.getActionCommand();
				if(arg.equals("Pause Game...")) 
				{
					if(_model.getStatus() == Tetris.RUNNING)
						_model.setStatus(Tetris.PAUSED);
					else if(_model.getStatus() == Tetris.PAUSED)
						_model.setStatus(Tetris.RUNNING);
				}
			}
		});
		
		history.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent ae)
			{
				String arg = (String)ae.getActionCommand();
				if(arg.equals("History...")) 
				{
					_hd.setVisible(true);
					if(_model.getStatus() == Tetris.RUNNING)
						_model.setStatus(Tetris.PAUSED);
				}
			}
		});
		
		abt.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent ae)
			{
				String arg = (String)ae.getActionCommand();
				if(arg.equals("About...")) 
				{
					_ad.setVisible(true);
					if(_model.getStatus() == Tetris.RUNNING)
						_model.setStatus(Tetris.PAUSED);
				}
			}
		});
		pack();
	    setVisible(true);
	}
	
	public void setModel(TetrisModel m)
	{
		_model = m;
	}
	
	public TetrisModel getModel()
	{
		return _model;
	}
	
	public void paint(Graphics g)
	{
		super.paint(g);
		scoreLabel.setText(new Integer(_model._score.getScore()).toString());
		levelLabel.setText(new Integer(_model._score.getLevel()).toString());
		linesLabel.setText(new Integer(_model._score.getNumLines()).toString());
	}
	
}
