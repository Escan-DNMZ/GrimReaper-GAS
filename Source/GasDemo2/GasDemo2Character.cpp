// Copyright Epic Games, Inc. All Rights Reserved.

#include "GasDemo2Character.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Abilities/GasAbilitySystemComponent.h"
#include "Abilities/GasGameplayAbility.h"
#include "Actors/Enemy/WalkingDeads.h"
#include "AttributeSet/GasAttributeSet.h"
#include "GameFramework/SpringArmComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Hearing.h"
#include "Ai/Ai_Tags.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SInteractionComponent.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"


//////////////////////////////////////////////////////////////////////////
// AGasDemo2Character

AGasDemo2Character::AGasDemo2Character()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rate for input
	TurnRateGamepad = 50.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm


	AbilitySystemComponent = CreateDefaultSubobject<UGasAbilitySystemComponent>("ASC");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	
	AttributeSet = CreateDefaultSubobject<UGasAttributeSet>("AttributeSet");

	InteractionComp = CreateDefaultSubobject<USInteractionComponent>(TEXT("Interaction"));
	

	bReplicates = true;
	bAbilitiesInitialized = false;

	DeadTag = FGameplayTag::RequestGameplayTag("Gameplay.Status.IsDead");
	
	Setup_Stimulus();


	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AGasDemo2Character::Interaction()
{
	if (InteractionComp)
	{
		InteractionComp->PrimaryInteraction(this);
	}
}

void AGasDemo2Character::BeginPlay()
{
	Super::BeginPlay();
	
}

//////////////////////////////////////////////////////////////////////////
// Input

APathActor* AGasDemo2Character::get_patrol_path()
{
	return patrol_path;
}

void AGasDemo2Character::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	
	PlayerInputComponent->BindAction("FlyMode", IE_Pressed, this, &AGasDemo2Character::FlyModeOn);
	PlayerInputComponent->BindAction("FlyModeOff", IE_Released, this, &AGasDemo2Character::FlyModeOff);


	PlayerInputComponent->BindAction("Interaction", IE_Pressed, this, &AGasDemo2Character::Interaction);


	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &AGasDemo2Character::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &AGasDemo2Character::MoveRight);
	
	PlayerInputComponent->BindAxis("MoveUp", this, &AGasDemo2Character::MoveUp);
	

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Turn Right / Left Gamepad", this, &AGasDemo2Character::TurnAtRate);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Look Up / Down Gamepad", this, &AGasDemo2Character::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AGasDemo2Character::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AGasDemo2Character::TouchStopped);

	BindAbilityInput();
	


}

void AGasDemo2Character::Setup_Stimulus()
{
	Stimulus = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("stimulus"));
	Stimulus->RegisterForSense(TSubclassOf<UAISense_Sight>());
	Stimulus->RegisterWithPerceptionSystem();
}

void AGasDemo2Character::on_distract()
{
	if (DistractionSound)
	{
		FVector const loc = GetActorLocation();
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), DistractionSound, loc);
		UAISense_Hearing::ReportNoiseEvent(GetWorld(), loc, 1.0f, this, 0.0f, Tags::noise_tag);
	}
}

// float MovementSpeed = AttributeSet->GetMovementSpeed();
// if(MovementSpeed > 0.f){
// 	GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;
// }




void AGasDemo2Character::AttackHandled_Implementation()
{
	TArray<AActor*> GasEnemy;
	for (FHitResult Hit: WeaponHits)
	{
		GasEnemy.Add(Hit.GetActor());
	}
	
	if (GasEnemy.Num() > 0 )
	{
		for (AActor* Enemy:GasEnemy)
		{
			if (Cast<IAbilitySystemInterface>(Enemy)->GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Gameplay.Status.IsDead")))
			{
				continue;
			}
			
			if (Cast<AWalkingDeads>(Enemy))
			{
				const FGameplayTag Tag = FGameplayTag::RequestGameplayTag("Attack.Attack");
				FGameplayEventData Payload = FGameplayEventData();
				Payload.Instigator = GetInstigator();
				Payload.Target = Enemy;
				Payload.TargetData = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActor(Enemy);
				UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetInstigator(),Tag,Payload);
			}
		}
	}
	else
	{
		FGameplayTag Tag = FGameplayTag::RequestGameplayTag("Attack.Attack");
		FGameplayEventData Payload = FGameplayEventData();
		Payload.Instigator = GetInstigator();
		Payload.TargetData = FGameplayAbilityTargetDataHandle();
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetInstigator(),Tag,Payload);
	}
}

