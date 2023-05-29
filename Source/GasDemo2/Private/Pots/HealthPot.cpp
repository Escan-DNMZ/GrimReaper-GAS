// Fill out your copyright notice in the Description page of Project Settings.


#include "Pots/HealthPot.h"
#include "Components/BoxComponent.h"
#include "Actors/Enemy/WalkingDeads.h"
#include <Abilities/GasAbilitySystemComponent.h>

// Sets default values
AHealthPot::AHealthPot()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	RootComponent = CollisionBox;
	
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AHealthPot::OnOverlapBegin);
}

// Called when the game starts or when spawned
void AHealthPot::BeginPlay()
{
	Super::BeginPlay();
	
}

void AHealthPot::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AWalkingDeads* Character = Cast<AWalkingDeads>(OtherActor);

	if (Character)
	{
		UAbilitySystemComponent* Ability = Character->GetAbilitySystemComponent();
		if (Ability)
		{
			FGameplayEffectContextHandle EffectContext = Ability->MakeEffectContext();
			
			FGameplayEffectSpecHandle SpecHandle = Ability->MakeOutgoingSpec(Effect, 0, EffectContext);
			
			if (SpecHandle.IsValid())
			{
				Ability->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			}
		}
		Destroy();
	}
}

// Called every frame
void AHealthPot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

