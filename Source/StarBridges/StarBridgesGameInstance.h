// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Puzzle/PuzzleDataStruct.h"

#include "StarBridgesGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class STARBRIDGES_API UStarBridgesGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puzzle Info")
	FPuzzleListStruct PuzzleListStruct;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puzzle Info")
	int32 PuzzleLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puzzle Info")
	TArray<FString> PuzzleIslandNames;

	
};
