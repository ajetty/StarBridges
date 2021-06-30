// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "ProceduralMeshComponent.h"

#include "Grid.generated.h"

/*
 * This class holds the functions and variables needed to render a procedural mesh. This mesh creates the puzzle grid used to calculate the layout of the level for gameplay.
 */
UCLASS()
class STARBRIDGES_API AGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	/**
	 * Sets default values for this actor's properties.
	 */
	AGrid(); 

	/**
	* Called every frame.
	*/
	virtual void Tick(float DeltaTime) override;

	/**
	* Integer that is assigned the number of rows in the grid.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Size", meta = (AllowPrivateAccess = "true"))
	int RowsNum = 0;   

	/**
	* Integer that is assigned the number of columns in the grid.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Size", meta = (AllowPrivateAccess = "true"))
	int ColsNum = 0;

	/**
	* Float that is assigned one side length of a grid tile.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Size", meta = (AllowPrivateAccess = "true"))
	float TileSideLength = 200.0f; 

	/**
	* Float that is assigned the value of the rendered grid lines 'width'/'thickness' in the procedural mesh.
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Line", meta = (AllowPrivateAccess = "true"))
	float LineThickness = 10.0f;   

	/**
	* RGBA color value of the grid lines. Only RGB is used for the variable.
	* This is a parameter for the function Create Material Instance. Sets the RGB value of the lines material instance.
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Line", meta = (AllowPrivateAccess = "true"))
	FLinearColor LineColor = FLinearColor(1.0f, 0.0f, 0.0f);    

	/**
	* Float that represents the Alpha value of the RGBA color value of the grid lines. This is a parameter Create Material Instance.
	* Sets the Alpha value of the line material instance.
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Line", meta = (AllowPrivateAccess = "true"))
	float LineOpacity = 1.0f;

	/**
	* Contains functions that create procedural mesh. Used when initializing or changing Grid Class values. 
	*/
	void GenerateProceduralMesh();

	/**
	* Takes a column and row from the local puzzle grid space, from the Json file, and interprets it into world space on the procedural grid. 
	*/
	UFUNCTION(BlueprintCallable, Category="Grid")
	FVector GetGridPosition(int Column, int Row);

	/**
	* Calculates the half width of a puzzle's grid from the grid's center position by using the number of rows in the puzzle. 
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Grid")
	float GetRowHalfWidth();

	/**
	* Calculates the center position of the puzzle and returning it in the form of a FVector. 
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Grid")
	FVector GetCenterPosition();

protected:
	/**
	* Called when the game starts or when spawned.
	*/
	virtual void BeginPlay() override;

	/**
	* Called when an instance of this class is placed (in editor) or spawned. Meant to reinitialize actor components when changed. Calls functions to instantiate a Material Instance for procedural mesh and the procedural mesh. 
	*/
	virtual void OnConstruction(const FTransform& Transform) override;

private:
	/**
	* A Dynamic Material Instance that holds the material for the lines of the grid created by the procedural mesh. Has the ability to change material properties during runtime. 
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Materials Reference", meta = (AllowPrivateAccess = "true"))
	UMaterialInstanceDynamic* LinesMaterial;

	/**
	* A Procedural Mesh that creates a grid mesh, a collection of triangles, indices and vertices, that forms squares in a row and column formation.
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Line", meta = (AllowPrivateAccess = "true"))
	UProceduralMeshComponent* ProceduralMesh; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials Reference", meta = (AllowPrivateAccess = "true"))
	UMaterialInterface* BaseMaterial; ///< Material used to create a Dynamic Material Instance. Assigned by content directory filepath with ConstructorHelpers::FObjectFinder in constructor.

	/**
	* A value that holds varying offset values of Tile Length Size. Value that helps in the creation of triangles vertices. 
	*/
	float LineStartPointValue;  

	/**
	* A value that holds varying offset values of Tile Length Size. Value that helps in the creation of triangles vertices.
	*/
	float LineEndPointValue; 

	/**
	* An array that holds triangle vertex buffer positions that will be given to a procedural mesh. 
	*/
	TArray<FVector> LineVertices;

	/**
	* An array that holds triangle index buffer positions that will be given to a procedural mesh. 
	*/
	TArray<int> LineTriangles;  

	/**
	* A function that creates a material instance used for the procedural mesh grid. 
	*/
	void CreateMaterialInstance(FLinearColor Color, float Opacity);

	/**
	* A function that calculates the start and end points of the procedural grid mesh lines. This is for both horizontal and vertical lines in the grid. 
	*/
	void CreateGridLinesGeometry();

	/**
	* A function taking the starting and ending points of a line and calculating the vertices with the index buffer of the triangles needed to render a square for the procedural mesh grid. A line's rendered thickness being a variable in the equation in calculating points.
	*/
	void CreateLine(FVector StartVector, FVector EndVector, float Thickness, TArray<FVector>& GridPointVertices, TArray<int>& TrianglePointCombination);

	/**
	* Function that takes components needed to render a procedural mesh to render a puzzle grid. Also sets the dynamic material of the mesh. 
	*/
	void CreateGridProceduralMesh();

};



