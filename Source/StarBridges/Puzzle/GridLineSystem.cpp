// Fill out your copyright notice in the Description page of Project Settings.


#include "GridLineSystem.h"

#include <basetyps.h>

#include "CollisionQueryFilterCallbackCore.h"
#include "DrawDebugHelpers.h"
#include "Chaos/ChaosPerfTest.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AGridLineSystem::AGridLineSystem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
}

// Called when the game starts or when spawned
void AGridLineSystem::BeginPlay()
{
	Super::BeginPlay();

	OwnedPuzzle = Cast<APuzzle>(UGameplayStatics::GetActorOfClass(GetWorld(), APuzzle::StaticClass()));
}

// Called every frame
void AGridLineSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AGridLineSystem::GenerateLinesForIslands()
{
	//set up a trigger capsule component for interactivity and a particle system to hold a particle beam for visualization 
	for (FGridLineDrawInfo Info : GridLineDrawInfoStructs)
	{
		CreateLine(Info.Name, Info.StartLocationBridgeVector1, Info.EndLocationBridgeVector1, Info.bIsANorthSouthBridge, 1);
		CreateLine(Info.Name, Info.StartLocationBridgeVector2A, Info.EndLocationBridgeVector2A, Info.bIsANorthSouthBridge, 2);
		CreateLine(Info.Name, Info.StartLocationBridgeVector2B, Info.EndLocationBridgeVector2B, Info.bIsANorthSouthBridge, 3);
	}
}

void AGridLineSystem::CreateLineSystemInfo(FVector StartLocation, FVector EndLocation, bool bIsNorthSouth, float RadianOffset, float TriggerRadius, FString Name)
{
	FGridLineDrawInfo GridLineDrawInfo;

	if (bIsNorthSouth)
	{
		GridLineDrawInfo.StartLocationBridgeVector1 = StartLocation + FVector(TriggerRadius, 0.0f, 5.0f);
		GridLineDrawInfo.StartLocationBridgeVector2A = StartLocation + FVector((0.0f + TriggerRadius * FMath::Cos(RadianOffset)), (0.0f + TriggerRadius * FMath::Sin(RadianOffset)), 5.0f);
		GridLineDrawInfo.StartLocationBridgeVector2B = StartLocation + FVector((0.0f + TriggerRadius * FMath::Cos(RadianOffset * 17.0f)), (0.0f + TriggerRadius * FMath::Sin(RadianOffset * 17.0f)),
		                                                                       5.0f);

		GridLineDrawInfo.EndLocationBridgeVector1 = EndLocation + FVector(-TriggerRadius, 0.0f, 5.0f);
		GridLineDrawInfo.EndLocationBridgeVector2A = EndLocation + FVector((0.0f + -TriggerRadius * FMath::Cos(RadianOffset * -1.0f)), (0.0f + -TriggerRadius * FMath::Sin(RadianOffset * -1.0f)),
		                                                                   5.0f);
		GridLineDrawInfo.EndLocationBridgeVector2B = EndLocation + FVector((0.0f + -TriggerRadius * FMath::Cos(RadianOffset * -17.0f)), (0.0f + -TriggerRadius * FMath::Sin(RadianOffset * -17.0f)),
		                                                                   5.0f);
	}
	else
	{
		GridLineDrawInfo.StartLocationBridgeVector1 = StartLocation + FVector(FVector(0.0f, TriggerRadius, 5.0f));
		GridLineDrawInfo.StartLocationBridgeVector2A = StartLocation + FVector(FVector((0.0f + TriggerRadius * FMath::Cos(RadianOffset * 3.5f)),
		                                                                               (0.0f + TriggerRadius * FMath::Sin(RadianOffset * 3.5f)), 5.0f));
		GridLineDrawInfo.StartLocationBridgeVector2B = StartLocation + FVector(FVector((0.0f + TriggerRadius * FMath::Cos(RadianOffset * 5.5f)),
		                                                                               (0.0f + TriggerRadius * FMath::Sin(RadianOffset * 5.5f)), 5.0f));

		GridLineDrawInfo.EndLocationBridgeVector1 = EndLocation + FVector(FVector(0.0f, -TriggerRadius, 5.0f));
		GridLineDrawInfo.EndLocationBridgeVector2A = EndLocation + FVector(FVector((0.0f + TriggerRadius * FMath::Cos(RadianOffset * -3.5f)), (0.0f + TriggerRadius * FMath::Sin(RadianOffset * -3.5f)),
		                                                                           5.0f));
		GridLineDrawInfo.EndLocationBridgeVector2B = EndLocation + FVector(FVector((0.0f + TriggerRadius * FMath::Cos(RadianOffset * -5.5f)), (0.0f + TriggerRadius * FMath::Sin(RadianOffset * -5.5f)),
		                                                                           5.0f));
	}

	GridLineDrawInfo.Name = Name;
	GridLineDrawInfo.bIsANorthSouthBridge = bIsNorthSouth;

	GridLineDrawInfoStructs.Add(GridLineDrawInfo);
}

