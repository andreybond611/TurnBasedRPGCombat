// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "CoreMinimal.h"
#include "EngineUtils.h"

class UWidget;

namespace Test
{

template <typename ActorType>
ActorType* SpawnBlueprintActor(UWorld* World, const FString& BlueprintReference, const FTransform& Transform = FTransform::Identity)
{
	const UBlueprint* Blueprint = LoadObject<UBlueprint>(nullptr, *BlueprintReference);
	return (World && Blueprint) ? World->SpawnActor<ActorType>(Blueprint->GeneratedClass, Transform) : nullptr;
}

UWorld* GetGameWorld();

template <typename ActorType>
ActorType* GetActorOfClass(TSubclassOf<AActor> ActorClass = ActorType::StaticClass())
{
	UWorld* World = GetGameWorld();
	check(World);
	for (TActorIterator<AActor> It(World, ActorClass); It; ++It)
	{
		AActor* Actor = *It;
		return Cast<ActorType>(Actor);
	}
	return nullptr;
}

class LevelScope
{
public:
	LevelScope(const FString& MapName);
	~LevelScope();
};

/**
 * Iterates over UEnums, first Function parameter is enum value, second is FName
 */
template <typename EnumType, typename FunctionType>
void EnumForEach(FunctionType&& Function)
{
	const UEnum* Enum = StaticEnum<EnumType>();
	for (int32 i = 0; i < Enum->NumEnums(); ++i)
	{
		Function(static_cast<EnumType>(Enum->GetValueByIndex(i)), Enum->GetNameByIndex(i));
	}
}

void CallBlueprintFunction(UObject* Caller, const FString& FunctionName, const TArray<FString>& Parameters);

UWidget* FindWidgetByName(const UUserWidget* Widget, const FName& WidgetName);

template <class WidgetType>
WidgetType* FindWidgetByClass()
{
	TArray<UUserWidget*> Widgets;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetGameWorld(), Widgets, WidgetType::StaticClass(), false);
	return Widgets.Num() != 0 ? Cast<WidgetType>(Widgets[0]) : nullptr;
}

} // namespace Test
