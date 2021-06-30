// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnIsland.h"

#include "SpawnedIslandsStateEnum.h"
#include "Kismet/GameplayStatics.h"
#include "StarBridges/StarBridgesCharacter.h"
#include "StarBridges/StarBridgesGameMode.h"
#include "StarBridges/StarBridgesPlayerController.h"

#define LOCTEXT_NAMESPACE "Island"

#define SET_BIT(Bitmask, Bit) (Bitmask |= 1ULL << static_cast<uint64>(Bit))
#define CLEAR_BIT(Bitmask, Bit) (Bitmask &= ~(1ULL << static_cast<uint64>(Bit)))
#define TEST_BIT(Bitmask, Bit) ((Bitmask & (1ULL << static_cast<uint64>(Bit))) > 0)

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

// Sets default values
APawnIsland::APawnIsland()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ArrowOffsetRadians = FMath::DegreesToRadians(ArrowOffsetDegrees);

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(RootComponent);

	TriggerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Trigger Sphere"));
	TriggerSphere->SetupAttachment(RootComponent);
	TriggerSphere->SetSphereRadius(TriggerRadius);

	NorthArrow1RelativeLocation = FVector(FVector(TriggerRadius, 0.0f, 5.0f));
	NorthArrow2ARelativeLocation = FVector(FVector((0.0f + TriggerRadius * FMath::Cos(ArrowOffsetRadians)), (0.0f + TriggerRadius * FMath::Sin(ArrowOffsetRadians)), 5.0f));
	NorthArrow2BRelativeLocation = FVector(FVector((0.0f + TriggerRadius * FMath::Cos(ArrowOffsetRadians * 17.0f)), (0.0f + TriggerRadius * FMath::Sin(ArrowOffsetRadians * 17.0f)), 5.0f));

	SouthArrow1RelativeLocation = FVector(FVector(-TriggerRadius, 0.0f, 5.0f));
	SouthArrow2ARelativeLocation = FVector(FVector((0.0f + -TriggerRadius * FMath::Cos(ArrowOffsetRadians * -1.0f)), (0.0f + -TriggerRadius * FMath::Sin(ArrowOffsetRadians * -1.0f)), 5.0f));
	SouthArrow2BRelativeLocation = FVector(FVector((0.0f + -TriggerRadius * FMath::Cos(ArrowOffsetRadians * -17.0f)), (0.0f + -TriggerRadius * FMath::Sin(ArrowOffsetRadians * -17.0f)), 5.0f));

	EastArrow1RelativeLocation = FVector(FVector(0.0f, TriggerRadius, 5.0f));
	EastArrow2ARelativeLocation = FVector(FVector((0.0f + TriggerRadius * FMath::Cos(ArrowOffsetRadians * 3.5f)), (0.0f + TriggerRadius * FMath::Sin(ArrowOffsetRadians * 3.5f)), 5.0f));
	EastArrow2BRelativeLocation = FVector(FVector((0.0f + TriggerRadius * FMath::Cos(ArrowOffsetRadians * 5.5f)), (0.0f + TriggerRadius * FMath::Sin(ArrowOffsetRadians * 5.5f)), 5.0f));

	WestArrow1RelativeLocation = FVector(FVector(0.0f, -TriggerRadius, 5.0f));
	WestArrow2ARelativeLocation = FVector(FVector((0.0f + TriggerRadius * FMath::Cos(ArrowOffsetRadians * -3.5f)), (0.0f + TriggerRadius * FMath::Sin(ArrowOffsetRadians * -3.5f)), 5.0f));
	WestArrow2BRelativeLocation = FVector(FVector((0.0f + TriggerRadius * FMath::Cos(ArrowOffsetRadians * -5.5f)), (0.0f + TriggerRadius * FMath::Sin(ArrowOffsetRadians * -5.5f)), 5.0f));

	NorthArrowBridge1Component = CreateDefaultSubobject<UArrowComponent>(TEXT("North Arrow Bridge 1"));
	NorthArrowBridge1Component->SetupAttachment(RootComponent);
	NorthArrowBridge1Component->SetArrowColor(FColor::Red);
	NorthArrowBridge1Component->SetRelativeLocation(NorthArrow1RelativeLocation);

	NorthArrowBridge2AComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("North Arrow Bridge 2A"));
	NorthArrowBridge2AComponent->SetupAttachment(RootComponent);
	NorthArrowBridge2AComponent->SetArrowColor(FColor::Red);
	NorthArrowBridge2AComponent->SetRelativeLocation(NorthArrow2ARelativeLocation);

	NorthArrowBridge2BComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("North Arrow Bridge 2B"));
	NorthArrowBridge2BComponent->SetupAttachment(RootComponent);
	NorthArrowBridge2BComponent->SetArrowColor(FColor::Red);
	NorthArrowBridge2BComponent->SetRelativeLocation(NorthArrow2BRelativeLocation);

	SouthArrowBridge1Component = CreateDefaultSubobject<UArrowComponent>(TEXT("South Arrow Bridge 1"));
	SouthArrowBridge1Component->SetupAttachment(RootComponent);
	SouthArrowBridge1Component->SetArrowColor(FColor::Blue);
	SouthArrowBridge1Component->SetRelativeLocation(SouthArrow1RelativeLocation);
	SouthArrowBridge1Component->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));

	SouthArrowBridge2AComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("South Arrow Bridge 2A"));
	SouthArrowBridge2AComponent->SetupAttachment(RootComponent);
	SouthArrowBridge2AComponent->SetArrowColor(FColor::Blue);
	SouthArrowBridge2AComponent->SetRelativeLocation(SouthArrow2ARelativeLocation);
	SouthArrowBridge2AComponent->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));

	SouthArrowBridge2BComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("South Arrow Bridge 2B"));
	SouthArrowBridge2BComponent->SetupAttachment(RootComponent);
	SouthArrowBridge2BComponent->SetArrowColor(FColor::Blue);
	SouthArrowBridge2BComponent->SetRelativeLocation(SouthArrow2BRelativeLocation);
	SouthArrowBridge2BComponent->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));

	EastArrowBridge1Component = CreateDefaultSubobject<UArrowComponent>(TEXT("East Arrow Bridge 1"));
	EastArrowBridge1Component->SetupAttachment(RootComponent);
	EastArrowBridge1Component->SetArrowColor(FColor::Green);
	EastArrowBridge1Component->SetRelativeLocation(EastArrow1RelativeLocation);
	EastArrowBridge1Component->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));

	EastArrowBridge2AComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("East Arrow Bridge 2A"));
	EastArrowBridge2AComponent->SetupAttachment(RootComponent);
	EastArrowBridge2AComponent->SetArrowColor(FColor::Green);
	EastArrowBridge2AComponent->SetRelativeLocation(EastArrow2ARelativeLocation);
	EastArrowBridge2AComponent->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));

	EastArrowBridge2BComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("East Arrow Bridge 2B"));
	EastArrowBridge2BComponent->SetupAttachment(RootComponent);
	EastArrowBridge2BComponent->SetArrowColor(FColor::Green);
	EastArrowBridge2BComponent->SetRelativeLocation(EastArrow2BRelativeLocation);
	EastArrowBridge2BComponent->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));

	WestArrowBridge1Component = CreateDefaultSubobject<UArrowComponent>(TEXT("West Arrow Bridge 1"));
	WestArrowBridge1Component->SetupAttachment(RootComponent);
	WestArrowBridge1Component->SetArrowColor(FColor::Purple);
	WestArrowBridge1Component->SetRelativeLocation(WestArrow1RelativeLocation);
	WestArrowBridge1Component->SetRelativeRotation(FRotator(0.0f, 270.0f, 0.0f));

	WestArrowBridge2AComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("West Arrow Bridge 2A"));
	WestArrowBridge2AComponent->SetupAttachment(RootComponent);
	WestArrowBridge2AComponent->SetArrowColor(FColor::Purple);
	WestArrowBridge2AComponent->SetRelativeLocation(WestArrow2ARelativeLocation);
	WestArrowBridge2AComponent->SetRelativeRotation(FRotator(0.0f, 270.0f, 0.0f));

	WestArrowBridge2BComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("West Arrow Bridge 2B"));
	WestArrowBridge2BComponent->SetupAttachment(RootComponent);
	WestArrowBridge2BComponent->SetArrowColor(FColor::Purple);
	WestArrowBridge2BComponent->SetRelativeLocation(WestArrow2BRelativeLocation);
	WestArrowBridge2BComponent->SetRelativeRotation(FRotator(0.0f, 270.0f, 0.0f));

	TextRenderComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Text Render Component"));
	TextRenderComponent->SetupAttachment(RootComponent);
	TextRenderComponent->SetXScale(1.0f);
	TextRenderComponent->SetYScale(1.0f);
	TextRenderComponent->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
	TextRenderComponent->SetWorldSize(200.0f);

	BaseMesh->OnBeginCursorOver.AddDynamic(this, &APawnIsland::OnBeginMouseOver);
	BaseMesh->OnEndCursorOver.AddDynamic(this, &APawnIsland::OnEndMouseOver);
}

