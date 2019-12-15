#include <iostream>
#include "CInputGrabberWin.h"
#include "CManager.h"
#include "CConfigHandler.h"
#include "MouseAroundArduinoLeonardoScript/CArduinoCommandParser.cpp"

int main(int argc, char** argv)
{
    std::cout << "Hello World!\n";
	
//CArduinoCommandParser myParser(64);
//
//myParser.InsertInput(195);
//myParser.InsertInput(30);
//myParser.InsertInput(255);
//myParser.InsertInput(0);
//myParser.InsertInput(224);

	CConfigHandler::getInstance()->ImportConfig("C:/Users/Emut/source/repos/MouseAround/config.ini");
	CManager test(argv[0], 1000, 500);
	Sleep(INFINITE);
}


