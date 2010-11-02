#ifndef CONTROL_CLASS_H
#define CONTROL_CLASS_H

#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code)&0x8000)?1:0)
#define KEY_UP(vk_code) ((GetAsyncKeyState(vk_code)&0x8000)?0:1)

enum keyCodes
{
	KEY_A,KEY_B,KEY_C,KEY_D,KEY_E,KEY_F,KEY_G,KEY_H,KEY_I,KEY_J,KEY_K,KEY_L,KEY_M,
	KEY_N,KEY_O,KEY_P,KEY_Q,KEY_R,KEY_S,KEY_T,KEY_U,KEY_V,KEY_W,KEY_X,KEY_Y,KEY_Z,
	KEY_SPACEBAR,KEY_ESCAPE,KEY_ENTER
);

class EXPORT Control
{
public:
	void up() = 0;
	void down() = 0;
	void left() = 0;
	void right() = 0;
	void button1() = 0;
	void button2() = 0;
	void startButton() = 0;
};
#endif
