// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SInteractionComponent.h"
#include <Kismet/GameplayStatics.h>
#include "Interfaces/SInteractInterface.h"

// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USInteractionComponent::PrimaryInteraction(APawn* Instigator)
{
	FCollisionObjectQueryParams QueryParams;
	QueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	AActor* MyOwner = GetOwner();

	FVector2D ScreenSize;

	GetWorld()->GetGameViewport()->GetViewportSize(ScreenSize);

	FVector2D CrosshairLocation(ScreenSize.X / 2.f, ScreenSize.Y / 2.f);

	FVector CrosshairWorldLocation;
	FVector CrosshairWorldDirection;

	//2D vekt�r bilgisini 3D vekt�re �evirir b�ylelikle y�n bilgisi i�in bir de�i�kene daha ihtiyac�m�z var.
	UGameplayStatics::DeprojectScreenToWorld(UGameplayStatics::GetPlayerController(this, 0),
		CrosshairLocation, CrosshairWorldLocation, CrosshairWorldDirection);

	FVector CrosshairEnd = CrosshairWorldLocation + (CrosshairWorldDirection * 1000);

	FCollisionShape Shape;
	Shape.SetSphere(50.f);

	TArray<FHitResult> hits;

	GetWorld()->SweepMultiByObjectType(hits, CrosshairWorldLocation, CrosshairEnd, FQuat::Identity, QueryParams, Shape);

	for (FHitResult hit:hits)
	{
		AActor* HitActor = hit.GetActor();
		if (HitActor)
		{
			if (HitActor->Implements<USInteractInterface>())
			{
				APawn* MyPawn = Cast<APawn>(GetOwner());
				ISInteractInterface* Interface = Cast<ISInteractInterface>(HitActor);
				Interface->Execute_Interact(HitActor, MyPawn);
				DrawDebugLine(GetWorld(), CrosshairWorldLocation, CrosshairEnd, FColor::Green, true, 1.0f);
			}
		}
	}
	

}

