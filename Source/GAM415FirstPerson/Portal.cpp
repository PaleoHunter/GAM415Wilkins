// Fill out your copyright notice in the Description page of Project Settings.

#include "Portal.h"
#include "GAM415FirstPersonCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
APortal::APortal()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Comp"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SceneCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("Capture"));
	RootArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Root Arrow"));

	RootComponent = BoxComp;
	Mesh->SetupAttachment(BoxComp);
	SceneCapture->SetupAttachment(Mesh);
	RootArrow->SetupAttachment(RootComponent);

	// Portal mesh should not block anything
	Mesh->SetCollisionResponseToAllChannels(ECR_Ignore);
}

// Called when the game starts or when spawned
void APortal::BeginPlay()
{
	Super::BeginPlay();

	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &APortal::OnOverlapBegin);

	// Hide this portal's mesh from its own scene capture
	Mesh->SetHiddenInSceneCapture(true);

	if (Mat)
	{
		Mesh->SetMaterial(0, Mat);
	}
}

// Called every frame
void APortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdatePortals();
}

void APortal::OnOverlapBegin(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	AGAM415FirstPersonCharacter* PlayerChar = Cast<AGAM415FirstPersonCharacter>(OtherActor);
	if (!PlayerChar || !OtherPortal)
	{
		return;
	}

	// Prevent rapid re-teleporting
	if (PlayerChar->bIsTeleporting)
	{
		return;
	}

	PlayerChar->bIsTeleporting = true;

	// Teleport the player to the other portal's arrow location
	const FVector TargetLocation = OtherPortal->RootArrow->GetComponentLocation();
	PlayerChar->SetActorLocation(TargetLocation);

	// Reset the teleport flag after a short delay
	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUFunction(this, FName("SetBool"), PlayerChar);
	GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, 1.0f, false);
}

void APortal::SetBool(AGAM415FirstPersonCharacter* PlayerChar)
{
	if (PlayerChar)
	{
		PlayerChar->bIsTeleporting = false;
	}
}

void APortal::UpdatePortals()
{
	if (!OtherPortal || !SceneCapture)
	{
		return;
	}

	// Calculate the relative offset between the two portals
	const FVector LocationOffset = GetActorLocation() - OtherPortal->GetActorLocation();

	// Get the player's camera transform
	APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	if (!CameraManager)
	{
		return;
	}

	const FVector CamLocation = CameraManager->GetTransformComponent()->GetComponentLocation();
	const FRotator CamRotation = CameraManager->GetTransformComponent()->GetComponentRotation();

	// Position the scene capture so it shows the correct view through the other portal
	const FVector CombinedLocation = CamLocation + LocationOffset;
	SceneCapture->SetWorldLocationAndRotation(CombinedLocation, CamRotation);
}