void APawnIsland::SetInputNumberLabel()
{
	FFormatNamedArguments Args;
	Args.Add("Island Input Number Label", InputNumber);

	FText FormattedText = FText::Format(LOCTEXT("Island", "{Island Input Number Label}"), Args);

	TextRenderComponent->SetText(FormattedText);
}

void APawnIsland::OnBeginMouseOver(UPrimitiveComponent* ClickedComp)
{
	//bool OverlapStatus = TEST_BIT(PlayerIslandStateEnum, ACTOR_OVERLAPPING);

	// if (OverlapStatus)
	// {
	// 	IslandColorState.CurrentIslandColor = IslandColorState.OverlapColor;
	// }
	// else
	// {
	// 	IslandColorState.CurrentIslandColor = IslandColorState.ActiveColor;
	// }

	OnStatusChange();
	DynamicBaseMaterial->SetVectorParameterValue("Color", IslandColorState.ActiveColor);
}

void APawnIsland::OnEndMouseOver(UPrimitiveComponent* ClickedComp)
{
	OnStatusChange();
}

void APawnIsland::OnStatusChange()
{
	int32 InputStatus = COUNT_BIT_INPUTS(PlayerIslandStateEnum, InputNumber);
	bool OverlapStatus = TEST_BIT(PlayerIslandStateEnum, ACTOR_OVERLAPPING);

	if(!OverlapStatus)
	{
		if(InputStatus == 1)
		{
			SET_BIT(PlayerIslandStateEnum, INPUT_BRIDGE_TOTAL_REACHED);
			IslandColorState.CurrentIslandColor = IslandColorState.CompletedColor;
		}
		else if(InputStatus == -1)
		{
			SET_BIT(PlayerIslandStateEnum, INPUT_BRIDGE_TOTAL_REACHED);
			IslandColorState.CurrentIslandColor = IslandColorState.WarningColor;
		}
		else
		{
			CLEAR_BIT(PlayerIslandStateEnum, INPUT_BRIDGE_TOTAL_REACHED);
			IslandColorState.CurrentIslandColor = IslandColorState.BaseColor;
		}
	}

	CLEAR_BIT(PlayerIslandStateEnum, MOUSE_CLICKED_CHOSEN);
	DynamicBaseMaterial->SetVectorParameterValue("Color", IslandColorState.CurrentIslandColor);
	CheckCorrectInputCompleteness();
	
}

