// Copyright Epic Games, Inc. All Rights Reserved.

#include "ReverseHullToolkitCommands.h"

#define LOCTEXT_NAMESPACE "FReverseHullToolkitModule"

void FReverseHullToolkitCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "ReverseHullToolkit", "Bring up ReverseHullToolkit window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
