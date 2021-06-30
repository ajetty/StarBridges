// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorFloor.h"

bool FPropStruct::GetFloatPropByName(FName PropName, float& OutPut) const
{
	UStruct* uStruct = FPropStruct::StaticStruct();
	FProperty* prop = uStruct->FindPropertyByName(PropName);

	if(!prop)
	{
		return false;
	}

	FFloatProperty* floatProperty = CastField<FFloatProperty>(prop);

	if(!floatProperty)
	{
		return false;
	}

	const void* structAddress = prop->ContainerPtrToValuePtr<void>(this);
	OutPut = floatProperty->GetFloatingPointPropertyValue(structAddress);
	return true;
}

bool FPropStruct::GetIntPropByName(FName PropName, int32& OutPut) const
{
	UStruct* uStruct = FPropStruct::StaticStruct();
	FProperty* prop = uStruct->FindPropertyByName(PropName);

	if(!prop)
	{
		return false;
	}

	FIntProperty* intProperty = CastField<FIntProperty>(prop);

	if(!intProperty)
	{
		return false;
	}

	const void* structAddress = prop->ContainerPtrToValuePtr<void>(this);
	OutPut = intProperty->GetSignedIntPropertyValue(structAddress);
	return true;
}

// Sets default values
AActorFloor::AActorFloor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(RootComponent);

}

bool AActorFloor::GetIntByName(UObject* Target, FName VarName, int32 &outInt)
{
	if(Target)
	{
		int32 ResultInt;
		FIntProperty* IntProperty = FindFProperty<FIntProperty>(Target->GetClass(), VarName);
		if(IntProperty)
		{
			ResultInt = IntProperty->GetPropertyValue_InContainer(Target);
			outInt = ResultInt;
			return true;
		}
	}
	return false;
}

// Called when the game starts or when spawned
void AActorFloor::BeginPlay()
{
	Super::BeginPlay();
	
}

bool AActorFloor::GetFloatFromPropStruct(FName PropName, FPropStruct Struct) const
{
	float Result;
	bool Success;
	Success = Struct.GetFloatPropByName(PropName, Result);
	return Success;
}

bool AActorFloor::GetIntFromPropStruct(FName PropName, FPropStruct Struct) const
{
	int32 Result;
	bool Success;
	Success = Struct.GetIntPropByName(PropName, Result);
	return Success;
}

// Called every frame
void AActorFloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

