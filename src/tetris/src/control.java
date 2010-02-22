package tetris.src;

class Control
{
	Shape s;
	
	Control(Shape s)
	{
		this.s = s;
	}
	
	public void rotate()
	{
		s.rotate();
	}
	
	public void down()
	{
		s.move();
	}
	
	public void left()
	{
		s.left();
	}
	
	public void right()
	{
		s.right();
	}
}