#include "CHandlerFactory.h"
#include "CArduinoHandler.h"
#include "CNetworkHostHandler.h"
#include "CSelfHandler.h"
#include <string.h>

IScreenHandlerIn* CHandlerFactory::CreateHandler(HandlerProperties config){
	if(strcmp("SELF", config.DEVICE_NAME) == 0)
		return new CSelfHandler(config.PORT);
	if(strcmp("ARDUINO", config.DEVICE_NAME) == 0)
		return new CArduinoHandler(config.PORT, config.width, config.heigth);
	if(strcmp("NETWORK", config.DEVICE_NAME) == 0)
		return new CNetworkHostHandler(config.width, config.heigth, config.IP_ADDR, config.PORT);
	printf("Unexpected config at CHandlerFactory::CreateHandler");
	return NULL;
}