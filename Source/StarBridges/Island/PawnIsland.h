// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/ArrowComponent.h"
#include "Components/SphereComponent.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"
#include "StarBridges/Puzzle/Puzzle.h"

#include "PawnIsland.generated.h"


USTRUCT(BlueprintType)
struct FIslandColorState
{
	GENERATED_BODY()

	FIslandColorState()
	{
		CurrentIslandColor = BaseColor;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	FLinearColor OverlapColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	FLinearColor BaseColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	FLinearColor ActiveColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	FLinearColor WarningColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	FLinearColor CompletedColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	FLinearColor MouseClickColor;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	FLinearColor CurrentIslandColor;
	
};

UCLASS()
class STARBRIDGES_API APawnIsland : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APawnIsland();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Puzzle Input", meta = (AllowPrivateAccess = "true"))
	int32 InputNumber;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Puzzle Input", meta = (AllowPrivateAccess = "true"))
	FVector GridLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Puzzle Input", meta = (AllowPrivateAccess = "true"))
	FString Name;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Puzzle Input", meta = (AllowPrivateAccess = "true"))
	TArray<FString> ConnectedIslands;
	
	UPROPERTY(EditAnywhere, Category = "Puzzle Input", meta = (Bitmask, BitmaskEnum = "ESpawnedIslandsStateEnum"))
    int64 CompletedIslandStateEnum;

	UPROPERTY(EditAnywhere, Category = "Puzzle Input", meta = (Bitmask, BitmaskEnum = "ESpawnedIslandsStateEnum"))
	int64 PlayerIslandStateEnum = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	APuzzle* OwnedPuzzle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	float TriggerRadius = 178.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	float ArrowOffsetDegrees = 20.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	float ArrowOffsetRadians;

	void SetInputNumberLabel();

	UFUNCTION()
	void OnBeginMouseOver(UPrimitiveComponent* ClickedComp);
	
	UFUNCTION()
	void OnEndMouseOver(UPrimitiveComponent* ClickedComp);

	void OnStatusChange();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UArrowComponent* NorthArrowBridge1Component;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UArrowComponent* NorthArrowBridge2AComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UArrowComponent* NorthArrowBridge2BComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UArrowComponent* EastArrowBridge1Component;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UArrowComponent* EastArrowBridge2AComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UArrowComponent* EastArrowBridge2BComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UArrowComponent* SouthArrowBridge1Component;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UArrowComponent* SouthArrowBridge2AComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UArrowComponent* SouthArrowBridge2BComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UArrowComponent* WestArrowBridge1Component;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UArrowComponent* WestArrowBridge2AComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UArrowComponent* WestArrowBridge2BComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	FVector NorthArrow1RelativeLocation;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	FVector NorthArrow2ARelativeLocation;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	FVector NorthArrow2BRelativeLocation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	FVector SouthArrow1RelativeLocation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	FVector SouthArrow2ARelativeLocation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	FVector SouthArrow2BRelativeLocation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	FVector EastArrow1RelativeLocation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	FVector EastArrow2ARelativeLocation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	FVector EastArrow2BRelativeLocation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	FVector WestArrow1RelativeLocation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	FVector WestArrow2ARelativeLocation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	FVector WestArrow2BRelativeLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	FIslandColorState IslandColorState;


	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
private:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials Reference", meta = (AllowPrivateAccess = "true"))
	UMaterialInterface* BaseMaterial;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Materials Reference", meta = (AllowPrivateAccess = "true"))
	UMaterialInstanceDynamic* DynamicBaseMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UTextRenderComponent* TextRenderComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USphereComponent* TriggerSphere;

	void CreateMaterialInstance();

	bool CheckCorrectInputCompleteness();
};

inline int COUNT_BIT_INPUTS(uint64 Bitmask, int MaxInputNumber)
{
	std::bitset<sizeof(uint64)> Bitset(Bitmask);

	if(Bitset.count() > static_cast<uint64>(MaxInputNumber))
	{
		return -1;
	}
	else if(Bitset.count() < static_cast<uint64>(MaxInputNumber))
	{
		return 0;
	}
	else
	{
		return 1;
	}
}



