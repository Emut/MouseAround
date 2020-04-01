# MouseAround

## C++11 based virtual keyboard-mouse extender and KVM switch.

**Features**
- Works on Windows and Linux(X11 dependent). 
- Send Mouse and Keyboard actions to other devices running the MouseAround via network connection.
- Send Mouse and Keyboard actions to unmanagable or not network attached computers via an Arduino Leonardo's USB connection. 
- Borderless Mode allows continuous mouse flow between all devices.
- Define hotkeys for changing active device, toggling borderless mode or executing custom system commands.
- Can change active input of monitors via system commands, thus can be used as a virtual KVM switch.

**Sample Config (Explanation Below)**\
\#CONFIG BEGINS\
DEVICE_COUNT=3;\
DEVICE_ID=0 DEVICE_NAME=SELF 	PORT=12357 	IP_ADDR=XXX.XXX.XXX.XXX WIDTH=-1 	HEIGTH=-1;\
DEVICE_ID=1 DEVICE_NAME=ARDUINO PORT=11 	IP_ADDR=XXX.XXX.XXX.XXX WIDTH=1080 	HEIGTH=1920;\
DEVICE_ID=2 DEVICE_NAME=NETWORK PORT=12357 	IP_ADDR=192.168.30.132 	WIDTH=1920 	HEIGTH=1080;\
\
HOTKEY_COUNT=6;\
HKEY_ID=0	KEYS=49,162,164	HKEY_TYPE=1	ARG=0;\
HKEY_ID=1	KEYS=50,162,164	HKEY_TYPE=1	ARG=1;\
HKEY_ID=2	KEYS=51,162,164	HKEY_TYPE=1	ARG=2;\
HKEY_ID=3	KEYS=52,162,164	HKEY_TYPE=2	ARG=-1;\
HKEY_ID=4	KEYS=53,162,164	HKEY_TYPE=3	ARG="C:\Program Files (x86)\Dell\Dell Display Manager\ddm.exe" /1:SetActiveInput DP;\
HKEY_ID=5	KEYS=54,162,164	HKEY_TYPE=3	ARG="C:\Program Files (x86)\Dell\Dell Display Manager\ddm.exe" /1:SetActiveInput VGA;\
\
BORDERLESS_SETTINGS_COUNT=3;\
BLESS_SET_ID=0	DEVICE_ID=0		LEFT=0,1	RIGHT=2559,2	UP=-1,-1	DOWN=-1,-1;\
BLESS_SET_ID=1	DEVICE_ID=1		LEFT=-1,-1	RIGHT=1079,0	UP=-1,-1	DOWN=-1,-1;\
BLESS_SET_ID=2	DEVICE_ID=2		LEFT=0,0	RIGHT=-1,-1		UP=-1,-1	DOWN=-1,-1;\
BLESS_ENABLE_CMD="C:\Program Files (x86)\Dell\Dell Display Manager\ddm.exe" /1:SetActiveInput DP;\
BLESS_DISABLE_CMD="C:\Program Files (x86)\Dell\Dell Display Manager\ddm.exe" /1:SetActiveInput VGA;\
\#CONFIG ENDS

Above config defines 3 devices. Host computer itself, another computer connected via network and running the
MouseAround and a third computer connected via an Arduino Leonardo. For SELF, config specifies the port host listens for 
incoming commands. For network connected devices, the IP address and port the MouseAround on target device listens must be
specified along with target's size. For Arduino connected devices the COM port used to access the Arduino and size of the 
target device must be specified.

Hotkey keys are defined as given in Windows' virtual keys: 
https://docs.microsoft.com/tr-tr/windows/win32/inputdev/virtual-key-codes
. Hotkey type 1 sets the active device to given arg. Hotkey type 2 toggles borderless mode. Hotkey type 3 executes the system
command given as arg.

For borderless mode, each device's borders and if exists, neighboring device on that border must be specified. Additionally,
2 system commands can be given to be executed during enabling and disabling the borderless mode.

Overall, with the above config, user can switch between 3 devices. 2 system commands can be invoked to change the active
input of a monitor. In borderless mode, 3 devices are defined side by side. Enabling borderless mode also changes monitor's
input to display the Arduino controlled device's screen.
