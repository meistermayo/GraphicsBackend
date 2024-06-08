#include "GraphicObject_Base.h"
#include "../Model/Model.h"
#include <assert.h>


GraphicObject_Base::GraphicObject_Base()
{
	pModel = nullptr;
}


void GraphicObject_Base::SetModel(Model* mod)
{
	pModel = mod;
}


