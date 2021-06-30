// Fill out your copyright notice in the Description page of Project Settings.


#include "Puzzle.h"

#include <string>

#include "DrawDebugHelpers.h"
#include "GridLineSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "StarBridges/Island/PawnIsland.h"
#include "JsonUtilities.h"
#include "JsonObjectConverter.h"
#include "Chaos/ChaosDebugDraw.h"
#include "StarBridges/StarBridgesGameMode.h"

#define LOCTEXT_NAMESPACE "Puzzle"

#define NORTH_BRIDGE_1 0
#define NORTH_BRIDGE_2 1
#define EAST_BRIDGE_1 2
#define EAST_BRIDGE_2 3
#define SOUTH_BRIDGE_1 4
#define SOUTH_BRIDGE_2 5
#define WEST_BRIDGE_1 6
#define WEST_BRIDGE_2 7
#define INPUT_BRIDGE_TOTAL_REACHED 8
#define CORRECT_INPUT_COMBINATIONS 9
#define NORTH_DIRECTION_GOOD 10
#define EAST_DIRECTION_GOOD 11
#define SOUTH_DIRECTION_GOOD 12
#define WEST_DIRECTION_GOOD 13
#define ACTOR_OVERLAPPING 14
#define MOUSE_CLICKED_CHOSEN 15

#define TOFLAG(Enum) (1ULL << static_cast<uint64>(Enum))
#define SET_BIT(Bitmask, Bit) (Bitmask |= 1ULL << static_cast<uint64>(Bit))
#define CLEAR_BIT(Bitmask, Bit) (Bitmask &= ~(1ULL << static_cast<uint64>(Bit)))
#define TEST_BIT(Bitmask, Bit) ((Bitmask & (1ULL << static_cast<uint64>(Bit))) > 0)
#define GET_STD_BITSET(Bitmask) ({ return std::bitset<sizeof(uint64)> Bitset(Bitmask)});


APuzzle::APuzzle()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	// Create dummy root scene component
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));

	static ConstructorHelpers::FClassFinder<APawnIsland> IslandBPClass(TEXT("/Game/Blueprints/BP_Island"));
	if (IslandBPClass.Class != nullptr)
	{
		PawnIslandBP = IslandBPClass.Class;
	}
}

void APuzzle::SetProperties(FPuzzleListStruct PuzzleList, int32 Level)
{
	PuzzleListStruct = PuzzleList;
	PuzzleLevelSelection = Level;
}

void APuzzle::ClearBitflagLines(FString IslandNames)
{
	FString IslandStartName = IslandNames.Left(1);
	FString IslandEndName = IslandNames.Left(2).Right(1);

	
	APawnIsland* StartIsland = IslandSpawnMap[IslandStartName];
	APawnIsland* EndIsland = IslandSpawnMap[IslandEndName];

	if(TEST_BIT(StartIsland->PlayerIslandStateEnum, NORTH_DIRECTION_GOOD) && TEST_BIT(EndIsland->PlayerIslandStateEnum, SOUTH_DIRECTION_GOOD))
	{
		CLEAR_BIT(StartIsland->PlayerIslandStateEnum, NORTH_BRIDGE_1);
		CLEAR_BIT(StartIsland->PlayerIslandStateEnum, NORTH_BRIDGE_2);

		CLEAR_BIT(EndIsland->PlayerIslandStateEnum, SOUTH_BRIDGE_1);
		CLEAR_BIT(EndIsland->PlayerIslandStateEnum, SOUTH_BRIDGE_2);
	}
	else if(TEST_BIT(StartIsland->PlayerIslandStateEnum, EAST_DIRECTION_GOOD) && TEST_BIT(EndIsland->PlayerIslandStateEnum, WEST_DIRECTION_GOOD))
	{
		CLEAR_BIT(StartIsland->PlayerIslandStateEnum, EAST_BRIDGE_1);
		CLEAR_BIT(StartIsland->PlayerIslandStateEnum, EAST_BRIDGE_2);

		CLEAR_BIT(EndIsland->PlayerIslandStateEnum, WEST_BRIDGE_1);
		CLEAR_BIT(EndIsland->PlayerIslandStateEnum, WEST_BRIDGE_2);
	}

	StartIsland->OnStatusChange();
	EndIsland->OnStatusChange();
}

