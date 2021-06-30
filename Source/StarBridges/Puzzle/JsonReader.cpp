// Fill out your copyright notice in the Description page of Project Settings.


#include "JsonReader.h"

#include "JsonObjectConverter.h"

void AJsonReader::ReadJsonFiles()
{
	const FString JsonFilePath = FPaths::ProjectContentDir() + "JSON/PuzzleEasy5x5.json";

	//read JSON file
	FString JsonString;

	if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*JsonFilePath))
	{
		UE_LOG(LogTemp, Warning, TEXT("JsonFilePath: FILE FOUND"));
	}

	FFileHelper::LoadFileToString(JsonString, *JsonFilePath);

	UE_LOG(LogTemp, Warning, TEXT("%s"), *JsonString);

	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonString);
	
	//FJsonData JsonData;
	if(FJsonObjectConverter::JsonObjectStringToUStruct<FPuzzleListStruct>(JsonString, &PuzzleListStruct,0, 0))
	{
		UE_LOG(LogTemp, Warning, TEXT("CONVERTED"));
	}

	for(int i = 0; i < PuzzleListStruct.puzzle.Num(); i++)
	{
		PuzzleNames.Add(PuzzleListStruct.puzzle[i].id);
	}
}

TArray<FString> AJsonReader::GetPuzzleMenuInfo()
{
	return PuzzleNames;
}

FPuzzleListStruct AJsonReader::PuzzleMenuChoice()
{
	return PuzzleListStruct;
}