float AGridLineSystem::GetHalfVectorDistance(FVector Start, FVector End)
{
	return FVector::Distance(Start, End) / 2.0f;
}

FVector AGridLineSystem::GetCapsuleWorldLocation(FVector Start, FVector End, float Distance, bool bIsNorthSouth)
{
	if(bIsNorthSouth)
	{
		return FVector(Start.X + Distance, Start.Y, Start.Z);
	}

	return FVector(Start.X, Start.Y + Distance, Start.Z);
}

bool AGridLineSystem::DrawLineBetweenIslands(FString IslandA, FString IslandB, int32 BridgeNumber)
{
	bool bIsSuccessful = false;
	FString BridgeMapName = IslandA + IslandB + " Bridge " + "1";
	FString BridgeMapNameB = IslandB + IslandA + " Bridge " + "1";
	
	FString BridgeMapName2 = IslandA + IslandB + " Bridge " + "2";
	FString BridgeMapName2B = IslandB + IslandA + " Bridge " + "2";

	FString BridgeMapName3 = IslandA + IslandB + " Bridge " + "3";
	FString BridgeMapName3B = IslandB + IslandA + " Bridge " + "3";
	if(BridgeNumber == 1)
	{
		if(LevelIslandParticleSystemMap.Find(BridgeMapName))
		{
			LevelIslandParticleSystemMap[BridgeMapName]->SetFloatParameter("Alpha", 1.0f);
			bIsSuccessful = true;
		}

		if(LevelIslandParticleSystemMap.Find(BridgeMapNameB))
		{
			LevelIslandParticleSystemMap[BridgeMapNameB]->SetFloatParameter("Alpha", 1.0f);
			bIsSuccessful = true;
		}
	}
	else
	{
		if(LevelIslandParticleSystemMap.Find(BridgeMapName2))
		{
			LevelIslandParticleSystemMap[BridgeMapName2]->SetFloatParameter("Alpha", 1.0f);
			LevelIslandParticleSystemMap[BridgeMapName3]->SetFloatParameter("Alpha", 1.0f);
			LevelIslandParticleSystemMap[BridgeMapName]->SetFloatParameter("Alpha", 0.0f);
			bIsSuccessful = true;
		}

		if(LevelIslandParticleSystemMap.Find(BridgeMapName2B))
		{
			LevelIslandParticleSystemMap[BridgeMapName2B]->SetFloatParameter("Alpha", 1.0f);
			LevelIslandParticleSystemMap[BridgeMapName3B]->SetFloatParameter("Alpha", 1.0f);
			LevelIslandParticleSystemMap[BridgeMapNameB]->SetFloatParameter("Alpha", 0.0f);
			bIsSuccessful = true;
		}
	}

	return bIsSuccessful;
}

void AGridLineSystem::OnBeginMouseOver(UPrimitiveComponent* ClickedComp)
{
	FString Name = ClickedComp->GetName().Left(2);

	FString Bridge1 = Name + " Bridge " + "1";
	FString Bridge2 = Name + " Bridge " + "2";
	FString Bridge3 = Name + " Bridge " + "3";

	LevelIslandParticleSystemMap[Bridge1]->SetColorParameter("Color", LineActiveColor);
	LevelIslandParticleSystemMap[Bridge2]->SetColorParameter("Color", LineActiveColor);
	LevelIslandParticleSystemMap[Bridge3]->SetColorParameter("Color", LineActiveColor);

}

void AGridLineSystem::OnEndMouseOver(UPrimitiveComponent* ClickedComp)
{

	FString Name = ClickedComp->GetName().Left(2);

	FString Bridge1 = Name + " Bridge " + "1";
	FString Bridge2 = Name + " Bridge " + "2";
	FString Bridge3 = Name + " Bridge " + "3";

	
	LevelIslandParticleSystemMap[Bridge1]->SetColorParameter("Color", LineBaseColor);
	LevelIslandParticleSystemMap[Bridge2]->SetColorParameter("Color", LineBaseColor);
	LevelIslandParticleSystemMap[Bridge3]->SetColorParameter("Color", LineBaseColor);
	
}

