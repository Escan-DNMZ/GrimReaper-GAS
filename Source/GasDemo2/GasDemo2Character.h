// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "Sound/SoundBase.h"
#include "GasDemo2Character.generated.h"

class UGasAttributeSet;

UCLASS(config=Game)
class AGasDemo2Character : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	AGasDemo2Character();

	
		class APathActor* get_patrol_path();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ai")
	class APathActor* patrol_path;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Input)
	float TurnRateGamepad;

	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category = "GAS")
	TArray<TSubclassOf<class UGasGameplayAbility>> DefaultAbilities;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	class USInteractionComponent* InteractionComp;

	
	void Interaction();

public:
	

	virtual void BeginPlay() override;

	UPROPERTY(Replicated,EditAnywhere,BlueprintReadWrite)
	TArray<class UAnimMontage*> AttackAnims;

	UFUNCTION(Server,Reliable,BlueprintCallable,Category="Attack")
    void AttackHandled();
	
	UPROPERTY(Replicated,EditAnywhere,BlueprintReadWrite)
	TArray<FHitResult> WeaponHits;
	
	// ASC

	UPROPERTY()
	uint8 bAbilitiesInitialized:1;

	UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="GAS")
	TObjectPtr<class UGasAbilitySystemComponent> AbilitySystemComponent;

	//AttributeSet
	friend UGasAttributeSet;
	
	UPROPERTY(BlueprintReadOnly,Category="GAS")
	TObjectPtr<UGasAttributeSet> AttributeSet;

	//GE
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
		TArray<TSubclassOf<class UGameplayEffect>> InitAttributeEffect;

	UFUNCTION()
	void InitializeAttributes();

	UFUNCTION()
	void GiveAbility();


	virtual void OnRep_PlayerState() override;


	virtual void PossessedBy(AController* NewController) override;


	void BindAbilityInput();

	UFUNCTION(BlueprintCallable)
	virtual float GetHealth();
	
	UFUNCTION(BlueprintCallable)
		virtual float GetMana();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	UFUNCTION(BlueprintImplementableEvent)
	void OnDamaged(float DamageAmount,const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags,
		AGasDemo2Character* InstigatorCharacter, AActor* DamageCauser);

	virtual void HandleDamage(float DamageAmount,const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags,
		AGasDemo2Character* InstigatorCharacter, AActor* DamageCauser);
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnHealthChanged(float DeltaValue,const struct FGameplayTagContainer& EventTags);

	virtual void HandleHealthChanged(float DeltaValue,const struct FGameplayTagContainer& EventTags);

	FGameplayTag DeadTag;
	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	void MoveUp(float Axis);

	UFUNCTION(Server, Reliable)
	void FlyModeOn();
	
	UFUNCTION(Server, Reliable)
	void FlyModeOff();

	UFUNCTION(BlueprintCallable, Category = "GAS | Character")
		virtual bool  IsAlive();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface
private:
	class UAIPerceptionStimuliSourceComponent* Stimulus;

	void Setup_Stimulus();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Sound", meta = (AllowPrivateAccess = "true"))
		USoundBase* DistractionSound;

	UFUNCTION(BlueprintCallable, Category = "Mana")
	void on_distract();
public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

