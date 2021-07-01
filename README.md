# Star Bridges

## About
This is a game demo built with Unreal Engine 4.26 with Blueprints and C++.

This is a puzzle demo based on the game Hashi where a player connects bridges between nodes, according to their values, to form one interconnecting path. What I have built to be unique in this demo is the way the grid, a dynamic procedural mesh, and the island nodes are positioned based off the player's decisions on what particular hashi puzzle they choose in the main menu. 

[Gameplay Demo Video](https://youtu.be/SqM581gZNWY)

## Goals

- To create a way to download and read puzzle info from outside the game. 
- Have the puzzle island nodes be able to keep track of their status through bitflags.
- Set up a way for bridges appear when the player connects islands with the ability to destroy them when the player chooses. 
- Have a game over screen display when completing a puzzle with the option of either quiting or playing another one. 

## JSON Reader

[JsonReader.cpp](https://github.com/ajetty/StarBridges/blob/main/Source/StarBridges/Puzzle/JsonReader.cpp)

[JsonReader.h](https://github.com/ajetty/StarBridges/blob/main/Source/StarBridges/Puzzle/JsonReader.h)

The class reads from a JSON file that holds puzzle specifications. When deserialized, the data from the JSON file is transfered to a UStruct format using FJsonObjectConverter::JsonObjectStringToUStruct. That UStruct is then saved to the Game Instance for when the player is transferred to the main playing level from the main menu.  

A JSON file holds a list of puzzle objects with values that create a level such as island grid coordinates and their bridge input values. The image below shows the setup for a JSON puzzle file. Please click to view. This one holds three easy puzzles.  

<a href="https://ajetty.github.io/StarBridges_ReadMePics/StarBridgesJSONExplanation.png">
<img src="https://ajetty.github.io/StarBridges_ReadMePics/StarBridgesJSONExplanation.png" height="250px" alt="Star Bridges JSON" />
</a><br />

### UStruct Format 

[PuzzleDataStruct.h](https://github.com/ajetty/StarBridges/blob/main/Source/StarBridges/Puzzle/PuzzleDataStruct.h)

This structure is set up with one main parent UStruct that holds an array of child UStructs with each child being an individual puzzle. Each child UStruct have their own children that hold objects with multiple values. The diagram below shows the relationship between these UStructs. 

<a href="https://ajetty.github.io/StarBridges_ReadMePics/StarBridgesStructRelationship.png">
<img src="https://ajetty.github.io/StarBridges_ReadMePics/StarBridgesStructRelationship.png" alt="Star Bridges JSON UStruct Relationship Diagram" />
</a><br /> 

## Puzzle Creation

### Puzzle

[Puzzle.h](https://github.com/ajetty/StarBridges/blob/main/Source/StarBridges/Puzzle/Puzzle.h)

[Puzzle.cpp](https://github.com/ajetty/StarBridges/blob/main/Source/StarBridges/Puzzle/Puzzle.cpp)

### Grid

[Grid.h](https://github.com/ajetty/StarBridges/blob/main/Source/StarBridges/Puzzle/Grid.h)

[Grid.cpp](https://github.com/ajetty/StarBridges/blob/main/Source/StarBridges/Puzzle/Grid.cpp)

### Islands

[PawnIsland.h](https://github.com/ajetty/StarBridges/blob/main/Source/StarBridges/Island/PawnIsland.h)

[PawnIsland.cpp](https://github.com/ajetty/StarBridges/blob/main/Source/StarBridges/Island/PawnIsland.cpp)

### Particle Beam Bridges

[GridLineSystem.h](https://github.com/ajetty/StarBridges/blob/main/Source/StarBridges/Puzzle/GridLineSystem.h)

[GridLineSystem.cpp](https://github.com/ajetty/StarBridges/blob/main/Source/StarBridges/Puzzle/GridLineSystem.cpp)

## Menu UI

## Game Mode

