// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#if WITH_AUTOMATION_TESTS

#include "TestUtils.h"
#include "Tests/AutomationCommon.h"
#include "Misc/OutputDeviceNull.h"
#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "Blueprint/WidgetTree.h"
#include "Misc/AutomationTest.h"

namespace Test
{

UWorld* Test::GetGameWorld()
{
	const TIndirectArray<FWorldContext>& WorldContexts = GEngine->GetWorldContexts();
	for (const FWorldContext& Context : WorldContexts)
	{
		if ((Context.WorldType == EWorldType::PIE || Context.WorldType == EWorldType::Game) && Context.World() != nullptr)
		{
			return Context.World();
		}
	}
	return nullptr;
}

Test::LevelScope::LevelScope(const FString& MapName)
{
	AutomationOpenMap(MapName);
}

Test::LevelScope::~LevelScope()
{
	ADD_LATENT_AUTOMATION_COMMAND(FExitGameCommand);
}

void Test::CallBlueprintFunction(UObject* Caller, const FString& FunctionName, const TArray<FString>& Parameters)
{
	if (!Caller)
	{
		return;
	}

	// Command pattern: "FunctionName Param1 Param2 Param3"
	FString Command = FunctionName;
	for (const auto& Param : Parameters)
	{
		Command.Append(" ").Append(Param);
	}
	FOutputDeviceNull OutputDeviceNull;
	Caller->CallFunctionByNameWithArguments(*Command, OutputDeviceNull, nullptr, true);
}

UWidget* Test::FindWidgetByName(const UUserWidget* Widget, const FName& WidgetName)
{
	if (!Widget || !Widget->WidgetTree)
		return nullptr;

	UWidget* FoundWidget = nullptr;
	UWidgetTree::ForWidgetAndChildren(Widget->WidgetTree->RootWidget,
									  [&](UWidget* Child)
									  {
										  if (Child && Child->GetFName().IsEqual(WidgetName))
										  {
											  FoundWidget = Child;
										  }
									  });
	return FoundWidget;
}

} // namespace Test
#endif
