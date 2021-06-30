// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid.h"

#include "Chaos/AABB.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AGrid::AGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Procedural Mesh"));
	RootComponent = ProceduralMesh;

}

void AGrid::GenerateProceduralMesh()
{
	CreateGridLinesGeometry();

	CreateGridProceduralMesh();
}

// Called when the game starts or when spawned
void AGrid::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGrid::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	//set Dynamic Material Instance to be used for grid lines.
	CreateMaterialInstance(LineColor, LineOpacity);

	GenerateProceduralMesh();
}

void AGrid::CreateGridLinesGeometry()
{
	LineVertices.Empty();
	LineTriangles.Empty();

	//horizontal lines
	for(int i = 0; i <= RowsNum; i++)
	{
		LineStartPointValue = TileSideLength * i;
		LineEndPointValue = ColsNum * TileSideLength;

		FVector StartVectorHorizontal = FVector(LineStartPointValue, 0.0f, 0.0f);
		FVector EndVectorHorizontal = FVector(LineStartPointValue, LineEndPointValue, 0.0f);
		
		CreateLine(StartVectorHorizontal, EndVectorHorizontal, LineThickness, LineVertices, LineTriangles);
	}

	//vertical lines
	for(int i = 0; i <= ColsNum; i++)
	{
		LineStartPointValue = TileSideLength * i;
		LineEndPointValue = RowsNum * TileSideLength;

		FVector StartVectorVertical = FVector(0.0f, LineStartPointValue, 0.0f);
		FVector EndVectorVertical = FVector(LineEndPointValue, LineStartPointValue, 0.0f);
		
		CreateLine(StartVectorVertical, EndVectorVertical, LineThickness, LineVertices, LineTriangles);
	}
}

void AGrid::CreateLine(FVector StartVector, FVector EndVector, float Thickness, TArray<FVector>& GridPointVertices, TArray<int>& TrianglePointCombination)
{
	//how thick we want a line to be - half since we're rending line thickness for neighbors as well
	float HalfLineThickness = Thickness / 2.0f;

	//get direction of sides that are orthogonal to end-start vector, going through the start point, using cross product
	FVector Length = UKismetMathLibrary::Subtract_VectorVector(EndVector, StartVector);
	Length = UKismetMathLibrary::Normal(Length, 0.0001);
	FVector OrthogonalDirection = FVector::CrossProduct(Length, FVector(0.0f, 0.0f, 1.0f));

	//creating triangle index buffer (call order) for the creation of procedural triangles - the square is made of two triangles - will be given to procedural mesh

	TArray<int> TriangleIndexBufferValues;
	
	//first triangle
	TriangleIndexBufferValues.Add(2 + GridPointVertices.Num());
	TriangleIndexBufferValues.Add(1 + GridPointVertices.Num());
	TriangleIndexBufferValues.Add(0 + GridPointVertices.Num());

	//second triangle
	TriangleIndexBufferValues.Add(2 + GridPointVertices.Num());
	TriangleIndexBufferValues.Add(3 + GridPointVertices.Num());
	TriangleIndexBufferValues.Add(1 + GridPointVertices.Num());

	//append to array to store total of all grid indices 
	TrianglePointCombination.Append(TriangleIndexBufferValues);

	//creating local space coordinates for each corner of the square - a vertex buffer of all vertex positions of triangles - will be given to procedural mesh

	TArray<FVector> SquareVertices;

	//vertex 0
	SquareVertices.Add(StartVector + OrthogonalDirection * HalfLineThickness);

	//vertex 1
	SquareVertices.Add(EndVector + OrthogonalDirection * HalfLineThickness);

	//vertex 2
	SquareVertices.Add(StartVector - OrthogonalDirection * HalfLineThickness);

	//vertex 3
	SquareVertices.Add(EndVector - OrthogonalDirection * HalfLineThickness);

	//append to array to store total of all grid vertices
	GridPointVertices.Append(SquareVertices);
	
}

void AGrid::CreateGridProceduralMesh()
{
	int32 SectionIndex = 0;
	TArray<FVector> Normals = TArray<FVector>();
	TArray<FVector2D> UV0 = TArray<FVector2D>();
	TArray<FLinearColor> VertexColors = TArray<FLinearColor>();
	TArray<FProcMeshTangent> ProceduralMeshTangents = TArray<FProcMeshTangent>();
	bool bCreateCollision = false;
	
	ProceduralMesh->CreateMeshSection_LinearColor(SectionIndex, LineVertices, LineTriangles, Normals, UV0, VertexColors, ProceduralMeshTangents, bCreateCollision);
	ProceduralMesh->SetMaterial(0, LinesMaterial);
}

FVector AGrid::GetGridPosition(int Row, int Column)
{
	float GridPosition_X = GetActorLocation().X;
	float GridPosition_Y = GetActorLocation().Y;

	float HalfSideLength = TileSideLength/2.0f;

	float XCoordinate = (Row * TileSideLength + GridPosition_X) - HalfSideLength;
	float YCoordinate = (Column * TileSideLength + GridPosition_Y) - HalfSideLength;

	return FVector(XCoordinate, YCoordinate, 1.0f);
}

float AGrid::GetRowHalfWidth()
{
	FVector CenterPosition = GetCenterPosition();

	float Width = CenterPosition.Y + TileSideLength * RowsNum/2.0f;
	
	return  Width;
}

FVector AGrid::GetCenterPosition()
{
	int32 CenterRow = UKismetMathLibrary::FCeil(RowsNum / 2.0f);
	int32 CenterCol = UKismetMathLibrary::FCeil(ColsNum / 2.0f);

	return GetGridPosition(CenterRow, CenterCol);
}

void AGrid::CreateMaterialInstance(FLinearColor Color, float Opacity)
{

	if(BaseMaterial && BaseMaterial->GetMaterial())
	{
		LinesMaterial = UMaterialInstanceDynamic::Create(BaseMaterial->GetMaterial(), this);

		//set values of material instance from color and opacity values from LineColor and LineOpacity, key names from original material
		LinesMaterial->SetScalarParameterValue(TEXT("Opacity"), Opacity);
		LinesMaterial->SetVectorParameterValue(TEXT("Color"), Color);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("WARNING: Grid.cpp: Material not found. Check for Base material in Blueprint."))
	}
		
}

// Called every frame
void AGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

