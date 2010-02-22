package myjava.application.tetris;

/**
 * Class representing L shape
 * @author Waqqas Sharif
 */

public class Ell extends Shape
{
	Ell(Grid g)
	{
		super();
		this.g = g;
		setName("Ell");
		setNumBlocks(4);
		setNumFrames(4);
		b = new Block[getNumBlocks()];
		randFrame();
		randColor();
		init();
		setNextArr();
	}
	
	public void init()
	{
		int s;
		switch(getCurrentFrame()) {
			case 1:
				s = Math.round(g.getAX() / 2);
				b[0] = new Block();
				b[0].setXIndex(s);
				b[0].setYIndex(1);
				
				b[1] = new Block();
				b[1].setXIndex(s);
				b[1].setYIndex(2);
				
				b[2] = new Block();
				b[2].setXIndex(s);
				b[2].setYIndex(3);
				
				b[3] = new Block();
				b[3].setXIndex(s + 1);
				b[3].setYIndex(3);
			break;
			
			case 2:
				s = Math.round(g.getAX() / 2);
				b[0] = new Block();
				b[0].setXIndex(s - 1);
				b[0].setYIndex(2);
				
				b[1] = new Block();
				b[1].setXIndex(s);
				b[1].setYIndex(2);
				
				b[2] = new Block();
				b[2].setXIndex(s + 1);
				b[2].setYIndex(2);
				
				b[3] = new Block();
				b[3].setXIndex(s - 1);
				b[3].setYIndex(3);
			break;
			
			case 3:
				s = Math.round(g.getAX() / 2);
				b[0] = new Block();
				b[0].setXIndex(s);
				b[0].setYIndex(1);
				
				b[1] = new Block();
				b[1].setXIndex(s + 1);
				b[1].setYIndex(1);
				
				b[2] = new Block();
				b[2].setXIndex(s + 1);
				b[2].setYIndex(2);
				
				b[3] = new Block();
				b[3].setXIndex(s + 1);
				b[3].setYIndex(3);
			break;
			
			case 4:
				s = Math.round(g.getAX() / 2);
				b[0] = new Block();
				b[0].setXIndex(s + 1);
				b[0].setYIndex(2);
				
				b[1] = new Block();
				b[1].setXIndex(s - 1);
				b[1].setYIndex(3);
				
				b[2] = new Block();
				b[2].setXIndex(s);
				b[2].setYIndex(3);
				
				b[3] = new Block();
				b[3].setXIndex(s + 1);
				b[3].setYIndex(3);
			break;
		}
	}
	
	public void move()
	{
		try {
			int i;
			switch(getCurrentFrame()) {
				case 1:
					for(i = 0; i < getNumBlocks(); i++) 
						g.mapArray[b[i].getYIndex()][b[i].getXIndex()] = 0; 
					
					if(b[2].getYIndex() < g.getAY() - 1 && g.mapArray[b[2].getYIndex() + 1][b[2].getXIndex()] == 0
					&& b[3].getYIndex() < g.getAY() - 1 && g.mapArray[b[3].getYIndex() + 1][b[3].getXIndex()] == 0) {
						for(i = 0; i < getNumBlocks(); i++) 
							b[i].setYIndex(b[i].getYIndex() + 1);
					}
					else {
						setState(0);
					}
					
					
					for(i = 0; i < getNumBlocks(); i++) 
						g.mapArray[b[i].getYIndex()][b[i].getXIndex()] = getColor(); 
				break;
				
				case 2:
					for(i = 0; i < getNumBlocks(); i++) 
						g.mapArray[b[i].getYIndex()][b[i].getXIndex()] = 0; 
					
					if(b[1].getYIndex() < g.getAY() - 1 && g.mapArray[b[1].getYIndex() + 1][b[1].getXIndex()] == 0
					&& b[2].getYIndex() < g.getAY() - 1 && g.mapArray[b[2].getYIndex() + 1][b[2].getXIndex()] == 0
					&& b[3].getYIndex() < g.getAY() - 1 && g.mapArray[b[3].getYIndex() + 1][b[3].getXIndex()] == 0) {
						for(i = 0; i < getNumBlocks(); i++) 
							b[i].setYIndex(b[i].getYIndex() + 1);
					}
					else {
						setState(0);
					}
					
					for(i = 0; i < getNumBlocks(); i++) 
						g.mapArray[b[i].getYIndex()][b[i].getXIndex()] = getColor(); 
				break;
				
				case 3:
					for(i = 0; i < getNumBlocks(); i++) 
						g.mapArray[b[i].getYIndex()][b[i].getXIndex()] = 0; 
						
					if(b[0].getYIndex() < g.getAY() - 1 && g.mapArray[b[0].getYIndex() + 1][b[0].getXIndex()] == 0
						 && b[3].getYIndex() < g.getAY() - 1 && g.mapArray[b[3].getYIndex() + 1][b[3].getXIndex()] == 0) {
						for(i = 0; i < getNumBlocks(); i++) 
							b[i].setYIndex(b[i].getYIndex() + 1);
					}
					else {
						setState(0);
					}
				
					for(i = 0; i < getNumBlocks(); i++) 
						g.mapArray[b[i].getYIndex()][b[i].getXIndex()] = getColor(); 
				break;
				
				case 4:
					for(i = 0; i < getNumBlocks(); i++)
						g.mapArray[b[i].getYIndex()][b[i].getXIndex()] = 0; 
					
					if(b[1].getYIndex() < g.getAY() - 1 && g.mapArray[b[1].getYIndex() + 1][b[1].getXIndex()] == 0
					&& b[2].getYIndex() < g.getAY() - 1 && g.mapArray[b[2].getYIndex() + 1][b[2].getXIndex()] == 0
					&& b[3].getYIndex() < g.getAY() - 1 && g.mapArray[b[3].getYIndex() + 1][b[3].getXIndex()] == 0) {
						for(i = 0; i < getNumBlocks(); i++) 
							b[i].setYIndex(b[i].getYIndex() + 1);
					}
					else {
						setState(0);
					}

					
					for(i = 0; i < getNumBlocks(); i++) 
						g.mapArray[b[i].getYIndex()][b[i].getXIndex()] = getColor(); 
				break;
			}
		}
		catch(Exception e) {}
	}
	
