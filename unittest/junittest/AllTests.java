package junittest;

import junit.framework.JUnit4TestAdapter;
import junit.framework.Test;
import junit.framework.TestSuite;

public class AllTests 
{
	public static Test suite() 
	{
		TestSuite suite = new TestSuite();
		suite.addTestSuite(TestThatWeGetHelloWorldPrompt.class);
		return new JUnit4TestAdapter(TestThatWeGetHelloWorldPrompt.class);
	}
}