void APuzzle::EndGame()
{
	for(auto& Island : IslandSpawnMap)
	{
		Island.Value->Destroy();
	}

	IslandSpawnMap.Empty();
}

void APuzzle::BeginPlay()
{
	Super::BeginPlay();
	
}

void APuzzle::LoadPuzzle()
{
	GameMode = Cast<AStarBridgesGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	Grid = Cast<AGrid>(UGameplayStatics::GetActorOfClass(GetWorld(), AGrid::StaticClass()));
	
	Grid->RowsNum = PuzzleListStruct.puzzle[PuzzleLevelSelection].grid;
	Grid->ColsNum = PuzzleListStruct.puzzle[PuzzleLevelSelection].grid;
	
	Grid->GenerateProceduralMesh();
	
	GenerateIslandsInWorld();
	
	GridLineSystem = Cast<AGridLineSystem>(UGameplayStatics::GetActorOfClass(GetWorld(), AGridLineSystem::StaticClass()));
	
	CreateGridLineSystem();
	
	GridLineSystem->GenerateLinesForIslands();
}

int32 APuzzle::GetIslandCount()
{
	return IslandSpawnMap.Num();
}

void APuzzle::GenerateIslandsInWorld()
{
	int32 PuzzleNumber = PuzzleLevelSelection;
	int32 NumberOfIslands = PuzzleListStruct.puzzle[PuzzleNumber].coordinates.Num();
	
	TArray<APawnIsland*> AllSpawnedIslands;

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	for (int i = 0; i < NumberOfIslands; i++)
	{
		FVector IslandGridLocation = Grid->GetGridPosition(PuzzleListStruct.puzzle[PuzzleNumber].coordinates[i].node[0], PuzzleListStruct.puzzle[PuzzleNumber].coordinates[i].node[1]);
	
		APawnIsland* SpawnedIsland = GetWorld()->SpawnActor<APawnIsland>(PawnIslandBP, IslandGridLocation, FRotator(0.0f, 0.0f, 0.0f), SpawnParameters);
	
		AllSpawnedIslands.Add(SpawnedIsland);
	}
	
	for (int32 i = 0; i < NumberOfIslands; i++)
	{
		AllSpawnedIslands[i]->Name = PuzzleListStruct.puzzle[PuzzleNumber].name[i];
		AllSpawnedIslands[i]->InputNumber = PuzzleListStruct.puzzle[PuzzleNumber].input[i];
	
		AllSpawnedIslands[i]->SetInputNumberLabel();
	
		AllSpawnedIslands[i]->ConnectedIslands.Append(PuzzleListStruct.puzzle[PuzzleNumber].connections[i].names);
		
		for (int32 BitFlagValue : PuzzleListStruct.puzzle[PuzzleNumber].state[i].value)
		{
			SET_BIT(AllSpawnedIslands[i]->CompletedIslandStateEnum, static_cast<uint64>(BitFlagValue));
		}
	
		if (TEST_BIT(AllSpawnedIslands[i]->CompletedIslandStateEnum, NORTH_DIRECTION_GOOD))
		{
			SET_BIT(AllSpawnedIslands[i]->PlayerIslandStateEnum, NORTH_DIRECTION_GOOD);
		}
	
		if (TEST_BIT(AllSpawnedIslands[i]->CompletedIslandStateEnum, EAST_DIRECTION_GOOD))
		{
			SET_BIT(AllSpawnedIslands[i]->PlayerIslandStateEnum, EAST_DIRECTION_GOOD);
		}
	
		if (TEST_BIT(AllSpawnedIslands[i]->CompletedIslandStateEnum, SOUTH_DIRECTION_GOOD))
		{
			SET_BIT(AllSpawnedIslands[i]->PlayerIslandStateEnum, SOUTH_DIRECTION_GOOD);
		}
	
		if (TEST_BIT(AllSpawnedIslands[i]->CompletedIslandStateEnum, WEST_DIRECTION_GOOD))
		{
			SET_BIT(AllSpawnedIslands[i]->PlayerIslandStateEnum, WEST_DIRECTION_GOOD);
		}
	
		IslandSpawnMap.Add(PuzzleListStruct.puzzle[PuzzleNumber].name[i], AllSpawnedIslands[i]);
	}
}

