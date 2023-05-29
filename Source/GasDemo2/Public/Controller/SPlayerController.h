// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPawnChanged, APawn*, NewPawn);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStateChanged, APlayerState*, NewPlayerState);
 

UCLASS()
class GASDEMO2_API ASPlayerController : public APlayerController
{
	GENERATED_BODY()

public:


	/*UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<UUserWidget> HudClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TObjectPtr<UUserWidget> HudClassInstance;

	UFUNCTION(BlueprintCallable)
	void Hud();*/

	UPROPERTY(BlueprintAssignable)
		FOnPawnChanged OnPawnChanged;


	UPROPERTY(BlueprintAssignable)
		FOnPlayerStateChanged OnPlayerStateReceived;

	virtual void SetPawn(APawn* InPawn) override;


	virtual void BeginPlayingState() override;

	UFUNCTION(BlueprintImplementableEvent)
		void BlueprintBeginPlayingState();

	virtual void OnRep_PlayerState() override;
	
};
