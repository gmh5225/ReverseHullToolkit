// Copyright Epic Games, Inc. All Rights Reserved.

#include "Framework/Application/SlateApplication.h"
#include "ReverseHullToolkitStyle.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleRegistry.h"
#include "Slate/SlateGameResources.h"
#include "Styling/SlateStyleMacros.h"

#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FReverseHullToolkitStyle::StyleInstance = nullptr;

void FReverseHullToolkitStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FReverseHullToolkitStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FReverseHullToolkitStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("ReverseHullToolkitStyle"));
	return StyleSetName;
}

const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);

TSharedRef< FSlateStyleSet > FReverseHullToolkitStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("ReverseHullToolkitStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("ReverseHullToolkit")->GetBaseDir() / TEXT("Resources"));

	Style->Set("ReverseHullToolkit.OpenPluginWindow", new IMAGE_BRUSH_SVG(TEXT("PlaceholderButtonIcon"), Icon20x20));

	return Style;
}

void FReverseHullToolkitStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FReverseHullToolkitStyle::Get()
{
	return *StyleInstance;
}
