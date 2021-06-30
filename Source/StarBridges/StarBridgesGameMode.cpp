// Copyright Epic Games, Inc. All Rights Reserved.

#include "StarBridgesGameMode.h"

#include <gameux.h>

#include "StarBridgesPlayerController.h"
#include "StarBridgesCharacter.h"
#include "StarBridgesGameInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

AStarBridgesGameMode::AStarBridgesGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AStarBridgesPlayerController::StaticClass();
	
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void AStarBridgesGameMode::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<AStarBridgesPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

}

void AStarBridgesGameMode::HandleGameStart()
{
	AStarBridgesCharacter* PlayerCharacter = Cast<AStarBridgesCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	
	PlayerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	PlayerController->GetCharacter()->SetActorLocation(FVector(0.0f, 0.0f, 5.0f));

	if (GetGameInstance())
	{
		UStarBridgesGameInstance* GameInstance = Cast<UStarBridgesGameInstance>(GetGameInstance());

		Puzzle = Cast<APuzzle>(UGameplayStatics::GetActorOfClass(GetWorld(), APuzzle::StaticClass()));
	
		Puzzle->PuzzleLevelSelection = GameInstance->PuzzleLevel;

		Puzzle->SetProperties(GameInstance->PuzzleListStruct, GameInstance->PuzzleLevel);
		
		Puzzle->LoadPuzzle();
	}
}

void AStarBridgesGameMode::HandleGameEnd()
{
	AStarBridgesCharacter* PlayerCharacter = Cast<AStarBridgesCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	
	PlayerCharacter->GetCharacterMovement()->DisableMovement();

	PlayerController->ViewGameOverScreen();

	Puzzle->EndGame();
}


