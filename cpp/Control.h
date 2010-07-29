#ifndef __CONTROL_H__
#define __CONTROL_H__
 
struct Controller
{
	int joyNum; // set to > -1 to get the number of the joystick used to control this item 
	int keyBoard; // set to 1 to let the keyboard control this player
	int mouse; // set to 1 to let the mouse control this player
	Controller( int joyNum = -1, int keyBoard = 0, int mouse = 0) { 
		this->joyNum = joyNum;
		this->keyBoard = keyBoard;
		this->mouse = mouse;
	}
};

#endif
