// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/SDoor.h"
#include "Components/BoxComponent.h"

// Sets default values
ASDoor::ASDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	Door->SetupAttachment(RootComponent);

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	BoxCollision->SetupAttachment(Door);

	
}

// Called when the game starts or when spawned
void ASDoor::BeginPlay()
{
	Super::BeginPlay();
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ASDoor::DoorOverlaped);
}

// Called every frame
void ASDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASDoor::Interact_Implementation(APawn* InstigatorPawn)
{
	//In Blueprint
}

void ASDoor::DoorOverlaped( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

