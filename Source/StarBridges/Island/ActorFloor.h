// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActorFloor.generated.h"



USTRUCT(BlueprintType)
struct FPropStruct
{
	GENERATED_BODY()

	//width and height of floor
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Size", meta = (AllowPrivateAccess = "true"))
	float FloorWidth = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Size", meta = (AllowPrivateAccess = "true"))
	float FloorDepth = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Size", meta = (AllowPrivateAccess = "true"))
	int32 GridSize = 0;

	bool GetFloatPropByName(FName PropName, float& OutPut) const;

	bool GetIntPropByName(FName PropName, int32& OutPut) const;
};

UCLASS()
class STARBRIDGES_API AActorFloor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActorFloor();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Assign Floor Integer Variable")
	static bool GetIntByName(UObject* Target, FName VarName, int32 &outInt);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Struct", meta = (AllowPrivateAccess = "true"))
	FPropStruct PropStructStats;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category="PropStruct")
	bool GetFloatFromPropStruct(FName PropName, FPropStruct Struct) const;

	UFUNCTION(BlueprintCallable, Category="PropStruct")
	bool GetIntFromPropStruct(FName PropName, FPropStruct Struct) const;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BaseMesh;

};
