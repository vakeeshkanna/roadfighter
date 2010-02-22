package tetris.src;

import java.awt.*;
//import java.awt.image.*;
import java.awt.event.*;
import javax.swing.*;
import java.awt.event.ActionEvent;
import java.awt.event.WindowEvent;

import java.util.*;

class Tetris extends JFrame
{
	static final int SIZE_X = 15,SIZE_Y = 24,NUM_SHAPES = 7;
	static final int HUD_X = 500,HUD_Y = 460;
	static final int IDLE = 0,READY = 1, RUNNING = 2 , PAUSED = 3, GAME_OVER = 4;
	static final int DEFAULT_SPEED = 600;
	static final int KEY_UP = 0, KEY_DOWN = 1;
	int status,keyStatus = KEY_UP;
	boolean timeToDraw = true;
	Grid grid;
	Shape cs,ns;
	Control c;
	Score sc;
	HistoryDialog hd = new HistoryDialog(this,"Game History");
	AboutDialog ad = new AboutDialog(this,"About");
	Color RED,BLUE,GREEN,TEAL,ORANGE,GREY,PURPLE;
	Tetris(Grid g)
	{
		super("YATC (Yet Another Tetris Clone)");
		
		status = IDLE;
		grid = g;
		RED = new Color(255,12,44);
		BLUE = new Color(30,135,240);
		GREEN = new Color(0,128,0);
		TEAL = new Color(63,128,128);
		ORANGE = new Color(255,128,0);
		GREY = new Color(128,128,128);
		PURPLE = new Color(128,0,128);
		init();
		this.setResizable(false);
		this.show(true);
		this.setSize(HUD_X,HUD_Y);
		addKeyListener(new KeyAdapter()
		{
			public void keyReleased(KeyEvent ke)
			{
				if(ke.getKeyCode() == KeyEvent.VK_UP)
				{
					keyStatus = KEY_UP;
				} 
			}
	
			public void keyPressed(KeyEvent ke)
			{
				if(ke.getKeyCode() == KeyEvent.VK_UP)
				{
					if(keyStatus == KEY_UP && cs.getState() == 1) {
						c.rotate();
						keyStatus = KEY_DOWN;
					}
				} 
				
				if(ke.getKeyCode() == KeyEvent.VK_LEFT) 
				{
					if(cs.getState() == 1)
						c.left();
				}
			
				if(ke.getKeyCode() == KeyEvent.VK_RIGHT) 
				{
					if(cs.getState() == 1)
						c.right();
				}
	
				if(ke.getKeyCode() == KeyEvent.VK_DOWN) 
				{
					if(cs.getState() == 1)
						c.down();
				} 
				
				//P key for pause
				if(ke.getKeyCode() == 80) 
				{
					if(getStatus() == RUNNING)
						setStatus(PAUSED);
					else if(getStatus() == PAUSED)
						setStatus(RUNNING);
				} 
			}
		});
		
		addWindowListener(new WindowAdapter()
		{
			public void windowClosing(WindowEvent we)
			{
				System.exit(0);
			}
		});
	}
	
	public void setStatus(int s)
	{
		status = s;
	}
	
	public int getStatus()
	{
		return status;
	}
	
