// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridLineDrawInfo.generated.h"

/**
 * 
 */
USTRUCT()
struct FGridLineDrawInfo
{
	GENERATED_BODY()

	FString Name;

	FVector StartLocationBridgeVector1;
	FVector StartLocationBridgeVector2A;
	FVector StartLocationBridgeVector2B;

	FVector EndLocationBridgeVector1;
	FVector EndLocationBridgeVector2A;
	FVector EndLocationBridgeVector2B;

	bool bIsANorthSouthBridge;

	float RadianCircumferenceOffset;

	float TriggerRadius;
	
};
