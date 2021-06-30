// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "PuzzleDataStruct.h"
#include "UObject/Object.h"
#include "JsonReader.generated.h"

/**
 * This is a class that reads from a Json file, parses it, and then puts the information into a UStruct format. The information within the read Json file holds information about the puzzle that is being played. 
 */
UCLASS()
class STARBRIDGES_API AJsonReader : public AActor
{
	GENERATED_BODY()

public:
	/*
	 * Function that takes a filepath, downloads a Json file then parses it into a string format. From this string, its deserialized into a multi UStruct format. 
	 */
	UFUNCTION(BlueprintCallable)
	void ReadJsonFiles();

	/*
	 * A UStruct that holds all parent objects of the Json file. All children objects are held in their own UStructs that are stored in this parent. 
	 */
	UPROPERTY(VisibleAnywhere, Category = Grid)
	FPuzzleListStruct PuzzleListStruct;

	/*
	 * An array that holds each Puzzle's ID name for selection in the UI menu system. 
	 */ 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Grid)
	TArray<FString> PuzzleNames;

	/*
	 * A blueprint callable function that returns the variable TArray<FString> PuzzleNames.
	 */
	UFUNCTION(BlueprintCallable)
	TArray<FString> GetPuzzleMenuInfo();

	/*
	 * A blueprint callable function that returns FPuzzleListStruct PuzzleListStruct, a main UStruct that holds all children structs and information about the Json puzzle file that's been read. 
	 */
	UFUNCTION(BlueprintCallable)
	FPuzzleListStruct PuzzleMenuChoice();
	
};
