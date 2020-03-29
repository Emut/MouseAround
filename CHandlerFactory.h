#ifndef CHANDLERFACTORY_H
#define CHANDLERFACTORY_H

#include "DataTypes.h"
#include "IScreenHandler.h"

class CHandlerFactory{
public:
	static IScreenHandlerIn* CreateHandler(HandlerProperties);
};

#endif