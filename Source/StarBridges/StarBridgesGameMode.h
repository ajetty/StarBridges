// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Puzzle/JsonReader.h"
#include "Puzzle/Puzzle.h"

#include "StarBridgesGameMode.generated.h"

class AStarBridgesPlayerController;
UCLASS()
class AStarBridgesGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	
	AStarBridgesGameMode();
	void HandleGameEnd();

	UFUNCTION(BlueprintCallable)
	void HandleGameStart();

protected:

	void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puzzle")
	APuzzle* Puzzle;

	AStarBridgesPlayerController* PlayerController;

	

private:



	
	
	
};



