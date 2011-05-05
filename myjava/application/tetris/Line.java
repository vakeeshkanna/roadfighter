package myjava.application.tetris;

/**
 * Class representing Line shape
 * @author Waqqas Sharif
 */

public class Line extends Shape
{
	Line(Grid g)
	{
		super();
		this.g = g;
		setName("Line");
		setNumBlocks(4);
		setNumFrames(2);
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
				s = 5;
				for(int i = 0; i < getNumBlocks(); i++) {
					b[i] = new Block();
					b[i].setXIndex(s + i);
					b[i].setYIndex(g.Y_OFFSET - 1);
				}
				
			break;
			
			case 2:
				for(int i = 0; i < getNumBlocks(); i++) {
					b[i] = new Block();
					b[i].setXIndex(7);
					b[i].setYIndex(i);
				}
				
			break;
		}
	}
	
	public void move()
	{	
		int i;
		try {
			switch(getCurrentFrame()) {
				case 1:
					for(i = 0; i < getNumBlocks(); i++) 
						g.mapArray[b[i].getYIndex()][b[i].getXIndex()] = 0; 	
					
					if(b[0].getYIndex() < g.getAY() - 1 && b[1].getYIndex() < g.getAY() - 1
					&& b[2].getYIndex() < g.getAY() - 1 && b[3].getYIndex() < g.getAY() - 1 
					&& g.mapArray[b[0].getYIndex() + 1][b[0].getXIndex()] == 0
					&& g.mapArray[b[1].getYIndex() + 1][b[1].getXIndex()] == 0
					&& g.mapArray[b[2].getYIndex() + 1][b[2].getXIndex()] == 0
					&& g.mapArray[b[3].getYIndex() + 1][b[3].getXIndex()] == 0) {
						for(i = 0; i < getNumBlocks(); i++) 
							b[i].setYIndex(b[i].getYIndex() + 1);
					} else {
						setState(0); 
					}

					for(i = 0; i < getNumBlocks(); i++) 
						g.mapArray[b[i].getYIndex()][b[i].getXIndex()] = getColor(); 
					
				break;
				
				case 2:
					for(i = 0; i < getNumBlocks(); i++) 
						g.mapArray[b[i].getYIndex()][b[i].getXIndex()] = 0;
					
					if(b[3].getYIndex() < g.getAY() - 1 
					 && g.mapArray[b[3].getYIndex() + 1][b[3].getXIndex()] == 0) {
						for(i = 0; i < getNumBlocks(); i++) 
							b[i].setYIndex(b[i].getYIndex() + 1);
					} else {
						setState(0); 
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
					
					if(b[0].getYIndex() < g.getAY() - 1
					&& g.mapArray[b[1].getYIndex() - 1][b[2].getXIndex()] == 0
					&& g.mapArray[b[1].getYIndex()][b[2].getXIndex()] == 0
					&& g.mapArray[b[1].getYIndex() + 1][b[2].getXIndex()] == 0
					&& g.mapArray[b[1].getYIndex() + 2][b[2].getXIndex()] == 0) {
						b[0].setXIndex(b[2].getXIndex());
						b[0].setYIndex(b[1].getYIndex() - 1);
						
						b[1].setXIndex(b[2].getXIndex());
						b[1].setYIndex(b[1].getYIndex());
						
						b[2].setXIndex(b[2].getXIndex());
						b[2].setYIndex(b[1].getYIndex() + 1);
						
						b[3].setXIndex(b[2].getXIndex());
						b[3].setYIndex(b[1].getYIndex() + 2);
						setCurrentFrame(2);
					}
					
					for(i = 0; i < getNumBlocks(); i++) 
						g.mapArray[b[i].getYIndex()][b[i].getXIndex()] = getColor();
				break;
				
				case 2:
					for(i = 0; i < getNumBlocks(); i++) 
						g.mapArray[b[i].getYIndex()][b[i].getXIndex()] = 0; 

					if(b[0].getXIndex() > 1 && b[0].getXIndex() < g.getAX() - 1) {
						if(g.mapArray[b[1].getYIndex()][b[3].getXIndex() -2] == 0
						&& g.mapArray[b[1].getYIndex()][b[3].getXIndex() -1] == 0
						&& g.mapArray[b[1].getYIndex()][b[3].getXIndex()] == 0
						&& g.mapArray[b[1].getYIndex()][b[3].getXIndex() +1] == 0) {
							b[0].setXIndex(b[3].getXIndex() - 2);
							b[0].setYIndex(b[1].getYIndex());
							
							b[1].setXIndex(b[3].getXIndex() - 1);
							b[1].setYIndex(b[1].getYIndex());
							
							b[2].setXIndex(b[3].getXIndex());
							b[2].setYIndex(b[1].getYIndex());
							
							b[3].setXIndex(b[3].getXIndex() + 1);
							b[3].setYIndex(b[1].getYIndex());
							setCurrentFrame(1);
						}
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
						
					if(b[0].getXIndex() > 0 && g.mapArray[b[0].getYIndex()][b[0].getXIndex() - 1] == 0) {
						for(i = 0; i < getNumBlocks(); i++) 
							b[i].setXIndex(b[i].getXIndex() - 1);
					}
					
					for(i = 0; i < getNumBlocks(); i++) 
						g.mapArray[b[i].getYIndex()][b[i].getXIndex()] = getColor();
				break;
				
				case 2:
					for(i = 0; i < getNumBlocks(); i++) 
						g.mapArray[b[i].getYIndex()][b[i].getXIndex()] = 0; 
						
					if(b[0].getXIndex() > 0 && b[1].getXIndex() > 0 && b[2].getXIndex() > 0 && b[3].getXIndex() > 0 
					&& g.mapArray[b[0].getYIndex()][b[0].getXIndex() - 1] == 0 && g.mapArray[b[1].getYIndex()][b[0].getXIndex() - 1] == 0
					&& g.mapArray[b[2].getYIndex()][b[0].getXIndex() - 1] == 0 && g.mapArray[b[3].getYIndex()][b[0].getXIndex() - 1] == 0) {
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
						
					if(b[3].getXIndex() < g.getAX() - 1 && g.mapArray[b[0].getYIndex()][b[3].getXIndex() + 1] == 0) {
						for(i = 0; i < getNumBlocks(); i++) 
							b[i].setXIndex(b[i].getXIndex() + 1);
					}
					
					for(i = 0; i < getNumBlocks(); i++) 
						g.mapArray[b[i].getYIndex()][b[i].getXIndex()] = getColor();
				break;
				
				case 2:
					for(i = 0; i < getNumBlocks(); i++) 
						g.mapArray[b[i].getYIndex()][b[i].getXIndex()] = 0; 
						
					if(b[0].getXIndex() < g.getAX() - 1 && b[1].getXIndex() < g.getAX() - 1 && b[2].getXIndex() < g.getAX() - 1 && b[3].getXIndex() < g.getAX() - 1 
					&& g.mapArray[b[0].getYIndex()][b[0].getXIndex() + 1] == 0 && g.mapArray[b[1].getYIndex()][b[0].getXIndex() + 1] == 0
					&& g.mapArray[b[2].getYIndex()][b[0].getXIndex() + 1] == 0 && g.mapArray[b[3].getYIndex()][b[0].getXIndex() + 1] == 0) {
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
	
	public void setNextArr()
	{
		initNextArr();
		int color = getColor();
		for(int i = 0; i < getNextY(); i++)
			nextArr[i][2] = color;
	}
}