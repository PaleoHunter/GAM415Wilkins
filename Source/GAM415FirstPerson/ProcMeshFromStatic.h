// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "ProcMeshFromStatic.generated.h"

class UStaticMeshComponent;

UCLASS()
class GAM415FIRSTPERSON_API AProcMeshFromStatic : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AProcMeshFromStatic();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostActorCreated() override;
	virtual void PostLoad() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Source static mesh that will be converted to a procedural mesh */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Mesh")
	UStaticMeshComponent* BaseMesh;

	/** Geometry data extracted from the static mesh */
	UPROPERTY()
	TArray<FVector> Vertices;

	UPROPERTY()
	TArray<int32> Triangles;

	UPROPERTY()
	TArray<FVector> Normals;

	UPROPERTY()
	TArray<FVector2D> UV0;

	UPROPERTY()
	TArray<FLinearColor> VertexColors;

	UPROPERTY()
	TArray<FColor> UpVertexColors;

	UPROPERTY()
	TArray<FProcMeshTangent> Tangents;

private:
	UPROPERTY()
	UProceduralMeshComponent* ProcMesh;

	void GetMeshData();
	void CreateMesh();
};