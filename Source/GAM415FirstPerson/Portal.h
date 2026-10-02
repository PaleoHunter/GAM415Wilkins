// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "Portal.generated.h"

class AGAM415FirstPersonCharacter;
class UStaticMeshComponent;
class UMaterialInterface;

UCLASS()
class GAM415FIRSTPERSON_API APortal : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APortal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Visual mesh of the portal */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* Mesh;

	/** Scene capture used to render the view through the other portal */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneCaptureComponent2D* SceneCapture;

	/** Arrow that marks the exact teleport destination */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UArrowComponent* RootArrow;

	/** Collision volume that triggers teleportation */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* BoxComp;

	/** The linked portal that this one connects to */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal")
	APortal* OtherPortal;

	/** Material applied to the portal mesh (usually uses the render target) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal")
	UMaterialInterface* Mat;

	/** Optional render target (can be set in the material instead) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal")
	UTextureRenderTarget2D* RenderTarget;

	/** Called when something overlaps the portal */
	UFUNCTION()
	void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	/** Resets the player's teleport cooldown flag */
	UFUNCTION()
	void SetBool(AGAM415FirstPersonCharacter* PlayerChar);

	/** Updates the scene capture to show the correct view through the other portal */
	UFUNCTION()
	void UpdatePortals();
};