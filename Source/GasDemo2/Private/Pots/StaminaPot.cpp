// Fill out your copyright notice in the Description page of Project Settings.


#include "Pots/StaminaPot.h"
#include "Components/BoxComponent.h"
#include <Actors/Enemy/WalkingDeads.h>
#include <Abilities/GasAbilitySystemComponent.h>

// Sets default values
AStaminaPot::AStaminaPot()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	RootComponent = CollisionBox;

	
}

// Called when the game starts or when spawned
void AStaminaPot::BeginPlay()
{
	Super::BeginPlay();
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AStaminaPot::OnOverlapBegin);
}

// Called every frame
void AStaminaPot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void AStaminaPot::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AWalkingDeads* Character = Cast<AWalkingDeads>(OtherActor);

	if (Character)
	{
		UAbilitySystemComponent* AbilityComp = Character->GetAbilitySystemComponent();
		if (AbilityComp)
		{
			FGameplayEffectContextHandle EffectContext = AbilityComp->MakeEffectContext();

			FGameplayEffectSpecHandle SpecHandle = AbilityComp->MakeOutgoingSpec(Effect, 0, EffectContext);

			if (SpecHandle.IsValid())
			{
				AbilityComp->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			}
		}
		Destroy();
	}
	
	
}

