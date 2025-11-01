#pragma once

#include <IrrIMGUI/IrrIMGUI.h>

using namespace irr;
using namespace IrrIMGUI;

#ifndef INITIALIZER_H
#define INITIALIZER_H

struct Initializer
{
	static SIrrlichtCreationParameters irrlichtDriverParams(CIMGUIEventReceiver& EventReceiver)
	{
		SIrrlichtCreationParameters IrrlichtParams;
		IrrlichtParams.DriverType = video::EDT_OPENGL;
		IrrlichtParams.WindowSize = core::dimension2d<u32>(1080, (1080 / 16) * 9);
		IrrlichtParams.Bits = 32;
		IrrlichtParams.Fullscreen = false;
		IrrlichtParams.Stencilbuffer = true;
		IrrlichtParams.AntiAlias = 16;
		IrrlichtParams.Vsync = false;
		IrrlichtParams.EventReceiver = &EventReceiver;
		return IrrlichtParams;
	}
};

#endif // !INITIALIZER_H
