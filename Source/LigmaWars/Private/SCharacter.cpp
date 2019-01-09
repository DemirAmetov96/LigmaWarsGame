
// Fill out your copyright notice in the Description page of Project Settings.

#include "SCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "LigmaWars.h"
#include "SHealthComponent.h"
#include "SWeapon.h"
#include "SGameMode.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;

	GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_WEAPON, ECR_Ignore);

	HealthComp = CreateDefaultSubobject<USHealthComponent>(TEXT("HealthComp"));

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	ZoomedFOV = 65.0f;
	ZoomInterpSpeed = 20.0f;

	WeaponAttachSocketName = "WeaponSocket";
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	DefaulFOV = CameraComp->FieldOfView;
	HealthComp->OnHealthChanged.AddDynamic(this, &ASCharacter::OnHealthChanged);
	PistolTemp = 80;
	RifleTemp = 0;
	BazookaTemp = 0;

	if (Role == ROLE_Authority)
	{
		//Spawn a default weapon
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		CurrentWeapon = GetWorld()->SpawnActor<ASWeapon>(StarterWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
		if (CurrentWeapon)
		{
			UE_LOG(LogTemp, Warning, TEXT("The SWeapon is Spawned"));
			CurrentWeapon->SetOwner(this);
			CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponAttachSocketName);
		}
	}
}

void ASCharacter::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector() * Value);
}

void ASCharacter::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector() * Value);
}

void ASCharacter::BeginCrouch()
{
	Crouch();
}

void ASCharacter::EndCrouch()
{
	UnCrouch();
}

void ASCharacter::BeginZoom()
{
	bWantsToZoom = true;
}

void ASCharacter::EndZoom()
{
	bWantsToZoom = false;
}

void ASCharacter::StartFire()
{
	UE_LOG(LogTemp, Warning, TEXT("The SCharacter is calling the fire function"));
	if (CurrentWeapon)
	{
		CurrentWeapon->StartFire();
	}
}

void ASCharacter::StopFire()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->StopFire();
	}
}



void ASCharacter::ChangeToPistol()
{
	if (Role < ROLE_Authority)
	{
		ServerChangeToPistol();
	}

	if (Role == ROLE_Authority)
	{

		SaveCurrentAmmo();

		//Spawn a default weapon
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		CurrentWeapon->SetActorHiddenInGame(true);
		CurrentWeapon = GetWorld()->SpawnActor<ASWeapon>(WeaponPistol, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
		if (CurrentWeapon)
		{
			UE_LOG(LogTemp, Warning, TEXT("The SWeapon is Spawned"));
			CurrentWeapon->SetOwner(this);
			CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponAttachSocketName);
			CurrentWeapon->SetActorHiddenInGame(false);
			CurrentWeapon->SetCurrentAmmo(PistolTemp);
		}
	}
}

void ASCharacter::ChangeToRifle()
{	
	if (Role < ROLE_Authority)
	{
		ServerChangeToRifle();
	}

	if (Role == ROLE_Authority)
	{

		SaveCurrentAmmo();

		//Spawn a default weapon
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		CurrentWeapon->SetActorHiddenInGame(true);
		CurrentWeapon = GetWorld()->SpawnActor<ASWeapon>(WeaponRifle, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
		if (CurrentWeapon)
		{
			UE_LOG(LogTemp, Warning, TEXT("The SWeapon is Spawned"));
			CurrentWeapon->SetOwner(this);
			CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponAttachSocketName);
			CurrentWeapon->SetActorHiddenInGame(false);
			CurrentWeapon->SetCurrentAmmo(RifleTemp);
		}
	}
}

void ASCharacter::ChangeToBazooka()
{
	if (Role < ROLE_Authority)
	{
		ServerChangeToBazooka();
	}

	if (Role == ROLE_Authority)
	{

		SaveCurrentAmmo();

		//Spawn a default weapon
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		CurrentWeapon->SetActorHiddenInGame(true);
		CurrentWeapon = GetWorld()->SpawnActor<ASWeapon>(WeaponBazooka, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
		if (CurrentWeapon)
		{
			UE_LOG(LogTemp, Warning, TEXT("The SWeapon is Spawned"));
			CurrentWeapon->SetOwner(this);
			CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponAttachSocketName);
			CurrentWeapon->SetActorHiddenInGame(false);
			CurrentWeapon->SetCurrentAmmo(BazookaTemp);
		}
	}
}

