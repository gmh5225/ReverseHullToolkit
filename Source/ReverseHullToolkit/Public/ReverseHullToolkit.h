// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "ReverseHullGenerator.h"

class FToolBarBuilder;
class FMenuBuilder;

class FReverseHullToolkitModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command (by default it will bring up plugin window) */
	void PluginButtonClicked();
	
private:

	void RegisterMenus();

	TSharedRef<class SDockTab> OnSpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs);

	FReply OnGenerateButtonClicked();

private:
	TSharedPtr<class FUICommandList> PluginCommands;
	TSharedPtr<ReverseHullGenerator> HullGenerator;
	bool DoCompactMesh = false;
};