	public void left()
	{
	int i;
		try {
			switch(getCurrentFrame()) {
				case 1:
					for(i = 0; i < getNumBlocks(); i++) 
						g.mapArray[b[i].getYIndex()][b[i].getXIndex()] = 0; 
						
					if(b[0].getXIndex() > 0 && b[1].getXIndex() > 0  && b[2].getXIndex() > 0
					&& g.mapArray[b[0].getYIndex()][b[0].getXIndex() - 1] == 0
					&& g.mapArray[b[1].getYIndex()][b[1].getXIndex() - 1] == 0
					&& g.mapArray[b[2].getYIndex()][b[2].getXIndex() - 1] == 0) {
						for(i = 0; i < getNumBlocks(); i++) 
							b[i].setXIndex(b[i].getXIndex() - 1);
					}
					
					for(i = 0; i < getNumBlocks(); i++) 
						g.mapArray[b[i].getYIndex()][b[i].getXIndex()] = getColor();
				break;
				
				case 2:
					for(i = 0; i < getNumBlocks(); i++) 
						g.mapArray[b[i].getYIndex()][b[i].getXIndex()] = 0; 
						
					if(b[0].getXIndex() > 0 && g.mapArray[b[0].getYIndex()][b[0].getXIndex() - 1] == 0 
					&& b[3].getXIndex() > 0 && g.mapArray[b[3].getYIndex()][b[3].getXIndex() - 1] == 0) {
						for(i = 0; i < getNumBlocks(); i++) 
							b[i].setXIndex(b[i].getXIndex() - 1);
					}	
						
					for(i = 0; i < getNumBlocks(); i++) 
						g.mapArray[b[i].getYIndex()][b[i].getXIndex()] = getColor();
				break;
				
				case 3:
					for(i = 0; i < getNumBlocks(); i++) 
						g.mapArray[b[i].getYIndex()][b[i].getXIndex()] = 0; 
						
					if(b[0].getXIndex() > 0 && g.mapArray[b[0].getYIndex()][b[0].getXIndex() - 1] == 0
					&& b[2].getXIndex() > 0 && g.mapArray[b[2].getYIndex()][b[2].getXIndex() - 1] == 0
					&& b[3].getXIndex() > 0 && g.mapArray[b[3].getYIndex()][b[3].getXIndex() - 1] == 0) {
						for(i = 0; i < getNumBlocks(); i++)
							b[i].setXIndex(b[i].getXIndex() - 1);
					}	
						
					for(i = 0; i < getNumBlocks(); i++) 
						g.mapArray[b[i].getYIndex()][b[i].getXIndex()] = getColor();
				break;
				
				case 4:
					for(i = 0; i < getNumBlocks(); i++) 
						g.mapArray[b[i].getYIndex()][b[i].getXIndex()] = 0; 
						
					if(b[0].getXIndex() > 0 && g.mapArray[b[0].getYIndex()][b[0].getXIndex() - 1] == 0 
					&& b[1].getXIndex() > 0 && g.mapArray[b[1].getYIndex()][b[1].getXIndex() - 1] == 0) {
						for(i = 0; i < getNumBlocks(); i++) 
							b[i].setXIndex(b[i].getXIndex() - 1);
					}	
						
					for(i = 0; i < getNumBlocks(); i++) 
						g.mapArray[b[i].getYIndex()][b[i].getXIndex()] = getColor();
				break;
			}
		}
		catch(Exception e) {}
	}
	
