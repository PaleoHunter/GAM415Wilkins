// Copyright Epic Games, Inc. All Rights Reserved.

#include "GAM415FirstPersonPickUpComponent.h"

UGAM415FirstPersonPickUpComponent::UGAM415FirstPersonPickUpComponent()
{
	// Setup the Sphere Collision
	SphereRadius = 32.f;
}

void UGAM415FirstPersonPickUpComponent::BeginPlay()
{
	Super::BeginPlay();

	// Register our Overlap Event
	OnComponentBeginOverlap.AddDynamic(this, &UGAM415FirstPersonPickUpComponent::OnSphereBeginOverlap);
}

void UGAM415FirstPersonPickUpComponent::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Checking if it is a First Person Character overlapping
	AGAM415FirstPersonCharacter* Character = Cast<AGAM415FirstPersonCharacter>(OtherActor);
	if(Character != nullptr)
	{
		// Notify that the actor is being picked up
		OnPickUp.Broadcast(Character);

		// Unregister from the Overlap Event so it is no longer triggered
		OnComponentBeginOverlap.RemoveAll(this);
	}
}
