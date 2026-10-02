// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PerlinProcterrain.generated.h"

class UProceduralMeshComponent;
class UMaterialInterface;

UCLASS()
class GAM415FIRSTPERSON_API APerlinProcterrain : public AActor
{
	GENERATED_BODY()

public:
	APerlinProcterrain();

	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0))
	int XSize = 0;

	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0))
	int YSize = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0))
	float ZMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0))
	float NoiseScale = 1.0f;

	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0.000001))
	float Scale = 100.0f;

	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0.000001))
	float UVScale = 1.0f;

	UPROPERTY(EditAnywhere)
	float radius = 200.0f;

	UPROPERTY(EditAnywhere)
	FVector Depth = FVector(0.0f, 0.0f, 20.0f);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* Mat;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Procedural Terrain")
	void AlterMesh(FVector impactPoint);

private:
	UProceduralMeshComponent* ProcMesh;
	TArray<FVector> Vertices;
	TArray<int> Triangles;
	TArray<FVector2D> UV0;
	TArray<FVector> Normals;
	TArray<FColor> UpVertexColors;

	int sectionID = 0;

	void CreateVertices();
	void CreateTriangles();
};