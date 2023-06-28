// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "ReverseHullToolkitStyle.h"

class FReverseHullToolkitCommands : public TCommands<FReverseHullToolkitCommands>
{
public:

	FReverseHullToolkitCommands()
		: TCommands<FReverseHullToolkitCommands>(TEXT("ReverseHullToolkit"), NSLOCTEXT("Contexts", "ReverseHullToolkit", "ReverseHullToolkit Plugin"), NAME_None, FReverseHullToolkitStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};