void ASCharacter::SaveCurrentAmmo()
{
	if (CurrentWeapon->ActorHasTag("Pistol"))
	{
		PistolTemp = CurrentWeapon->GetCurrentAmmo();
	}
	if (CurrentWeapon->ActorHasTag("Rifle"))
	{
		RifleTemp = CurrentWeapon->GetCurrentAmmo();
	}
	if (CurrentWeapon->ActorHasTag("Bazooka"))
	{
		BazookaTemp = CurrentWeapon->GetCurrentAmmo();
	}
}

void ASCharacter::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	if (OtherActor->ActorHasTag("Powerup"))
	{
		if (this->ActorHasTag("Player"))
		{
			OtherActor->Destroy();
			CurrentWeapon->CurrentAmmo = CurrentWeapon->DefaultAmmo;
		}
	}
}

void ASCharacter::PlayWaveInProgressAmbiance_Implementation()
{
	UGameplayStatics::PlaySoundAtLocation(this, WaveInProgressAmbiance, GetActorLocation());
}

bool ASCharacter::PlayWaveInProgressAmbiance_Validate()
{
	return true;
}

void ASCharacter::PlayWaveEndSound_Implementation()
{
	UGameplayStatics::PlaySoundAtLocation(this, WaveFinishSound, GetActorLocation());
}

bool ASCharacter::PlayWaveEndSound_Validate()
{
	return true;
}

void ASCharacter::ServerChangeToPistol_Implementation()
{
	ChangeToPistol();
}

bool ASCharacter::ServerChangeToPistol_Validate()
{
	return true;
}

void ASCharacter::ServerChangeToRifle_Implementation()
{
	ChangeToRifle();
}

bool ASCharacter::ServerChangeToRifle_Validate()
{
	return true;
}

void ASCharacter::ServerChangeToBazooka_Implementation()
{
	ChangeToBazooka();
}

bool ASCharacter::ServerChangeToBazooka_Validate()
{
	return true;
}

void ASCharacter::OnHealthChanged(USHealthComponent* OwningHealthComp, float Health, float HealthDelta,
	const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (Health <= 0.0f && !bDied)
	{
		//Die!
		bDied = true;
	
		GetMovementComponent()->StopMovementImmediately();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		DetachFromControllerPendingDestroy();

		CurrentWeapon->Destroy(true);
		SetLifeSpan(10.0f);
	}
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float TargetFOV = bWantsToZoom ? ZoomedFOV : DefaulFOV;

	float NewFOV = FMath::FInterpTo(CameraComp->FieldOfView, TargetFOV, DeltaTime, ZoomInterpSpeed);

	CameraComp->SetFieldOfView(NewFOV);
}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);

	PlayerInputComponent->BindAxis("LookUp", this, &ASCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &ASCharacter::AddControllerYawInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ASCharacter::BeginCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ASCharacter::EndCrouch);

	PlayerInputComponent->BindAction("Zoom", IE_Pressed, this, &ASCharacter::BeginZoom);
	PlayerInputComponent->BindAction("Zoom", IE_Released, this, &ASCharacter::EndZoom);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ASCharacter::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ASCharacter::StopFire);

	PlayerInputComponent->BindAction("ChangeToPistol", IE_Released, this, &ASCharacter::ChangeToPistol);
	PlayerInputComponent->BindAction("ChangeToRifle", IE_Released, this, &ASCharacter::ChangeToRifle);
	PlayerInputComponent->BindAction("ChangeToBazooka", IE_Released, this, &ASCharacter::ChangeToBazooka);
}

FVector ASCharacter::GetPawnViewLocation() const
{
	if (CameraComp)
	{
		return CameraComp->GetComponentLocation();
	}

	return Super::GetPawnViewLocation();
}

ASWeapon * ASCharacter::GetCurrentWeapon() const
{
	return CurrentWeapon;
}

void ASCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const 
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASCharacter, CurrentWeapon);
	DOREPLIFETIME(ASCharacter, bDied);
	DOREPLIFETIME(ASCharacter, WeaponPistol);
	DOREPLIFETIME(ASCharacter, WeaponRifle);
	DOREPLIFETIME(ASCharacter, WeaponBazooka);
	DOREPLIFETIME(ASCharacter, WeaponAttachSocketName);
	DOREPLIFETIME(ASCharacter, StarterWeaponClass);
}