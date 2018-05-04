// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "MyProject.h"
#include "Modules/ModuleManager.h"

namespace
{
	TAutoConsoleVariable<int32> CVARDebugObjectsDrawing(
		TEXT("IMSHA.DebugObjects"),
		0,
		TEXT("Draw Debug Lines for Objects: 1-DamageSphere"),
		ECVF_Cheat
	);
}


IMPLEMENT_PRIMARY_GAME_MODULE( FDefaultGameModuleImpl, MyProject, "MyProject" );
 