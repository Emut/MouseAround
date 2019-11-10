#include <iostream>
#include "CInputGrabberWin.h"
#include "CManager.h"
#include "CConfigHandler.h"

int main(int argc, char** argv)
{
    std::cout << "Hello World!\n";
	
	CConfigHandler::getInstance()->ImportConfig("C:/Users/Emut/source/repos/MouseAround/config.ini");
	CManager test(argv[0], 1000, 500);
	Sleep(INFINITE);
}


