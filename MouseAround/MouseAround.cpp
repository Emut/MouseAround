#include <iostream>
#include "CInputGrabberWin.h"
#include "CManager.h"

int main(int argc, char** argv)
{
    std::cout << "Hello World!\n";
	CManager test(argv[0], 1000, 500);
	Sleep(60 * 1000);
}


