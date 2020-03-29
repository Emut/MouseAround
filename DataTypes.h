#ifndef DATATYPES_H
#define DATATYPES_H

struct BorderlessInfo{
	int leftNeigh;
	int rightNeigh;
	int topNeigh;
	int bottomNeigh;

	int leftBorder;
	int rightBorder;
	int topBorder;
	int bottomBorder;

	BorderlessInfo();
	void Print() const;
};

struct HandlerProperties{
	int DEVICE_ID;
	char DEVICE_NAME[8];
	char IP_ADDR[16];	//XXX.XXX.XXX.XXX\0
	int PORT;
	int width;
	int heigth;
	BorderlessInfo itsBorders;

	void Print() const;
};

enum HotkeyType{
	SET_ACTIVE_SCREEN,
	TOGGLE_BORDERLESS_MODE,
	EXECUTE_CMD,
	DUMMY
};

struct HotkeyInfo
{
	unsigned char* ucpHotkeySequence;
	unsigned char ucHotkeyLength;
	int nHotkeyID;
	HotkeyType type;
	void* args;

	void setTypeAndArgs(HotkeyType type, const char* args);
	HotkeyInfo();
	~HotkeyInfo();

	void Print() const;
};



#endif