UAbilitySystemComponent* AGasDemo2Character::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
	
}

void AGasDemo2Character::MoveUp(float Axis)
{
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Z);
	AddMovementInput(Direction, Axis);
}
void AGasDemo2Character::FlyModeOn_Implementation()
{
	if (GetCharacterMovement()->CanEverFly())
	{
		GetCharacterMovement()->SetMovementMode(MOVE_Flying, 0);
	}
	
	
	
}
void AGasDemo2Character::FlyModeOff_Implementation()
{
	
	if (GetCharacterMovement()->CanEverFly())
	{
		GetCharacterMovement()->SetMovementMode(MOVE_Walking, 0);
	}
	
	
}
bool AGasDemo2Character::IsAlive(){
	return GetHealth() > 0;
}



void AGasDemo2Character::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void AGasDemo2Character::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}



void AGasDemo2Character::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void AGasDemo2Character::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}


void AGasDemo2Character::InitializeAttributes()
{
	check(AbilitySystemComponent)
	//Apply Passives
	for (TSubclassOf<UGameplayEffect> PassiveEffect : InitAttributeEffect)
	{

		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(PassiveEffect, 1, EffectContext);


		if (SpecHandle.IsValid())
		{
			FActiveGameplayEffectHandle GeHandle =
				AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
	
}

void AGasDemo2Character::GiveAbility()
{
	check(AbilitySystemComponent)
	if (HasAuthority() && AbilitySystemComponent)
	{
		// Foreach ability in DefaultAbilities, grant the ability
		for (TSubclassOf<UGasGameplayAbility>& StartupAbility : DefaultAbilities)
		{
	
			AbilitySystemComponent->GiveAbility(
			FGameplayAbilitySpec(StartupAbility, 1, static_cast<int32>(StartupAbility.GetDefaultObject()->AbilityInputID), this));

		}
	}
}

void AGasDemo2Character::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	//Client Only
	AbilitySystemComponent->InitAbilityActorInfo(this,this);
	BindAbilityInput();
	
}

void AGasDemo2Character::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Server Only

	AbilitySystemComponent->InitAbilityActorInfo(this,this);
	InitializeAttributes();
	GiveAbility();
}


void AGasDemo2Character::BindAbilityInput()
{
	if (AbilitySystemComponent && InputComponent)
	{
		const FGameplayAbilityInputBinds Binds("Confirm","Cancel","EGasAbilityInputId", static_cast<int32>(EGasAbilityInputId::Confirm), static_cast<int32>(EGasAbilityInputId::Cancel));
		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent,Binds);
	}
}

float AGasDemo2Character::GetHealth()
{
	if (AttributeSet)
	{
		return AttributeSet->GetHealth();
	}
	return 0.1f;
}

float AGasDemo2Character::GetMana()
{
	if (AttributeSet)
	{
		return AttributeSet->GetMana();
	}
	return 0.1f;
}

void AGasDemo2Character::MoveForward(float Value)
{
	
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AGasDemo2Character::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AGasDemo2Character::HandleDamage(float DamageAmount, const FHitResult& HitInfo,
	const FGameplayTagContainer& DamageTags, AGasDemo2Character* InstigatorCharacter, AActor* DamageCauser)
{
	OnDamaged(DamageAmount,HitInfo,DamageTags,InstigatorCharacter,DamageCauser);
}
void AGasDemo2Character::HandleHealthChanged(float DeltaValue, const FGameplayTagContainer& EventTags)
{
	OnHealthChanged(DeltaValue,EventTags);
	if (GetHealth() <= 0)
	{
		AbilitySystemComponent->AddLooseGameplayTag(DeadTag);
	}
}

void AGasDemo2Character::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGasDemo2Character, AttackAnims);
	
	
}

