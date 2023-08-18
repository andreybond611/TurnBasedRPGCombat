// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"

struct FPriorityText
{
	FPriorityText() = default;

	FPriorityText(FText InText, const int32 InPriority, const FColor InColor = FColor::White)
		: Text{InText}
		, Priority{InPriority}
		, Color{InColor}
	{
	}

	FText Text;
	int32 Priority;
	FColor Color;
};

struct PriorityTextPredicate
{
	bool operator()(const FPriorityText& A, const FPriorityText& B) const
	{
		// Inverted compared to std::priority_queue - higher priorities float to the top
		return A.Priority > B.Priority;
	}
};