package tetris.src;

import java.awt.*;
import java.awt.event.*;
import java.applet.*;
class HistoryDialog extends Dialog implements ActionListener
{
	HistoryDialog(Frame parent, String title)
	{
		super(parent,title,false);
		setLayout(new FlowLayout());
		setSize(610,420);
		Font f = new Font("Dialog",Font.PLAIN,10);
		setFont(f);
		TextArea ta = new TextArea("\t\t\t\t\tHistory\n\n"+
									"Tetris is a very popular computer puzzle game. It was originally designed and programmed by Alexey Pazhitnov\n"+ 
									"(last name transliterated Pajitnov by The Tetris Company [1]) in 1985, while he was working for Computing\n"+
									"Centre of the Russian Academy of Sciences in Moscow, during the days of the Soviet Union. Pajitnov has cited\n"+
									"pentominoes as a source of inspiration for the game. Its name is derived from the Greek numerical prefix\n"+
									"\"tetra-\" meaning four, as all of the blocks are made up of four segments\n\n"+
									"The game (or one of its many variants) is available for nearly every video game console and computer operating system,\n"+ 
									"as well as on devices such as graphing calculators, mobile phones, and PDAs. Tetris has even appeared as part of an\n"+ 
									"art exhibition on the side of Brown University's 14-story Sciences Library [1]. The game first gained mainstream\n"+
									"exposure and popularity beginning in 1989 when Nintendo released Tetris on its Game Boy system. Tetris consistently\n"+ 
									"appears on lists of the greatest video games of all time.\n\n"+
									"For complete history, see /doc/Tetris.htm or http://en.wikipedia.org/wiki/Tetris",25,95);
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