// Fill out your copyright notice in the Description page of Project Settings.

#include "CubeDMI.h"
#include "GAM415FirstPersonCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

// Sets default values
ACubeDMI::ACubeDMI()
{
	PrimaryActorTick.bCanEverTick = false;		// Not used, so disable for a tiny performance win

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube Mesh"));

	RootComponent = BoxComp;
	CubeMesh->SetupAttachment(BoxComp);
}

// Called when the game starts or when spawned
void ACubeDMI::BeginPlay()
{
	Super::BeginPlay();

	// Bind overlap event
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ACubeDMI::OnOverlapBegin);

	// Create dynamic material instance if a base material is assigned
	if (BaseMat)
	{
		DMIMat = UMaterialInstanceDynamic::Create(BaseMat, this);

		if (CubeMesh && DMIMat)
		{
			CubeMesh->SetMaterial(0, DMIMat);
		}
	}
}

// Called every frame
void ACubeDMI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACubeDMI::OnOverlapBegin(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	// Early out if the overlapping actor is not the player
	AGAM415FirstPersonCharacter* Player = Cast<AGAM415FirstPersonCharacter>(OtherActor);
	if (!Player || !DMIMat)
	{
		return;
	}

	// Generate a random color
	const float RandR = UKismetMathLibrary::RandomFloatInRange(0.f, 1.f);
	const float RandG = UKismetMathLibrary::RandomFloatInRange(0.f, 1.f);
	const float RandB = UKismetMathLibrary::RandomFloatInRange(0.f, 1.f);

	const FLinearColor RandColor(RandR, RandG, RandB, 1.f);

	// Update material parameters
	DMIMat->SetVectorParameterValue(TEXT("Color"), RandColor);
	DMIMat->SetScalarParameterValue(TEXT("Darkness"), RandR);

	// Spawn Niagara system if assigned
	if (ColorP && OtherComp)
	{
		UNiagaraComponent* ParticleComp = UNiagaraFunctionLibrary::SpawnSystemAttached(
			ColorP,
			OtherComp,
			NAME_None,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::KeepRelativeOffset,
			true
		);

		if (ParticleComp)
		{
			ParticleComp->SetNiagaraVariableLinearColor(TEXT("RandColor"), RandColor);
		}
	}
}