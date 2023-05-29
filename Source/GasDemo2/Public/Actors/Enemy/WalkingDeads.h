// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GasDemo2/GasDemo2Character.h"
#include "WalkingDeads.generated.h"

/**
 * 
 */
UCLASS()
class GASDEMO2_API AWalkingDeads : public AGasDemo2Character
{
	GENERATED_BODY()
public:
	AWalkingDeads();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void StartSprint();

	UFUNCTION(BlueprintCallable, Category = "Attack")
		void Stun();


};
