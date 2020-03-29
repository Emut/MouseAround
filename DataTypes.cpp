#include "DataTypes.h"
#include <string.h>
#include <iostream>


using namespace std;
HotkeyInfo::HotkeyInfo(){
	ucpHotkeySequence = NULL;
	args = NULL;
}

HotkeyInfo::~HotkeyInfo(){	//overriden for now. Need CC and operator= to work properly.
	//if(ucpHotkeySequence != NULL)
	//	delete ucpHotkeySequence;
	//if(args != NULL)
	//	delete args;
}


BorderlessInfo::BorderlessInfo(){
	leftNeigh = -1;
	rightNeigh = -1;
	topNeigh = -1;
	bottomNeigh = -1;
}

void BorderlessInfo::Print() const{
	printf("    BORDERS: l:(%+05d,%+05d) r:(%+05d,%+05d) u:(%+05d,%+05d) d:(%+05d,%+05d)\n", leftBorder, leftNeigh, rightBorder, rightNeigh, topBorder, topNeigh, bottomBorder, bottomNeigh);
}

void HandlerProperties::Print() const{
	printf("DEV_ID:%-3d DEV_NAM:%-7s PORT:%-5d IP:%-15s WIDTH:%-4d HEIGTH:%-4d\n", DEVICE_ID, DEVICE_NAME, PORT, IP_ADDR, width, heigth);
	//std::cout<<"DEV_ID:"<<DEVICE_ID<<"\tDEV_NAM:"<<DEVICE_NAME<<"\tPORT:"<<PORT<<"IP_ADDR:"<<IP_ADDR<<"WIDTH:"<<width<<"HEIGTH:"<<heigth<<std::endl;
	itsBorders.Print();	
}

void HotkeyInfo::setTypeAndArgs(HotkeyType typeToSet, const char* argsToSet){
	type = typeToSet;
	switch(type){
	case SET_ACTIVE_SCREEN:
		args = new int[1];
		*((int*)args) = (int)strtol(argsToSet, NULL, 0);
		break;
	case TOGGLE_BORDERLESS_MODE:
		args = NULL;
		break;
	case EXECUTE_CMD:
		args = new char[strlen(argsToSet) + 1];
		strcpy((char*)args, argsToSet);
		break;
	default:
		break;
	}
}

void HotkeyInfo::Print() const{
	cout<<"KEYS ";
	for(int i = 0; i < ucHotkeyLength; ++i)
		cout<<(int)ucpHotkeySequence[i]<<' ';
	switch(type){
	case SET_ACTIVE_SCREEN:
		cout<<"SET_ACTIVE_SCREEN "<<*((int*)args)<<endl; 
		break;
	case TOGGLE_BORDERLESS_MODE:
		cout<<"TOGGLE_BORDERLESS_MODE"<<endl; 
		break;
	case EXECUTE_CMD:
		cout<<"EXECUTE_CMD "<<(char*)args<<endl; 
		break;
	default:
		break;
	}
}