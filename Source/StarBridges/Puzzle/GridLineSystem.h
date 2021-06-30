// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/Actor.h"
#include "StarBridges/Island/PawnIsland.h"
#include "StarBridges/Island/GridLineDrawInfo.h"
#include "GridLineSystem.generated.h"


UCLASS()
class STARBRIDGES_API AGridLineSystem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGridLineSystem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	TArray<struct FGridLineDrawInfo> GridLineDrawInfoStructs = TArray<struct FGridLineDrawInfo>();

	void GenerateLinesForIslands();

	void CreateLineSystemInfo(FVector StartLocation, FVector EndLocation, bool bIsNorthSouth, float RadianOffset, float TriggerRadius, FString Name);

	float GetHalfVectorDistance(FVector Start, FVector End);

	FVector GetCapsuleWorldLocation(FVector Start, FVector End, float Distance, bool bIsNorthSouth);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Grid Line Structs", meta = (TitleProperty = "Trigger Sphere Map"))
	TMap<FString, UCapsuleComponent*> LevelIslandTriggerSphereMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Grid Line Structs", meta = (TitleProperty = "Particle System Map"))
	TMap<FString, UParticleSystemComponent*> LevelIslandParticleSystemMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Components")
	UParticleSystem* BeamParticleTemplate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	FLinearColor LineBaseColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	FLinearColor LineActiveColor;

	bool DrawLineBetweenIslands(FString IslandA, FString IslandB, int32 BridgeNumber);

	UFUNCTION()
	void OnBeginMouseOver(UPrimitiveComponent* ClickedComp);
	
	UFUNCTION()
	void OnEndMouseOver(UPrimitiveComponent* ClickedComp);

	void CreateLine(FString IslandConnectionStructName, FVector StartLocation, FVector EndLocation, bool bIsANorthSouthBridge, int32 BridgeNumber);

	void DeleteLines(FString LineName);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	APuzzle* OwnedPuzzle;
	

};




