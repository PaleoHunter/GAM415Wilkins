// Copyright Epic Games, Inc. All Rights Reserved.

#include "GAM415FirstPersonProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SphereComponent.h"
#include "Components/DecalComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "PerlinProcterrain.h"

AGAM415FirstPersonProjectile::AGAM415FirstPersonProjectile()
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
	CollisionComp->OnComponentHit.AddDynamic(this, &AGAM415FirstPersonProjectile::OnHit);

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Visual mesh
	BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ball Mesh"));

	// Set as root component
	RootComponent = CollisionComp;          // ? fixed (was = = )
	BallMesh->SetupAttachment(CollisionComp);

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

void AGAM415FirstPersonProjectile::BeginPlay()
{
	Super::BeginPlay();

	// Generate a random color for this projectile
	RandColor = FLinearColor(
		UKismetMathLibrary::RandomFloatInRange(0.f, 1.f),
		UKismetMathLibrary::RandomFloatInRange(0.f, 1.f),
		UKismetMathLibrary::RandomFloatInRange(0.f, 1.f),
		1.f
	);

	// Create and apply dynamic material
	if (ProjMat)
	{
		DMIMat = UMaterialInstanceDynamic::Create(ProjMat, this);
		if (BallMesh && DMIMat)
		{
			BallMesh->SetMaterial(0, DMIMat);
			DMIMat->SetVectorParameterValue(TEXT("ProjColor"), RandColor);
		}
	}
}

void AGAM415FirstPersonProjectile::OnHit(
	UPrimitiveComponent* HitComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	FVector NormalImpulse,
	const FHitResult& Hit)
{
	// Apply physics impulse if we hit a simulating component
	if (OtherActor && OtherActor != this && OtherComp && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
	}

	if (!OtherActor)
	{
		return;
	}

	// Spawn impact particles
	if (ColorP)
	{
		UNiagaraComponent* ParticleComp = UNiagaraFunctionLibrary::SpawnSystemAttached(
			ColorP,
			HitComp,
			NAME_None,
			FVector(-20.f, 0.f, 0.f),
			FRotator::ZeroRotator,
			EAttachLocation::KeepRelativeOffset,
			true
		);

		if (ParticleComp)
		{
			ParticleComp->SetNiagaraVariableLinearColor(TEXT("RandomColor"), RandColor);
		}

		// Hide the mesh and disable collision after impact
		if (BallMesh)
		{
			BallMesh->DestroyComponent();
		}
		CollisionComp->BodyInstance.SetCollisionProfileName(TEXT("NoCollision"));
	}

	// Spawn a colored decal at the impact point
	if (BaseMat)
	{
		const float FrameNum = UKismetMathLibrary::RandomFloatInRange(0.f, 3.f);
		const float DecalSize = UKismetMathLibrary::RandomFloatInRange(20.f, 40.f);

		UDecalComponent* Decal = UGameplayStatics::SpawnDecalAtLocation(
			GetWorld(),
			BaseMat,
			FVector(DecalSize),
			Hit.Location,
			Hit.Normal.Rotation(),
			0.f
		);

		if (Decal)
		{
			UMaterialInstanceDynamic* MatInstance = Decal->CreateDynamicMaterialInstance();
			if (MatInstance)
			{
				MatInstance->SetVectorParameterValue(TEXT("Color"), RandColor);
				MatInstance->SetScalarParameterValue(TEXT("Frame"), FrameNum);
			}
		}
	}

	// Deform procedural terrain if we hit one
	if (APerlinProcterrain* ProcTerrain = Cast<APerlinProcterrain>(OtherActor))
	{
		ProcTerrain->AlterMesh(Hit.ImpactPoint);
	}
}