	private void init()
	{
		grid.init();
		ns = getNextShape(grid);
		cs = getNextShape(grid);
		c = new Control(cs);
		sc = new Score(0,0,0,DEFAULT_SPEED);

		MenuBar mb = new MenuBar();
		setMenuBar(mb);
		Menu file = new Menu("File");
		Menu gamestate =  new Menu("Game State");
		Menu about = new Menu("About");
		MenuItem item1,item2,item3,item4,item5;
		file.add(item1 = new MenuItem("New Game..."));
		file.add(item2 = new MenuItem("Exit Game..."));
		mb.add(file);
		
		gamestate.add(item3 = new MenuItem("Pause Game..."));
		mb.add(gamestate);
		
		about.add(item4 = new MenuItem("History..."));
		about.add(item5 = new MenuItem("About..."));
		mb.add(about);
		
		item1.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent ae)
			{
				String arg = (String)ae.getActionCommand();
				if(arg.equals("New Game..."))
					setStatus(READY);
			}
		});
		
		item2.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent ae)
			{
				String arg = (String)ae.getActionCommand();
				if(arg.equals("Exit Game..."))
					System.exit(0);
			}
		});
		
		item3.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent ae)
			{
				String arg = (String)ae.getActionCommand();
				if(arg.equals("Pause Game...")) {
					if(getStatus() == RUNNING)
						setStatus(PAUSED);
					else if(getStatus() == PAUSED)
						setStatus(RUNNING);
				}
			}
		});
		
		item4.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent ae)
			{
				String arg = (String)ae.getActionCommand();
				if(arg.equals("History...")) {
					hd.setVisible(true);
					if(getStatus() == RUNNING)
						setStatus(PAUSED);
				}
			}
		});
		
		item5.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent ae)
			{
				String arg = (String)ae.getActionCommand();
				if(arg.equals("About...")) {
					ad.setVisible(true);
					if(getStatus() == RUNNING)
						setStatus(PAUSED);
				}
			}
		});
		
		Font f = new Font("Dialog",Font.PLAIN,16);
		setFont(f);
	}
	
	public void setControl(Shape s)
	{
		c = new Control(s);
	}
	
	public void setCurrentShape(Shape s)
	{
		cs = s;
	}
	
	public Shape getCurrentShape()
	{
		return cs;
	}
	
	public Shape getNextShape()
	{
		cs = ns;
		ns = getNextShape(grid);
		return cs;
	}
	
	public static Shape getNextShape(Grid g)
	{
		Random rand = new Random();
		int shapeNum = 1 + rand.nextInt(Tetris.NUM_SHAPES);
		//shapeNum = 1;
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
	
	public int getLimit()
	{
		int i,j,limit = grid.getAY();
		try {
			for(i = 0; i < grid.getAY(); i++) {
				for(j = 0; j < grid.getAX(); j++) {
					if(grid.mapArray[i][j] > 0) {
						limit--;
						break;
					}
				}
			}
		}
		catch(Exception e) {}
		return (grid.getAY() - limit);
	}
	
	public void checkForLine()
	{
		int i,j,k,limit,count = 0,linesDel = 0;
		limit = grid.getAY() - getLimit() - 1;
		try {
			for(i = grid.getAY() - 1; i > limit; i--) {
				for(j = 0; j < grid.getAX(); j++) {
					if(grid.mapArray[i][j] > 0)
						count++;
				}
				if(count == grid.getAX()) {
					for(k = 0; k < grid.getAX(); k++)
						grid.mapArray[i][k] = 0;
					linesDel++;
				}
				count = 0;
			}
			
			for(int total = 0; total <= linesDel; total++) {
				for(i = grid.getAY() - 1; i > limit; i--) {
					for(j = 0; j < grid.getAX(); j++) {
						if(grid.mapArray[i][j] == 0)
							count++;
					}
					if(count == grid.getAX()) {
						for(k = 0; k < grid.getAX(); k++) {
							grid.mapArray[i][k] = grid.mapArray[i - 1][k];
							grid.mapArray[i - 1][k] = 0;
						}
					}
					count = 0;
				}
			}
			
			switch(linesDel)
			{
				case 1:
					sc.setScore(sc.getScore() + 100);
				break;
				
				case 2:
					sc.setScore(sc.getScore() + 300);
				break;
				
				case 3:
					sc.setScore(sc.getScore() + 700);
				break;
				
				case 4:
					sc.setScore(sc.getScore() + 1500);
				break;
				
				default:
				break;
			}

			if(linesDel > 0) {
				sc.setNumLines(sc.getNumLines() + linesDel);
				timeToDraw = true;
				sc.setSpeed(DEFAULT_SPEED - (sc.getLevel() * 15) + 50);
			}
		}
		catch(Exception e) {}
	}
	
	public void paint(Graphics g)
	{
		Color c = null;
		g.setColor(Color.black);
		g.drawRect(27,48,301,401);
		for(int i = 0;i < grid.getAY() - grid.Y_OFFSET;i++) {
			for(int j = 0;j < grid.getAX();j++)
			{
				if(grid.mapArray[i + grid.Y_OFFSET][j] > 0)
					{
						switch(grid.mapArray[i + grid.Y_OFFSET][j]) {
							case 1:
								c = RED;
							break;
							
							case 2:
								c = BLUE;
							break;
							
							case 3:
								c = GREEN;
							break;
							
							case 4:
								c = TEAL;
							break;
							
							case 5:
								c = ORANGE;
							break;
							
							case 6:
								c = GREY;
							break;
							
							case 7:
								c = PURPLE;
							break;
						}
						g.setColor(c);
						g.fillRect((j * 20) + 30,(i * 20) + 50,17,17);
					}
				else {
					g.setColor(Color.white);
					g.fillRect((j * 20) + 30,(i * 20) + 50,17,17);
				}
			}
		}		
		g.setColor(Color.blue);
		g.drawString("Next",409,69);
		g.setColor(Color.black);
		g.drawRect(379,79,99,79);
		g.setColor(Color.white);
		
		for(int i = 0;i < ns.getNextY();i++) {
			for(int j = 0;j < ns.getNextX();j++)
			{
				if(ns.nextArr[i][j] > 0)
					{
						switch(ns.nextArr[i][j]) {
							case 1:
								c = RED;
							break;
							
							case 2:
								c = BLUE;
							break;
							
							case 3:
								c = GREEN;
							break;
							
							case 4:
								c = TEAL;
							break;
							
							case 5:
								c = ORANGE;
							break;
							
							case 6:
								c = GREY;
							break;
							
							case 7:
								c = PURPLE;
							break;
						}
						g.setColor(c);
						g.fillRect(350 + (j * 20) + 30,80 + (i * 20),17,17);
					}
				else {
					g.setColor(Color.white);
					g.fillRect(350 + (j * 20) + 30,80 + (i * 20),17,17);
				}
			}
		}		
		
		g.setColor(Color.blue);
		g.drawString("Score",404,189);
		g.setColor(Color.black);
		g.drawRect(379,199,99,39);
		
		g.setColor(Color.blue);
		g.drawString("Level",408,269);
		g.setColor(Color.black);
		g.drawRect(379,279,99,39);
		
		g.setColor(Color.blue);
		g.drawString("Lines",408,349);
		g.setColor(Color.black);
		g.drawRect(379,359,99,39);
		
		if(timeToDraw) {
			g.setColor(Color.white);
			g.fillRect(380,200,99,39);
			g.setColor(Color.red);
			g.drawString(""+sc.getScore(),410,224);
			
			g.setColor(Color.white);
			g.fillRect(380,280,99,39);
			g.setColor(Color.red);
			g.drawString(""+sc.getLevel(),410,304);
		
			g.setColor(Color.white);
			g.fillRect(380,360,99,39);
			g.setColor(Color.red);
			g.drawString(""+sc.getNumLines(),410,384);
			timeToDraw = false;
		}
		
		if(getStatus() == GAME_OVER) {
			g.setColor(Color.black);
			for(int i = 0; i < 9;i++) {
				if(i == 4)
					continue;
					g.fillRect(60 + (i * 20) + 30,210,17,17);
			}
			g.setColor(Color.red);
			g.drawString("G",91,225);
			g.drawString("a",114,225);
			g.drawString("m",132,225);
			g.drawString("e",152,225);
			g.drawString("O",192,225);
			g.drawString("v",214,225);
			g.drawString("e",234,225);
			g.drawString("r",254,225);
		}
		
		if(getStatus() == PAUSED) {
			g.setColor(Color.black);
			for(int i = 0; i < 7;i++) 
				g.fillRect(80 + (i * 20) + 30,210,17,17);
			g.setColor(Color.red);
			g.drawString("P",113,225);
			g.drawString("a",134,225);
			g.drawString("u",154,225);
			g.drawString("s",174,225);
			g.drawString("e",194,225);
			g.drawString("d",214,225);
			g.drawString("!",235,225);
		}
	}
	
	public boolean checkGridFilled()
	{
		boolean filled = false;
		if(cs.getState() == 0) {
			for(int j = 0; j < grid.getAX();j++) {
				if(grid.mapArray[3][j] > 0) {
					filled = true;
					break;
				}
			}
		}
		return filled;
	}
	
	public static void main(String [] args)
	{
		Grid g = new Grid(SIZE_Y,SIZE_X);
 		Tetris t = new Tetris(g);
		TransformThread tt = new TransformThread(t,g);
	
		while(true) {
			if(t.getStatus() == READY) {
				t.setStatus(RUNNING);
				try {
					Thread.sleep(500);
				}
				catch(Exception e) {}
				t.init();
				t.timeToDraw = true;
				if(tt.isAlive())
					tt.suspend();
				tt = new TransformThread(t,g);
				tt.setPriority(Thread.MAX_PRIORITY);
				tt.start();
			}
			
			while(t.getStatus() != t.GAME_OVER && t.getStatus() != t.READY) {
				if(t.checkGridFilled()) {
					t.setStatus(GAME_OVER);
					t.repaint();
					break;
				}
				
				if(t.getStatus() == PAUSED) {
					t.repaint();
					while(t.getStatus() == PAUSED) {
						if(t.getStatus() == RUNNING)
							break;
					}
				}
				
				t.repaint();
			}
		}
	}
}