// Called when the game starts or when spawned
void APawnIsland::BeginPlay()
{
	Super::BeginPlay();

	TriggerSphere->OnComponentBeginOverlap.AddDynamic(this, &APawnIsland::OnOverlapBegin);
	TriggerSphere->OnComponentEndOverlap.AddDynamic(this, &APawnIsland::OnOverlapEnd);

	CreateMaterialInstance();

	TextRenderComponent->SetTextRenderColor(FColor::Black);

	OwnedPuzzle = Cast<APuzzle>(UGameplayStatics::GetActorOfClass(GetWorld(), APuzzle::StaticClass()));
}


void APawnIsland::CreateMaterialInstance()
{
	DynamicBaseMaterial = UMaterialInstanceDynamic::Create(BaseMaterial->GetMaterial(), this);
	BaseMesh->SetMaterial(0, DynamicBaseMaterial);
}

bool APawnIsland::CheckCorrectInputCompleteness()
{

	uint64 PlayerBitmask = PlayerIslandStateEnum & static_cast<uint64>(255);
	uint64 CompletedBitmask = CompletedIslandStateEnum & static_cast<uint64>(255);
	
	bool Result = ((PlayerBitmask ^ CompletedBitmask) == 0);
	if(Result)
	{
		SET_BIT(PlayerIslandStateEnum, CORRECT_INPUT_COMBINATIONS);
	}
	else
	{
		CLEAR_BIT(PlayerIslandStateEnum, CORRECT_INPUT_COMBINATIONS);
	}
	return Result;
}

void APawnIsland::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                 const FHitResult& SweepResult)
{
	if (AStarBridgesCharacter* tempActor = Cast<AStarBridgesCharacter>(OtherActor))
	{
		SET_BIT(PlayerIslandStateEnum, ACTOR_OVERLAPPING);
		IslandColorState.CurrentIslandColor = IslandColorState.OverlapColor;
		DynamicBaseMaterial->SetVectorParameterValue("Color", IslandColorState.CurrentIslandColor);

		tempActor->PlayerController->bIsPlayerOverlappingIsland = true;
	}
}

void APawnIsland::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AStarBridgesCharacter* tempActor = Cast<AStarBridgesCharacter>(OtherActor))
	{
		CLEAR_BIT(PlayerIslandStateEnum, ACTOR_OVERLAPPING);
		OnStatusChange();

		tempActor->PlayerController->bIsPlayerOverlappingIsland = false;
	}
}

// Called every frame
void APawnIsland::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APawnIsland::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

#undef LOCTEXT_NAMESPACE
