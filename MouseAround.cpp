#include <iostream>
#include "CManager.h"
#include <thread>
#include <chrono>
#include <future>

int main(int argc, char** argv)
{
	std::cout << "************************\n";
    std::cout << "*    MouseAround by    *\n";
	std::cout << "*      Umut Ekici      *\n";
	std::cout << "* numutekici@gmail.com *\n";
	std::cout << "*    github.com/Emut   *\n";
	std::cout << "*      2019 - 2020     *\n";
	std::cout << "************************\n";
	
	CManager test(argv[0]);
	std::this_thread::sleep_for(std::chrono::seconds(30));		//for quick tests
	std::promise<void>().get_future().wait();
	//Sleep(INFINITE);
	//Sleep(30*1000);
}


