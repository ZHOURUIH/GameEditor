#include "WindowComponentRotateSpeed.h"
#include "WindowHeader.h"

void WindowComponentRotateSpeed::applyRotation(const MATRIX3& rotation)
{
	Window* window = static_cast<Window*>(mComponentOwner);
	window->setMatrixRotation(rotation);
}