void APuzzle::CreateGridLineSystem()
{
	for (auto It = IslandSpawnMap.CreateConstIterator(); It; ++It)
	{
		bool bIsANorthSouthBridge;

		for (FString ConnectedIsland : It.Value()->ConnectedIslands)
		{
			APawnIsland* Connection = IslandSpawnMap[ConnectedIsland];

			if (TEST_BIT(Connection->PlayerIslandStateEnum, SOUTH_DIRECTION_GOOD) && TEST_BIT(It.Value()->PlayerIslandStateEnum, NORTH_DIRECTION_GOOD))
			{
				FString Name = It.Value()->Name + ConnectedIsland;
				bIsANorthSouthBridge = true;
				GridLineSystem->CreateLineSystemInfo(It.Value()->GetActorLocation(), Connection->GetActorLocation(), bIsANorthSouthBridge, It.Value()->ArrowOffsetRadians, It.Value()->TriggerRadius, Name);
			}
			else if (TEST_BIT(Connection->PlayerIslandStateEnum, WEST_DIRECTION_GOOD) && TEST_BIT(It.Value()->PlayerIslandStateEnum, EAST_DIRECTION_GOOD))
			{
				FString Name = It.Value()->Name + ConnectedIsland;
				bIsANorthSouthBridge = false;
				GridLineSystem->CreateLineSystemInfo(It.Value()->GetActorLocation(), Connection->GetActorLocation(), bIsANorthSouthBridge, It.Value()->ArrowOffsetRadians, It.Value()->TriggerRadius, Name);
			}
		}
	}
}

void APuzzle::CheckForCompletePuzzle()
{
	bool CheckResult = true;
	
	for(const TPair<FString, APawnIsland*>& pair : IslandSpawnMap)
	{
		CheckResult &= TEST_BIT(pair.Value->PlayerIslandStateEnum, CORRECT_INPUT_COMBINATIONS);
	}

	if(CheckResult)
	{
		GameMode->HandleGameEnd();
	}
}

