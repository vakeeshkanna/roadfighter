package myjava.application.tetris;

import java.awt.*;
import java.awt.event.*;

/**
 * A wrapper around Dialog, used for displaying AboutDialog
 * @author Waqqas Sharif
 */

public class AboutDialog extends Dialog implements ActionListener
{
	AboutDialog(Frame parent, String title)
	{
		super(parent,title,false);
		setLayout(new FlowLayout());
		setSize(300,200);
		Font f = new Font("Dialog",Font.PLAIN,10);
		setFont(f);
		TextArea ta = new TextArea("\tYATC (Yet Another Tetris Clone)\n\n"+
								    "Originally designed and programmed by Alexey Pazhitnov. (1985)\n\n"+
									"Reprogrammed by Waqqas Sharif. (2006)",9,40,3);
		ta.setEditable(false);
		add(ta);
		Button b;
		add(b = new Button("OK"));
		b.addActionListener(this);
	}
	
	public void actionPerformed(ActionEvent ae)
	{
		dispose();
	}
	
	public void paint(Graphics g)
	{
	
	}
}