	public void right()
	{
		int i;
		try {
			switch(getCurrentFrame()) {
				case 1:
					for(i = 0; i < getNumBlocks(); i++) 
						g.mapArray[b[i].getYIndex()][b[i].getXIndex()] = 0; 
						
					if(b[0].getXIndex() < g.getAX() - 1 && b[1].getXIndex() < g.getAX() - 1 && b[3].getXIndex() < g.getAX() - 1 
					&& g.mapArray[b[0].getYIndex()][b[0].getXIndex() + 1] == 0
					&& g.mapArray[b[1].getYIndex()][b[1].getXIndex() + 1] == 0
					&& g.mapArray[b[3].getYIndex()][b[3].getXIndex() + 1] == 0) {
						for(i = 0; i < getNumBlocks(); i++) 
							b[i].setXIndex(b[i].getXIndex() + 1);
					}
					
					for(i = 0; i < getNumBlocks(); i++) 
						g.mapArray[b[i].getYIndex()][b[i].getXIndex()] = getColor();
				break;
				
				case 2:
					for(i = 0; i < getNumBlocks(); i++) 
						g.mapArray[b[i].getYIndex()][b[i].getXIndex()] = 0; 
						
					if(b[2].getXIndex() < g.getAX() - 1 && g.mapArray[b[2].getYIndex()][b[2].getXIndex() + 1] == 0
					&& b[3].getXIndex() < g.getAX() - 1 && g.mapArray[b[3].getYIndex()][b[3].getXIndex() + 1] == 0) {
						for(i = 0; i < getNumBlocks(); i++) 
							b[i].setXIndex(b[i].getXIndex() + 1);
					}	
						
					for(i = 0; i < getNumBlocks(); i++) 
						g.mapArray[b[i].getYIndex()][b[i].getXIndex()] = getColor();
				break;
				
				case 3:
					for(i = 0; i < getNumBlocks(); i++) 
						g.mapArray[b[i].getYIndex()][b[i].getXIndex()] = 0; 
						
					if(b[1].getXIndex() < g.getAX() - 1 && g.mapArray[b[1].getYIndex()][b[1].getXIndex() + 1] == 0
					&& b[2].getXIndex() < g.getAX() - 1 && g.mapArray[b[2].getYIndex()][b[2].getXIndex() + 1] == 0
					&& b[3].getXIndex() < g.getAX() - 1 && g.mapArray[b[3].getYIndex()][b[3].getXIndex() + 1] == 0) {
						for(i = 0; i < getNumBlocks(); i++)
							b[i].setXIndex(b[i].getXIndex() + 1);
					}	
						
					for(i = 0; i < getNumBlocks(); i++) 
						g.mapArray[b[i].getYIndex()][b[i].getXIndex()] = getColor();
				break;
				
				case 4:
					for(i = 0; i < getNumBlocks(); i++) 
						g.mapArray[b[i].getYIndex()][b[i].getXIndex()] = 0; 
						
					if(b[0].getXIndex() < g.getAX() - 1 && g.mapArray[b[0].getYIndex()][b[0].getXIndex() + 1] == 0
					&& b[3].getXIndex() < g.getAX() - 1 && g.mapArray[b[3].getYIndex()][b[3].getXIndex() + 1] == 0) {
						for(i = 0; i < getNumBlocks(); i++) 
							b[i].setXIndex(b[i].getXIndex() + 1);
					}	
						
					for(i = 0; i < getNumBlocks(); i++) 
						g.mapArray[b[i].getYIndex()][b[i].getXIndex()] = getColor();
				break;
			}
		}
		catch(Exception e) {}
	}
	