void AGridLineSystem::CreateLine(FString IslandConnectionStructName, FVector StartLocation, FVector EndLocation, bool bIsANorthSouthBridge, int32 BridgeNumber)
{
	float Distance = GetHalfVectorDistance(StartLocation, EndLocation);
	FVector WorldLocation = GetCapsuleWorldLocation(StartLocation, EndLocation, Distance, bIsANorthSouthBridge);

	float CapsuleRadius = 30.0f;
	FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::KeepWorld, false);

	FRotator CapsuleRelativeRotation = (bIsANorthSouthBridge) ? FRotator(90.0f, 0.0f, 0.0f) : FRotator(90.0f, 0.0f, 90.0f);

	FString BridgeMapName = IslandConnectionStructName + " Bridge " + FString::FromInt(BridgeNumber);
	FString BridgeCapsuleName = IslandConnectionStructName + " Bridge " + FString::FromInt(BridgeNumber) + " Capsule";
	FString BridgeParticleName = IslandConnectionStructName + " Bridge " + FString::FromInt(BridgeNumber) + " Beam";
	
	LevelIslandTriggerSphereMap.Add(BridgeMapName, NewObject<UCapsuleComponent>(this, UCapsuleComponent::StaticClass(), *BridgeMapName));
	LevelIslandTriggerSphereMap[BridgeMapName]->AttachToComponent(RootComponent, AttachmentTransformRules);
	LevelIslandTriggerSphereMap[BridgeMapName]->SetWorldLocation(WorldLocation);
	LevelIslandTriggerSphereMap[BridgeMapName]->SetRelativeRotation(CapsuleRelativeRotation);
	LevelIslandTriggerSphereMap[BridgeMapName]->SetCapsuleSize(CapsuleRadius, Distance);
	LevelIslandTriggerSphereMap[BridgeMapName]->RegisterComponent();
	LevelIslandTriggerSphereMap[BridgeMapName]->SetGenerateOverlapEvents(true);
	LevelIslandTriggerSphereMap[BridgeMapName]->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	LevelIslandTriggerSphereMap[BridgeMapName]->SetCollisionObjectType(ECC_WorldStatic);
	LevelIslandTriggerSphereMap[BridgeMapName]->SetNotifyRigidBodyCollision(true);
	LevelIslandTriggerSphereMap[BridgeMapName]->SetVisibility(true);
	LevelIslandTriggerSphereMap[BridgeMapName]->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	LevelIslandTriggerSphereMap[BridgeMapName]->OnBeginCursorOver.AddDynamic(this, &AGridLineSystem::OnBeginMouseOver);
	LevelIslandTriggerSphereMap[BridgeMapName]->OnEndCursorOver.AddDynamic(this, &AGridLineSystem::OnEndMouseOver);

	LevelIslandParticleSystemMap.Add(BridgeMapName, NewObject<UParticleSystemComponent>(this, UParticleSystemComponent::StaticClass(), *BridgeParticleName));
	LevelIslandParticleSystemMap[BridgeMapName]->SetTemplate(BeamParticleTemplate);
	LevelIslandParticleSystemMap[BridgeMapName]->AttachToComponent(RootComponent, AttachmentTransformRules);
	LevelIslandParticleSystemMap[BridgeMapName]->RegisterComponent();
	LevelIslandParticleSystemMap[BridgeMapName]->SetBeamSourcePoint(0, StartLocation, 0);
	LevelIslandParticleSystemMap[BridgeMapName]->SetBeamEndPoint(0, EndLocation);
	
}

void AGridLineSystem::DeleteLines(FString LineName)
{
	FString Name = LineName.Left(2);

	FString Bridge1 = Name + " Bridge " + "1";
	FString Bridge2 = Name + " Bridge " + "2";
	FString Bridge3 = Name + " Bridge " + "3";

	LevelIslandParticleSystemMap[Bridge1]->SetFloatParameter("Alpha", 0.0f);
	LevelIslandParticleSystemMap[Bridge2]->SetFloatParameter("Alpha", 0.0f);
	LevelIslandParticleSystemMap[Bridge3]->SetFloatParameter("Alpha", 0.0f);
}
