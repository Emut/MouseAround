#pragma once
namespace KeyboardTypeHelper {
	bool WinToArduino(unsigned char& par_ucKey) {
		//printf("%d\n", par_ucKey);
		switch (par_ucKey)
		{
		case 1:
			return true;
			break;
		case 2:
			return true;
			break;
		case 4:
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