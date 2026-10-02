// Fill out your copyright notice in the Description page of Project Settings.

#include "PerlinProcterrain.h"
#include "ProceduralMeshComponent.h"
#include "KismetProceduralMeshLibrary.h"

APerlinProcterrain::APerlinProcterrain()
{
	PrimaryActorTick.bCanEverTick = false;

	ProcMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Procedural Mesh"));
	ProcMesh->SetupAttachment(GetRootComponent());
}

void APerlinProcterrain::BeginPlay()
{
	Super::BeginPlay();

	CreateVertices();
	CreateTriangles();
	ProcMesh->CreateMeshSection(sectionID, Vertices, Triangles, Normals, UV0, UpVertexColors, TArray<FProcMeshTangent>(), true);
	ProcMesh->SetMaterial(0, Mat);
}

void APerlinProcterrain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APerlinProcterrain::AlterMesh(FVector impactPoint)
{
	// Convert impact point to local space once
	const FVector LocalImpact = impactPoint - GetActorLocation();

	// Pre-compute squared radius (avoids expensive sqrt)
	const float RadiusSq = radius * radius;

	bool bAnyVertexAltered = false;

	for (int32 i = 0; i < Vertices.Num(); ++i)
	{
		if (FVector::DistSquared(Vertices[i], LocalImpact) < RadiusSq)
		{
			Vertices[i] -= Depth;
			bAnyVertexAltered = true;
		}
	}

	// Update mesh only once if something changed
	if (bAnyVertexAltered)
	{
		ProcMesh->UpdateMeshSection(sectionID, Vertices, Normals, UV0, UpVertexColors, TArray<FProcMeshTangent>());
	}
}

void APerlinProcterrain::CreateVertices()
{
	for (int X = 0; X <= XSize; X++)
	{
		for (int Y = 0; Y <= YSize; Y++)
		{
			float Z = FMath::PerlinNoise2D(FVector2D(X * NoiseScale + 0.1f, Y * NoiseScale + 0.1f)) * ZMultiplier;
			Vertices.Add(FVector(X * Scale, Y * Scale, Z));
			UV0.Add(FVector2D(X * UVScale, Y * UVScale));
		}
	}
}

void APerlinProcterrain::CreateTriangles()
{
	int Vertex = 0;

	for (int X = 0; X < XSize; X++)
	{
		for (int Y = 0; Y < YSize; Y++)
		{
			Triangles.Add(Vertex);
			Triangles.Add(Vertex + 1);
			Triangles.Add(Vertex + YSize + 1);
			Triangles.Add(Vertex + 1);
			Triangles.Add(Vertex + YSize + 2);
			Triangles.Add(Vertex + YSize + 1);
			Vertex++;
		}
	}
	Vertex++;
}