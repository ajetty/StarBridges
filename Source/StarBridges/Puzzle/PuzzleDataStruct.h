// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "PuzzleDataStruct.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FCoordinates
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<int32> node;
};

USTRUCT(BlueprintType)
struct FStates
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<int32> value;
};

USTRUCT(BlueprintType)
struct FConnections
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FString> names;
};

USTRUCT(BlueprintType)
struct FPuzzleDataStruct
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString id;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 grid;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FCoordinates> coordinates;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<int32> input;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FString> name;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FStates> state;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FConnections> connections;	
};

USTRUCT(BlueprintType)
struct FPuzzleListStruct
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FPuzzleDataStruct> puzzle;
};


