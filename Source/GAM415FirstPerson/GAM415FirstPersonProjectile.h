// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GAM415FirstPersonProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UStaticMeshComponent;
class UMaterialInterface;
class UMaterialInstanceDynamic;
class UNiagaraSystem;

UCLASS(config = Game)
class AGAM415FirstPersonProjectile : public AActor
{
	GENERATED_BODY()

public:
	AGAM415FirstPersonProjectile();

protected:
	virtual void BeginPlay() override;

public:
	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

	/** Visual mesh of the projectile */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Projectile)
	UStaticMeshComponent* BallMesh;

	/** Base material used for the impact decal */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Impact")
	UMaterialInterface* BaseMat;

	/** Material used to create the projectile’s dynamic material instance */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	UMaterialInterface* ProjMat;

	/** Niagara system spawned on impact */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Impact")
	UNiagaraSystem* ColorP;

	/** Runtime random color for this projectile */
	UPROPERTY(BlueprintReadOnly, Category = "Projectile")
	FLinearColor RandColor;

	/** Runtime dynamic material instance */
	UPROPERTY(BlueprintReadOnly, Category = "Projectile")
	UMaterialInstanceDynamic* DMIMat;

	/** Called when the projectile hits something */
	UFUNCTION()
	void OnHit(
		UPrimitiveComponent* HitComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit
	);

	/** Returns CollisionComp subobject */
	FORCEINLINE USphereComponent* GetCollisionComp() const { return CollisionComp; }

	/** Returns ProjectileMovement subobject */
	FORCEINLINE UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }
};