	public void rotate()
	{
		int i;
		try {
			switch(getCurrentFrame()) {
				case 1:
					for(i = 0; i < getNumBlocks(); i++) 
						g.mapArray[b[i].getYIndex()][b[i].getXIndex()] = 0; 
					
					if(b[0].getXIndex() > 0 && b[3].getXIndex() < g.getAX() - 1
					&& g.mapArray[b[0].getYIndex() + 1][b[0].getXIndex()] == 0
					&& g.mapArray[b[1].getYIndex()][b[1].getXIndex() + 1] == 0
					&& g.mapArray[b[2].getYIndex() - 1][b[2].getXIndex() + 2] == 0
					&& g.mapArray[b[3].getYIndex()][b[3].getXIndex() - 1] == 0) {
						b[0].setXIndex(b[0].getXIndex());
						b[0].setYIndex(b[0].getYIndex() + 1);
						
						b[1].setXIndex(b[1].getXIndex() + 1);
						b[1].setYIndex(b[1].getYIndex());
						
						b[2].setXIndex(b[2].getXIndex() + 2);
						b[2].setYIndex(b[2].getYIndex() - 1);
				
						b[3].setXIndex(b[3].getXIndex() - 1);
						b[3].setYIndex(b[3].getYIndex());
						
						setCurrentFrame(2);
					}
					
					for(i = 0; i < getNumBlocks(); i++) 
						g.mapArray[b[i].getYIndex()][b[i].getXIndex()] = getColor();
				break;
				
				case 2:
					for(i = 0; i < getNumBlocks(); i++) 
						g.mapArray[b[i].getYIndex()][b[i].getXIndex()] = 0; 

					if((b[2].getYIndex() +  1) < g.getAY() - 1 && g.mapArray[b[2].getYIndex() + 1][b[2].getXIndex() - 1] == 0
					&& g.mapArray[b[3].getYIndex() + 1][b[3].getXIndex() + 1] == 0) {
						b[2].setXIndex(b[2].getXIndex() - 1);
						b[2].setYIndex(b[2].getYIndex() + 1);
						
						b[3].setXIndex(b[3].getXIndex() + 1);
						b[3].setYIndex(b[3].getYIndex() + 1);
						
						setCurrentFrame(3);
					}
						
					for(i = 0; i < getNumBlocks(); i++) 
						g.mapArray[b[i].getYIndex()][b[i].getXIndex()] = getColor();
				break;
				
				case 3:
					for(i = 0; i < getNumBlocks(); i++) 
						g.mapArray[b[i].getYIndex()][b[i].getXIndex()] = 0; 

					if(b[1].getXIndex() < g.getAX() - 1
					&& g.mapArray[b[0].getYIndex()][b[0].getXIndex() + 2] == 0
					&& g.mapArray[b[1].getYIndex() + 1][b[1].getXIndex() - 1] == 0
					&& g.mapArray[b[3].getYIndex() - 1][b[3].getXIndex() + 1] == 0) {
						b[0].setXIndex(b[0].getXIndex() + 2);
						
						b[1].setXIndex(b[1].getXIndex() - 1);
						b[1].setYIndex(b[1].getYIndex() + 1);
						
						b[3].setXIndex(b[3].getXIndex() + 1);
						b[3].setYIndex(b[3].getYIndex() - 1);
						
						setCurrentFrame(4);
					}
					
					for(i = 0; i < getNumBlocks(); i++) 
						g.mapArray[b[i].getYIndex()][b[i].getXIndex()] = getColor();
				break;
				
				case 4:
					for(i = 0; i < getNumBlocks(); i++) 
						g.mapArray[b[i].getYIndex()][b[i].getXIndex()] = 0; 

					if(g.mapArray[b[0].getYIndex() - 1][b[0].getXIndex() - 2] == 0
					&& g.mapArray[b[1].getYIndex() - 1][b[1].getXIndex()] == 0
					&& g.mapArray[b[2].getYIndex()][b[2].getXIndex() - 1] == 0
					&& g.mapArray[b[3].getYIndex()][b[3].getXIndex() - 1] == 0) {
						b[0].setXIndex(b[0].getXIndex() - 2);
						b[0].setYIndex(b[0].getYIndex() - 1);
						
						b[1].setYIndex(b[1].getYIndex() - 1);
						
						b[2].setXIndex(b[2].getXIndex() - 1);
						
						b[3].setXIndex(b[3].getXIndex() - 1);
						
						setCurrentFrame(1);
					}
						
					for(i = 0; i < getNumBlocks(); i++) 
						g.mapArray[b[i].getYIndex()][b[i].getXIndex()] = getColor();
				break;
			}
		}
		catch(Exception e) {}
	}
	
	public void setNextArr()
	{
		initNextArr();
		int color = getColor();
		nextArr[1][3] = color;
		nextArr[2][1] = color;
		nextArr[2][2] = color;
		nextArr[2][3] = color;
	}
}