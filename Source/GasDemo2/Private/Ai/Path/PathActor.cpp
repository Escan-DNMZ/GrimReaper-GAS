// Fill out your copyright notice in the Description page of Project Settings.


#include "Ai/Path/PathActor.h"

// Sets default values
APathActor::APathActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APathActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APathActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector APathActor::get_patrol_Index(const int index) const
{
	return patrol_point[index];
}

int APathActor::num() const
{
	return patrol_point.Num();
}

