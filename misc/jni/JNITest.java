class JNITest
{
	private native void print();
	public static void main(String []args)
	{
		
	}
	static {
		System.load("/Users/waqqassharif/Documents/source code/jni/jnilib.so");
		//System.loadLibrary("jnilib");
	}
}