void APuzzle::GenerateLinkBetweenTwoIslands(APawnIsland* MouseClickedIsland)
{
	FString OverlapIslandName;
	FString MouseClickedIslandName = MouseClickedIsland->Name;

	APawnIsland* OverlapIsland = nullptr;

	for (auto& Island : IslandSpawnMap)
	{
		if (TEST_BIT(Island.Value->PlayerIslandStateEnum, ACTOR_OVERLAPPING))
		{
			OverlapIslandName = Island.Key;
			OverlapIsland = Island.Value;
		}
	}

	for (FString ConnectedIsland : OverlapIsland->ConnectedIslands)
	{
		if (MouseClickedIslandName == ConnectedIsland)
		{
			if (TEST_BIT(MouseClickedIsland->PlayerIslandStateEnum, NORTH_DIRECTION_GOOD) && TEST_BIT(OverlapIsland->PlayerIslandStateEnum, SOUTH_DIRECTION_GOOD))
			{
				if (!TEST_BIT(OverlapIsland->PlayerIslandStateEnum, SOUTH_BRIDGE_1))
				{
					GridLineSystem->DrawLineBetweenIslands(OverlapIslandName, MouseClickedIslandName, 1);
					SET_BIT(OverlapIsland->PlayerIslandStateEnum, SOUTH_BRIDGE_1);
					SET_BIT(MouseClickedIsland->PlayerIslandStateEnum, NORTH_BRIDGE_1);
					SET_BIT(MouseClickedIsland->PlayerIslandStateEnum, MOUSE_CLICKED_CHOSEN);
				}
				else if (!TEST_BIT(OverlapIsland->PlayerIslandStateEnum, SOUTH_BRIDGE_2))
				{
					GridLineSystem->DrawLineBetweenIslands(OverlapIslandName, MouseClickedIslandName, 2);
					SET_BIT(OverlapIsland->PlayerIslandStateEnum, SOUTH_BRIDGE_2);
					SET_BIT(MouseClickedIsland->PlayerIslandStateEnum, NORTH_BRIDGE_2);
					SET_BIT(MouseClickedIsland->PlayerIslandStateEnum, MOUSE_CLICKED_CHOSEN);
				}

				break;
			}

			if (TEST_BIT(MouseClickedIsland->PlayerIslandStateEnum, SOUTH_DIRECTION_GOOD) && TEST_BIT(OverlapIsland->PlayerIslandStateEnum, NORTH_DIRECTION_GOOD))
			{
				if (!TEST_BIT(OverlapIsland->PlayerIslandStateEnum, NORTH_BRIDGE_1))
				{
					GridLineSystem->DrawLineBetweenIslands(OverlapIslandName, MouseClickedIslandName, 1);
					SET_BIT(OverlapIsland->PlayerIslandStateEnum, NORTH_BRIDGE_1);
					SET_BIT(MouseClickedIsland->PlayerIslandStateEnum, SOUTH_BRIDGE_1);
					SET_BIT(MouseClickedIsland->PlayerIslandStateEnum, MOUSE_CLICKED_CHOSEN);
				}
				else if (!TEST_BIT(OverlapIsland->PlayerIslandStateEnum, NORTH_BRIDGE_2))
				{
					GridLineSystem->DrawLineBetweenIslands(OverlapIslandName, MouseClickedIslandName, 2);
					SET_BIT(OverlapIsland->PlayerIslandStateEnum, NORTH_BRIDGE_2);
					SET_BIT(MouseClickedIsland->PlayerIslandStateEnum, SOUTH_BRIDGE_2);
					SET_BIT(MouseClickedIsland->PlayerIslandStateEnum, MOUSE_CLICKED_CHOSEN);
				}
				break;
			}

			if (TEST_BIT(MouseClickedIsland->PlayerIslandStateEnum, EAST_DIRECTION_GOOD) && TEST_BIT(OverlapIsland->PlayerIslandStateEnum, WEST_DIRECTION_GOOD))
			{
				if (!TEST_BIT(OverlapIsland->PlayerIslandStateEnum, WEST_BRIDGE_1))
				{
					GridLineSystem->DrawLineBetweenIslands(OverlapIslandName, MouseClickedIslandName, 1);
					SET_BIT(OverlapIsland->PlayerIslandStateEnum, WEST_BRIDGE_1);
					SET_BIT(MouseClickedIsland->PlayerIslandStateEnum, EAST_BRIDGE_1);				
					SET_BIT(MouseClickedIsland->PlayerIslandStateEnum, MOUSE_CLICKED_CHOSEN);
				}
				else if (!TEST_BIT(OverlapIsland->PlayerIslandStateEnum, WEST_BRIDGE_2))
				{
					GridLineSystem->DrawLineBetweenIslands(OverlapIslandName, MouseClickedIslandName, 2);
					SET_BIT(OverlapIsland->PlayerIslandStateEnum, WEST_BRIDGE_2);
					SET_BIT(MouseClickedIsland->PlayerIslandStateEnum, EAST_BRIDGE_2);
					SET_BIT(MouseClickedIsland->PlayerIslandStateEnum, MOUSE_CLICKED_CHOSEN);
				}

				break;
			}

			if (TEST_BIT(MouseClickedIsland->PlayerIslandStateEnum, WEST_DIRECTION_GOOD) && TEST_BIT(OverlapIsland->PlayerIslandStateEnum, EAST_DIRECTION_GOOD))
			{
				if (!TEST_BIT(OverlapIsland->PlayerIslandStateEnum, EAST_BRIDGE_1))
				{
					GridLineSystem->DrawLineBetweenIslands(OverlapIslandName, MouseClickedIslandName, 1);
					SET_BIT(OverlapIsland->PlayerIslandStateEnum, EAST_BRIDGE_1);
					SET_BIT(MouseClickedIsland->PlayerIslandStateEnum, WEST_BRIDGE_1);
					SET_BIT(MouseClickedIsland->PlayerIslandStateEnum, MOUSE_CLICKED_CHOSEN);
				}
				else if (!TEST_BIT(OverlapIsland->PlayerIslandStateEnum, EAST_BRIDGE_2))
				{
					GridLineSystem->DrawLineBetweenIslands(OverlapIslandName, MouseClickedIslandName, 2);
					SET_BIT(OverlapIsland->PlayerIslandStateEnum, EAST_BRIDGE_2);
					SET_BIT(MouseClickedIsland->PlayerIslandStateEnum, WEST_BRIDGE_2);
					SET_BIT(MouseClickedIsland->PlayerIslandStateEnum, MOUSE_CLICKED_CHOSEN);
				}

				break;
			}
		}
	}

	MouseClickedIsland->OnStatusChange();
	OverlapIsland->OnStatusChange();

	CheckForCompletePuzzle();
	
}


#undef LOCTEXT_NAMESPACE
