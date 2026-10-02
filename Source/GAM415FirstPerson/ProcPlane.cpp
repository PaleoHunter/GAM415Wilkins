// Fill out your copyright notice in the Description page of Project Settings.

#include "ProcPlane.h"
#include "ProceduralMeshComponent.h"

// Sets default values
AProcPlane::AProcPlane()
{
	PrimaryActorTick.bCanEverTick = false;

	ProcMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Proc Mesh"));
	RootComponent = ProcMesh;		// Make the procedural mesh the root
}

// Called when the game starts or when spawned
void AProcPlane::BeginPlay()
{
	Super::BeginPlay();
}

void AProcPlane::PostActorCreated()
{
	Super::PostActorCreated();
	CreateMesh();
}

void AProcPlane::PostLoad()
{
	Super::PostLoad();
	CreateMesh();
}

// Called every frame
void AProcPlane::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProcPlane::CreateMesh()
{
	if (!ProcMesh)
	{
		return;
	}

	// Create the mesh section (empty normals / vertex colors / tangents for a simple plane)
	ProcMesh->CreateMeshSection(
		0,							// Section Index
		Vertices,
		Triangles,
		TArray<FVector>(),			// Normals
		UV0,
		TArray<FColor>(),			// Vertex Colors
		TArray<FProcMeshTangent>(),	// Tangents
		true						// Create collision
	);

	// Apply material if one is assigned
	if (PlaneMat)
	{
		ProcMesh->SetMaterial(0, PlaneMat);
	}
}