// Copyright Epic Games, Inc. All Rights Reserved.

#include "StarBridgesPlayerController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "StarBridgesCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Island/PawnIsland.h"
#include "Kismet/GameplayStatics.h"
#include "Puzzle/Grid.h"
#include "Puzzle/GridLineSystem.h"

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

#define SET_BIT(Bitmask, Bit) (Bitmask |= 1ULL << static_cast<uint64>(Bit))
#define CLEAR_BIT(Bitmask, Bit) (Bitmask &= ~(1ULL << static_cast<uint64>(Bit)))
#define TEST_BIT(Bitmask, Bit) ((Bitmask & (1ULL << static_cast<uint64>(Bit))) > 0)

AStarBridgesPlayerController::AStarBridgesPlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void AStarBridgesPlayerController::GetIslandOnMouseClicked()
{
	if(bIsPlayerMovementStopped)
	{
		// Trace to see what is under the mouse cursor
		FHitResult Hit;
		GetHitResultUnderCursor(ECC_Visibility, false, Hit);

		if(Hit.bBlockingHit && Hit.GetActor()->IsA(APawnIsland::StaticClass()))
		{
			APawnIsland* MouseClickedIsland = Cast<APawnIsland>(Hit.GetActor());
			MouseClickedIsland->OwnedPuzzle->GenerateLinkBetweenTwoIslands(MouseClickedIsland);			
		}
		else if(Hit.bBlockingHit && Hit.GetActor()->IsA(AGridLineSystem::StaticClass()))
		{
			AGridLineSystem* GridLineSystem = Cast<AGridLineSystem>(Hit.GetActor());
			GridLineSystem->DeleteLines(Hit.GetComponent()->GetName());
			GridLineSystem->OwnedPuzzle->ClearBitflagLines(Hit.GetComponent()->GetName());		
		}

		SetIsSpaceBarPressed();
	}
}

void AStarBridgesPlayerController::StopPlayerControllerCharacter()
{
	GetCharacter()->GetCharacterMovement()->StopMovementImmediately();
}

void AStarBridgesPlayerController::ViewGameOverScreen()
{
	
	GameOverScreen = CreateWidget(this, GameOverScreenClass);

	if(GameOverScreen != nullptr)
	{
		GameOverScreen->AddToViewport();
	}
}

void AStarBridgesPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	// keep updating the destination every tick while desired
	if (bMoveToMouseCursor && !bIsPlayerMovementStopped)
	{
		MoveToMouseCursor();
	}
}

void AStarBridgesPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &AStarBridgesPlayerController::OnSetDestinationPressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &AStarBridgesPlayerController::OnSetDestinationReleased);

	InputComponent->BindAction("SetCharacterMovement", IE_Pressed, this, &AStarBridgesPlayerController::SetIsSpaceBarPressed);

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &AStarBridgesPlayerController::GetIslandOnMouseClicked);

	// support touch devices 
	InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AStarBridgesPlayerController::MoveToTouchLocation);
	InputComponent->BindTouch(EInputEvent::IE_Repeat, this, &AStarBridgesPlayerController::MoveToTouchLocation);

	InputComponent->BindAction("ResetVR", IE_Pressed, this, &AStarBridgesPlayerController::OnResetVR);
}

void AStarBridgesPlayerController::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AStarBridgesPlayerController::MoveToMouseCursor()
{
	if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
	{
		if (AStarBridgesCharacter* MyPawn = Cast<AStarBridgesCharacter>(GetPawn()))
		{
			if (MyPawn->GetCursorToWorld())
			{
				UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, MyPawn->GetCursorToWorld()->GetComponentLocation());
			}
		}
	}
	else
	{
		// Trace to see what is under the mouse cursor
		FHitResult Hit;
		GetHitResultUnderCursor(ECC_Visibility, false, Hit);

		if (Hit.bBlockingHit)
		{
			// We hit something, move there
			SetNewMoveDestination(Hit.ImpactPoint);
		}
	}
}

void AStarBridgesPlayerController::MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	FVector2D ScreenSpaceLocation(Location);

	// Trace to see what is under the touch location
	FHitResult HitResult;
	GetHitResultAtScreenPosition(ScreenSpaceLocation, CurrentClickTraceChannel, true, HitResult);
	if (HitResult.bBlockingHit)
	{
		// We hit something, move there
		SetNewMoveDestination(HitResult.ImpactPoint);
	}
}

void AStarBridgesPlayerController::SetNewMoveDestination(const FVector DestLocation)
{
	APawn* const MyPawn = GetPawn();
	if (MyPawn)
	{
		float const Distance = FVector::Dist(DestLocation, MyPawn->GetActorLocation());

		// We need to issue move command only if far enough in order for walk animation to play correctly
		if ((Distance > 120.0f))
		{
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DestLocation);
		}
	}
}

void AStarBridgesPlayerController::SetIsSpaceBarPressed()
{	
	bIsSpaceBarPressed = !bIsSpaceBarPressed;

	SetIsPlayerCharacterMovementStopped();
}

void AStarBridgesPlayerController::SetIsPlayerCharacterMovementStopped()
{
	if(bIsSpaceBarPressed == true && bIsPlayerOverlappingIsland == true)
	{
		bIsPlayerMovementStopped = true;

		GetCharacter()->SetActorRotation(FRotator(0.0f, 180.0f, 0.0f));
		
		GetCharacter()->GetCharacterMovement()->StopMovementImmediately();

		AGrid* Grid = Cast<AGrid>(UGameplayStatics::GetActorOfClass(GetWorld(), AGrid::StaticClass()));
		
		SetViewTargetWithBlend(Grid, CameraBlendTime, EViewTargetBlendFunction::VTBlend_Linear, 0.0f, false);
		
	}
	else
	{
		bIsPlayerMovementStopped = false;
		SetViewTargetWithBlend(GetCharacter(), CameraBlendTime, EViewTargetBlendFunction::VTBlend_Linear, 0.0f, false);
	}
}

void AStarBridgesPlayerController::OnSetDestinationPressed()
{
	if(bIsPlayerMovementStopped == false)
	{
		// set flag to keep updating destination until released
		bMoveToMouseCursor = true;
		bIsSpaceBarPressed = false;
	}
	UE_LOG(LogTemp, Warning, TEXT("Desination Down"));
}

void AStarBridgesPlayerController::OnSetDestinationReleased()
{
	// clear flag to indicate we should stop updating the destination
	bMoveToMouseCursor = false;
	UE_LOG(LogTemp, Warning, TEXT("Desination Up"));
}
