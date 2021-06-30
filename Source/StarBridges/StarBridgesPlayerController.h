// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "StarBridgesPlayerController.generated.h"

UCLASS()
class AStarBridgesPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AStarBridgesPlayerController();

	bool bIsPlayerOverlappingIsland;
	bool bIsPlayerMovementStopped;
	bool bIsAnIslandMouseClicked;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess))
	float CameraBlendTime;

	void StopPlayerControllerCharacter();

	void ViewGameOverScreen();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puzzle")
	TSubclassOf<UUserWidget> GameOverScreenClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Puzzle", meta = (AllowPrivateAccess = "true"))
	UUserWidget* GameOverScreen;

protected:

	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	bool bIsSpaceBarPressed;
	
	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Navigate player to the current mouse cursor location. */
	void MoveToMouseCursor();

	/** Navigate player to the current touch location. */
	void MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location);
	
	/** Navigate player to the given world location. */
	void SetNewMoveDestination(const FVector DestLocation);

	/** Input handlers for SetDestination action. */
	void OnSetDestinationPressed();
	void OnSetDestinationReleased();

	void SetIsSpaceBarPressed();

	void SetIsPlayerCharacterMovementStopped();

	void GetIslandOnMouseClicked();
};


