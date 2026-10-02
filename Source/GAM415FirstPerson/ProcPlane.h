// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProcPlane.generated.h"

class UProceduralMeshComponent;
class UMaterialInterface;

UCLASS()
class GAM415FIRSTPERSON_API AProcPlane : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AProcPlane();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostActorCreated() override;
	virtual void PostLoad() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Vertex positions for the plane */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Mesh")
	TArray<FVector> Vertices;

	/** Triangle indices */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Mesh")
	TArray<int32> Triangles;

	/** UV coordinates */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Mesh")
	TArray<FVector2D> UV0;

	/** Material applied to the procedural mesh */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Mesh")
	UMaterialInterface* PlaneMat;

	/** Builds (or rebuilds) the procedural mesh section */
	UFUNCTION(BlueprintCallable, Category = "Procedural Mesh")
	void CreateMesh();

private:
	UPROPERTY()
	UProceduralMeshComponent* ProcMesh;
};