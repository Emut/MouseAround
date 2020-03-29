#pragma once

#ifndef VK_LEFTMOUSECLICK
#define VK_LEFTMOUSECLICK 1
#endif // !VK_LEFTMOUSECLICK

#ifndef VK_RIGHTMOUSECLICK
#define VK_RIGHTMOUSECLICK 2
#endif // !VK_RIGHTMOUSECLICK

#ifndef VK_MIDDLEMOUSECLICK
#define VK_MIDDLEMOUSECLICK 4
#endif // !VK_MIDDLEMOUSECLICK

#ifndef VK_RETURN
#define VK_RETURN 0x0D
#endif // !VK_RETURN

#ifndef VK_BACK
#define VK_BACK 0x08
#endif // !VK_BACK

#ifndef VK_ESCAPE
#define VK_ESCAPE 0x1B
#endif // !VK_ESCAPE

#ifndef VK_SPACE
#define VK_SPACE 0x20
#endif // !VK_SPACE

#ifndef VK_TAB
#define VK_TAB 0x09
#endif // !VK_TAB

#ifndef VK_HOME
#define VK_HOME 0x24
#endif // !VK_HOME

#ifndef VK_END
#define VK_END 0x23
#endif // !VK_END

#ifndef VK_INSERT
#define VK_INSERT 0x2D
#endif // !VK_INSERT

#ifndef VK_DELETE
#define VK_DELETE 0x2E
#endif // !VK_DELETE

#ifndef VK_PRIOR
#define VK_PRIOR 0x21
#endif // !VK_PRIOR

#ifndef VK_NEXT
#define VK_NEXT 0x22
#endif // !VK_NEXT

#ifndef VK_CAPITAL
#define VK_CAPITAL 0x14
#endif // !VK_CAPITAL

#ifndef VK_LCONTROL
#define VK_LCONTROL 0xA2
#endif // !VK_LCONTROL

#ifndef VK_CONTROL
#define VK_CONTROL 0x11
#endif // !VK_CONTROL

#ifndef VK_RCONTROL
#define VK_RCONTROL 0xA3
#endif // !VK_RCONTROL

#ifndef VK_LSHIFT
#define VK_LSHIFT 0xA0
#endif // !VK_LSHIFT

#ifndef VK_SHIFT
#define VK_SHIFT 0x10
#endif // !VK_SHIFT

#ifndef VK_RSHIFT
#define VK_RSHIFT 0xA1
#endif // !VK_RSHIFT

#ifndef VK_LMENU
#define VK_LMENU 0xA4
#endif // !VK_LMENU

#ifndef VK_MENU
#define VK_MENU 0x12
#endif // !VK_MENU

#ifndef VK_RMENU
#define VK_RMENU 0xA5
#endif // !VK_RMENU

#ifndef VK_LWIN
#define VK_LWIN 0x5B
#endif // !VK_LWIN

#ifndef VK_RWIN
#define VK_RWIN 0x5C
#endif // !VK_RWIN

#ifndef VK_LEFT
#define VK_LEFT 0x25
#endif // !VK_LEFT

#ifndef VK_RIGHT
#define VK_RIGHT 0x27
#endif // !VK_RIGHT

#ifndef VK_UP
#define VK_UP 0x26
#endif // !VK_UP

#ifndef VK_DOWN
#define VK_DOWN 0x28
#endif // !VK_DOWN

#ifndef VK_OEM_COMMA
#define VK_OEM_COMMA 0xBC
#endif // !VK_OEM_COMMA

#ifndef VK_OEM_PERIOD
#define VK_OEM_PERIOD 0xBE
#endif // !VK_OEM_PERIOD

#ifndef VK_OEM_MINUS
#define VK_OEM_MINUS 0xBD
#endif // !VK_OEM_MINUS

#ifndef VK_OEM_PLUS
#define VK_OEM_PLUS 0xBB
#endif // !VK_OEM_PLUS

#ifndef VK_OEM_1
#define VK_OEM_1 0xBA
#endif // !VK_OEM_1

#ifndef VK_OEM_2
#define VK_OEM_2 0xBF
#endif // !VK_OEM_2

#ifndef VK_OEM_3
#define VK_OEM_3 0xC0
#endif // !VK_OEM_3

#ifndef VK_OEM_4
#define VK_OEM_4 0xDB
#endif // !VK_OEM_4

#ifndef VK_OEM_5
#define VK_OEM_5 0xDC
#endif // !VK_OEM_5

#ifndef VK_OEM_6
#define VK_OEM_6 0xDD
#endif // !VK_OEM_6

#ifndef VK_OEM_7
#define VK_OEM_7 0xDE
#endif // !VK_OEM_7

