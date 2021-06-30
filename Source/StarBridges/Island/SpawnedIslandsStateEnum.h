// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "SpawnedIslandsStateEnum.generated.h"

/**
 * 
 */
UENUM(meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class ESpawnedIslandsStateEnum : uint64
{
	NONE						= 0, 
	NORTH_BRIDGE_1				= 1, //0
	NORTH_BRIDGE_2				= 2, //1
	EAST_BRIDGE_1				= 4, //2
	EAST_BRIDGE_2				= 8, //3
	SOUTH_BRIDGE_1				= 16, //4
	SOUTH_BRIDGE_2				= 32, //5
	WEST_BRIDGE_1				= 64, //6
	WEST_BRIDGE_2				= 128, //7
	INPUT_BRIDGE_TOTAL_REACHED	= 256, //8
	CORRECT_INPUT_COMBINATIONS	= 512, //9
	NORTH_DIRECTION_GOOD		= 1024, //10
	EAST_DIRECTION_GOOD			= 2048, //11
	SOUTH_DIRECTION_GOOD		= 4096, //12
	WEST_DIRECTION_GOOD			= 8192, //13
	ACTOR_OVERLAPPING			= 16384, //14
	MOUSE_CLICKED_CHOSEN		= 32768 //15
};
ENUM_CLASS_FLAGS(ESpawnedIslandsStateEnum);

