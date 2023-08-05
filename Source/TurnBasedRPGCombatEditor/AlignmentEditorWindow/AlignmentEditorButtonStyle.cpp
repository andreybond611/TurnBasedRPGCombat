// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#include "AlignmentEditorButtonStyle.h"
#include "Framework/Application/SlateApplication.h"
#include "Styling/SlateStyleRegistry.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"

#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FAlignmentEditorButtonStyle::StyleInstance = nullptr;

void FAlignmentEditorButtonStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FAlignmentEditorButtonStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FAlignmentEditorButtonStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("AlignmentEditorButtonStyle"));
	return StyleSetName;
}


const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);

TSharedRef<FSlateStyleSet> FAlignmentEditorButtonStyle::Create()
{
	TSharedRef<FSlateStyleSet> Style = MakeShareable(new FSlateStyleSet("AlignmentEditorButtonStyle"));
	
	Style->SetContentRoot(FPaths::ProjectContentDir() / TEXT("Editor/Slate"));

	Style->Set("AlignmentEditor.AlignmentEditorButton", new IMAGE_BRUSH(TEXT("YinYangSymbol"), Icon20x20));
	return Style;
}

void FAlignmentEditorButtonStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FAlignmentEditorButtonStyle::Get()
{
	return *StyleInstance;
}