#ifndef VK_OEM_8
#define VK_OEM_8 0xDF
#endif // !VK_OEM_8

#ifndef VK_OEM_102
#define VK_OEM_102 0xE2
#endif // !VK_OEM_102

#ifndef VK_F1
#define VK_F1 0x70
#endif // !VK_F1

#ifndef VK_F24
#define VK_F24 0x87
#endif // !VK_F24


namespace KeyboardTypeHelper {

	bool WinToArduino(unsigned char& par_ucKey) {
		//printf("%d\n", par_ucKey);
		switch (par_ucKey)
		{
		case VK_LEFTMOUSECLICK:
			return true;
			break;
		case VK_RIGHTMOUSECLICK:
			return true;
			break;
		case VK_MIDDLEMOUSECLICK:
			return true;
			break;
		case VK_RETURN:
			par_ucKey = 0xB0;
			return true;
			break;
		case VK_BACK:
			par_ucKey = 0xB2;
			return true;
			break;
		case VK_ESCAPE:
			par_ucKey = 0xB1;
			return true;
			break;
		case VK_SPACE:
			par_ucKey = ' ';
			return true;
			break;
		case VK_TAB:
			par_ucKey = 0xB3;
			return true;
			break;
		case VK_HOME:
			par_ucKey = 0xD2;
			return true;
			break;
		case VK_END:
			par_ucKey = 0xD5;
			return true;
			break;
		case VK_INSERT:
			par_ucKey = 0xD1;
			return true;
			break;
		case VK_DELETE:
			par_ucKey = 0xD4;
			return true;
			break;
		case VK_PRIOR:
			par_ucKey = 0xD3;
			return true;
			break;
		case VK_NEXT:
			par_ucKey = 0xD6;
			return true;
			break;
		case VK_CAPITAL:
			par_ucKey = 0xC1;
			return true;
			break;
		case VK_LCONTROL:
		case VK_CONTROL:
			par_ucKey = 0x80;
			return true;
			break;
		case VK_RCONTROL:
			par_ucKey = 0x84;
			return true;
			break;
		case VK_LSHIFT:
		case VK_SHIFT:
			par_ucKey = 0x81;
			return true;
			break;
		case VK_RSHIFT:
			par_ucKey = 0x85;
			return true;
			break;
		case VK_LMENU:
		case VK_MENU:
			par_ucKey = 0x82;
			return true;
			break;
		case VK_RMENU:
			par_ucKey = 0x86;
			return true;
			break;
		case VK_LWIN:
			par_ucKey = 0x83;
			return true;
			break;
		case VK_RWIN:
			par_ucKey = 0x87;
			return true;
			break;
		case VK_LEFT:
			par_ucKey = 0xD8;
			return true;
			break;
		case VK_RIGHT:
			par_ucKey = 0xD7;
			return true;
			break;
		case VK_UP:
			par_ucKey = 0xDA;
			return true;
			break;
		case VK_DOWN:
			par_ucKey = 0xD9;
			return true;
			break;
		case VK_OEM_COMMA:
			par_ucKey = ',';
			return true;
			break;
		case VK_OEM_PERIOD:
			par_ucKey = '.';
			return true;
			break;
		case VK_OEM_MINUS:
			par_ucKey = '-';
			return true;
			break;
		case VK_OEM_PLUS:
			par_ucKey = '=';
			return true;
			break;
		case VK_OEM_1:
			par_ucKey = ';';
			return true;
			break;
		case VK_OEM_2:
			par_ucKey = '/';
			return true;
			break;
		case VK_OEM_3:
			par_ucKey = '~';
			return true;
			break;
		case VK_OEM_4:
			par_ucKey = '[';
			return true;
			break;
		case VK_OEM_5:	
			par_ucKey = '\\';
			return true;
			break;
		case VK_OEM_6:
			par_ucKey = ']';
			return true;
			break;
		case VK_OEM_7:
			par_ucKey = '\'';
			return true;
			break;
		case VK_OEM_8:
			par_ucKey = '-';
			return true;
			break;
		case VK_OEM_102:
			par_ucKey = '<';
			return true;
			break;
		default:
			if (par_ucKey >= '0' && par_ucKey <= '9') {
				return true;
			}
			if (par_ucKey >= 'A' && par_ucKey <= 'Z') {
				par_ucKey += 32;	//ardu uses lower cases>
				return true;
			}
			if (par_ucKey >= VK_F1 && par_ucKey <= VK_F24) {
				par_ucKey += 0xC2 - VK_F1;	//ardu uses C2 as F1
				return true;
			}
			printf("unknwn key%d\n", par_ucKey);
			return false;
			break;
		}
		
	}
}