package tetris.src;

class Box extends Shape
{
	Box(Grid g)
	{
		super();
		this.g = g;
		setName("Box");
		setNumBlocks(4);
		setNumFrames(1);
		b = new Block[getNumBlocks()];
		randFrame();
		randColor();
		init();
		setNextArr();
	}
	
	public void init()
	{
		int s;
		s = Math.round(g.getAX() / 2);
		b[0] = new Block();
		b[0].setXIndex(s);
		b[0].setYIndex(2);
		
		b[1] = new Block();
		b[1].setXIndex(s + 1);
		b[1].setYIndex(2);
		
		b[2] = new Block();
		b[2].setXIndex(s);
		b[2].setYIndex(3);
		
		b[3] = new Block();
		b[3].setXIndex(s + 1);
		b[3].setYIndex(3);
	}
	
	public void move()
	{	
		try {
			int i,j;
			for(i = 0; i < getNumBlocks(); i++)
				g.mapArray[b[i].getYIndex()][b[i].getXIndex()] = 0; 
			
			if( b[2].getYIndex() < g.getAY() - 1 && g.mapArray[b[2].getYIndex() + 1][b[2].getXIndex()] == 0
			 && b[3].getYIndex() < g.getAY() - 1 && g.mapArray[b[3].getYIndex() + 1][b[3].getXIndex()] == 0) {
				for(i = 0; i < getNumBlocks(); i++) 
					b[i].setYIndex(b[i].getYIndex() + 1);
			}
			else {
				setState(0);
			}
			
			
			for(i = 0; i < getNumBlocks(); i++) 
				g.mapArray[b[i].getYIndex()][b[i].getXIndex()] = getColor(); 
		}
		catch(Exception e) {}
	}
	
	public void rotate()
	{
		
	}
	
	public void left()
	{
		int i;
		try {
			for(i = 0; i < getNumBlocks(); i++) 
				g.mapArray[b[i].getYIndex()][b[i].getXIndex()] = 0; 
				
			if(b[0].getXIndex() > 0 && g.mapArray[b[0].getYIndex()][b[0].getXIndex() - 1] == 0
			&& b[2].getXIndex() > 0 && g.mapArray[b[2].getYIndex()][b[2].getXIndex() - 1] == 0) {
				for(i = 0; i < getNumBlocks(); i++) 
					b[i].setXIndex(b[i].getXIndex() - 1);
			}
			
			for(i = 0; i < getNumBlocks(); i++) 
				g.mapArray[b[i].getYIndex()][b[i].getXIndex()] = getColor();
		}
		catch(Exception e) {}
	}
	
	public void right()
	{
		int i;
		try {
			for(i = 0; i < getNumBlocks(); i++) 
				g.mapArray[b[i].getYIndex()][b[i].getXIndex()] = 0; 
				
			if(b[1].getXIndex() < g.getAX() - 1 && g.mapArray[b[1].getYIndex()][b[1].getXIndex() + 1] == 0
			&& b[3].getXIndex() < g.getAX() - 1 && g.mapArray[b[3].getYIndex()][b[3].getXIndex() + 1] == 0) {
				for(i = 0; i < getNumBlocks(); i++) 
					b[i].setXIndex(b[i].getXIndex() + 1);
			}
			
			for(i = 0; i < getNumBlocks(); i++) 
				g.mapArray[b[i].getYIndex()][b[i].getXIndex()] = getColor();
		}
		catch(Exception e) {}
	}
	
	public void setNextArr()
	{
		initNextArr();
		int color = getColor();
		nextArr[1][1] = color;
		nextArr[1][2] = color;
		nextArr[2][1] = color;
		nextArr[2][2] = color;
	}
}