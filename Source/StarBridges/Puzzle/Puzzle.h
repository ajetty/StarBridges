// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Grid.h"
#include "Chaos/ISpatialAcceleration.h"
#include <bitset> 
#include "PuzzleDataStruct.h"
#include "Puzzle.generated.h"

class AStarBridgesGameMode;
class AGridLineSystem;
class AActorFloor;
class APawnIsland;
UCLASS()
class STARBRIDGES_API APuzzle : public AActor
{
	GENERATED_BODY()

public:
	APuzzle();

	//Number of blocks along each side of grid
	UPROPERTY(Category=Grid, EditAnywhere, BlueprintReadOnly)
	int32 GridSize;

	UPROPERTY(Category=Grid, EditAnywhere, BlueprintReadOnly)
	FLinearColor IslandLineConnectionColor;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<APawnIsland> PawnIslandBP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	AGrid* Grid;

	void GenerateLinkBetweenTwoIslands(APawnIsland* MouseClickedIsland);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Grid)
	AGridLineSystem* GridLineSystem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Grid)
	int32 PuzzleLevelSelection;

	UFUNCTION(BlueprintCallable)
	void SetProperties(FPuzzleListStruct PuzzleList, int32 Level);

	void ClearBitflagLines(FString IslandNames);

	void LoadPuzzle();

	int32 GetIslandCount();

	UPROPERTY(VisibleAnywhere, Category = Grid)
	FPuzzleListStruct PuzzleListStruct;

	void EndGame();
	
protected:
	virtual void BeginPlay() override;

	
private:
	void GenerateIslandsInWorld();

	UPROPERTY(VisibleAnywhere, Category = Grid)
	TMap<FString, APawnIsland*> IslandSpawnMap;
	
	void CreateGridLineSystem();

	void CheckForCompletePuzzle();
	
	AStarBridgesGameMode* GameMode;



};
