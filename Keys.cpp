#include "Keys.h"
#include "GraphicsBackend.h"

bool Keyboard::GetKeyState(KEY_CODE key)
{
	return GraphicsBackend::GetKeyState(key);
}