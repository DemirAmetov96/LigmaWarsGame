// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "SCharacter.generated.h"


class UCameraComponent;
class USpringArmComponent;
class ASWeapon;
class USHealthComponent;

UCLASS()
class LIGMAWARS_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);

	void BeginCrouch();
	void EndCrouch();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	USHealthComponent* HealthComp;

	bool bWantsToZoom;
	UPROPERTY(EditDefaultsOnly, Category = "Player")
	float ZoomedFOV;
	UPROPERTY(EditDefaultsOnly, Category = "Player", meta = (ClampMin = 0.1, ClampMax = 100))
	float ZoomInterpSpeed;
	float DefaulFOV;

	void BeginZoom();
	void EndZoom();

	UPROPERTY(Replicated)
	ASWeapon* CurrentWeapon;

	UPROPERTY(EditDefaultsOnly, Replicated, Category = "Player")
		TSubclassOf<ASWeapon> StarterWeaponClass;

	UPROPERTY(EditDefaultsOnly, Replicated, Category = "Player")
		TSubclassOf<ASWeapon> WeaponPistol;

	UPROPERTY(EditDefaultsOnly, Replicated, Category = "Player")
		TSubclassOf<ASWeapon> WeaponRifle;

	UPROPERTY(EditDefaultsOnly, Replicated, Category = "Player")
		TSubclassOf<ASWeapon> WeaponBazooka;

	UPROPERTY(VisibleDefaultsOnly, Replicated, Category = "Player")
	FName WeaponAttachSocketName;

	UFUNCTION()
	void OnHealthChanged(USHealthComponent* OwningHealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Player")
	bool bDied;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual FVector GetPawnViewLocation() const override;
	
	UFUNCTION(BlueprintCallable, Category = "Player")
		ASWeapon* GetCurrentWeapon() const;

	UFUNCTION(BlueprintCallable, Category = "Player")
		void StartFire();

	UFUNCTION(BlueprintCallable, Category = "Player")
		void StopFire();

	UFUNCTION()
	void ChangeToPistol();

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerChangeToPistol();

	UFUNCTION()
		void ChangeToRifle();

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerChangeToRifle();

	UFUNCTION()
		void ChangeToBazooka();

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerChangeToBazooka();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Replicated, Category = "Weapon")
		int32 PistolTemp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Replicated, Category = "Weapon")
		int32 RifleTemp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Replicated, Category = "Weapon")
		int32 BazookaTemp;

	UFUNCTION()
		void SaveCurrentAmmo();

	UPROPERTY(EditDefaultsOnly, Category = "Player")
		USoundCue* WaveFinishSound;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
		USoundCue* WaveInProgressAmbiance;

	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void PlayWaveEndSound();

	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void PlayWaveInProgressAmbiance();

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};