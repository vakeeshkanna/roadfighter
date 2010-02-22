package junittest;

import org.junit.After;
import org.junit.AfterClass;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;
import org.junit.Ignore; 
import junit.framework.TestCase;

public class TestThatWeGetHelloWorldPrompt extends TestCase
{
	HelloWorld hw;
	
	@BeforeClass
	public static void setUpBeforeClass() throws Exception 
	{
	}

	@AfterClass
	public static void tearDownAfterClass() throws Exception 
	{
	}

	@Before
	public void setUp() throws Exception 
	{
		hw = new HelloWorld();
	}

	@After
	public void tearDown() throws Exception 
	{
	}

	@Test
	public void testSay()
	{
		assertEquals("Hello World!", hw.say());
	}
	
	@Ignore("not ready yet")
	@Test
	public void testSay2() 
	{
		assertEquals("Hello World!", hw.say());
	}
}
