package myjava.application.tetris;

/**
 * Controls currently falling shape
 * @author Waqqas Sharif
 */

public class Control
{
	Shape s;
	
	Control(Shape s)
	{
		this.s = s;
	}
	
	/**
	 * Rotate current shape
	*/
	public void rotate()
	{
		s.rotate();
	}
	
	/**
	 * Bring current shape down
	*/
	public void down()
	{
		s.move();
	}
	
	/**
	 * Move current shape left
	*/
	public void left()
	{
		s.left();
	}
	
	/**
	 * Move current shape right
	*/
	
	public void right()
	{
		s.right();
	}
	
	/**
	 * Set current shape
	*/
	public void setShape(Shape s)
	{
		this.s = s;
	}
}