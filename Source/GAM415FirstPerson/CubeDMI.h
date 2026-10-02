// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "CubeDMI.generated.h"

class UNiagaraSystem;
class UStaticMeshComponent;
class UMaterialInterface;
class UMaterialInstanceDynamic;

UCLASS()
class GAM415FIRSTPERSON_API ACubeDMI : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACubeDMI();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Collision volume that triggers the color change */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* BoxComp;

	/** Visual mesh of the cube */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* CubeMesh;

	/** Base material used to create the Dynamic Material Instance */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
	UMaterialInterface* BaseMat;

	/** Runtime dynamic material instance */
	UPROPERTY(BlueprintReadOnly, Category = "Materials")
	UMaterialInstanceDynamic* DMIMat;

	/** Niagara system to spawn on overlap (optional) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	UNiagaraSystem* ColorP;

	/** Called when something overlaps the box component */
	UFUNCTION()
	void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);
};