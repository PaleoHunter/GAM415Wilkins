// Fill out your copyright notice in the Description page of Project Settings.

#include "ProcMeshFromStatic.h"
#include "KismetProceduralMeshLibrary.h"
#include "Components/StaticMeshComponent.h"
#include "ProceduralMeshComponent.h"

// Sets default values
AProcMeshFromStatic::AProcMeshFromStatic()
{
	PrimaryActorTick.bCanEverTick = false;

	ProcMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Proc Mesh"));
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));

	RootComponent = ProcMesh;
	BaseMesh->SetupAttachment(ProcMesh);
}

// Called when the game starts or when spawned
void AProcMeshFromStatic::BeginPlay()
{
	Super::BeginPlay();
}

void AProcMeshFromStatic::PostActorCreated()
{
	Super::PostActorCreated();
	GetMeshData();
}

void AProcMeshFromStatic::PostLoad()
{
	Super::PostLoad();
	GetMeshData();
}

void AProcMeshFromStatic::GetMeshData()
{
	if (!BaseMesh)
	{
		return;
	}

	UStaticMesh* StaticMesh = BaseMesh->GetStaticMesh();
	if (!StaticMesh)
	{
		return;
	}

	// Extract geometry data from the static mesh
	UKismetProceduralMeshLibrary::GetSectionFromStaticMesh(
		StaticMesh,
		0,					// LOD Index
		0,					// Section Index
		Vertices,
		Triangles,
		Normals,
		UV0,
		Tangents
	);

	// Create (or recreate) the procedural mesh section
	CreateMesh();
}

void AProcMeshFromStatic::CreateMesh()
{
	if (!ProcMesh)
	{
		return;
	}

	ProcMesh->CreateMeshSection(
		0,					// Section Index
		Vertices,
		Triangles,
		Normals,
		UV0,
		UpVertexColors,
		Tangents,
		true				// Create collision
	);
}

// Called every frame
void AProcMeshFromStatic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}