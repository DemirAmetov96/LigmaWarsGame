#include "Collectible.h"

// Sets default values
ACollectible::ACollectible()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACollectible::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority()) {
		SetReplicates(true);
		SetReplicateMovement(true);
	}
}

void ACollectible::MakeObjectRotate(float DeltaTime)
{
	FRotator currentRotation = GetActorRotation();
	currentRotation.Yaw = currentRotation.Yaw + (RotationSpeed * DeltaTime);
	SetActorRotation(currentRotation);
}

void ACollectible::MakeObjectFloat(float DeltaTime)
{
	//Get the location of the actor every frame
	CurrentLocation = GetActorLocation();

	//Delta Time is the time between each frame. If we add the DeltaTime each frame, we get the total running time (RunningTime)
	RunningTime = RunningTime + DeltaTime;

	//We want the change in height to be smooth, so we use a sin function
	SinWave = (FMath::Sin(RunningTime * FloatingSpeed));

	//Change the z position every frame. SinWave returns values between 1 and -1, but we wont notice that in the world its too small, so we multiply the change by FloatingHeight
	CurrentLocation.Z = CurrentLocation.Z + (SinWave * FloatingHeight * DeltaTime);

	//We set the new updated current location onto the actor to move it
	SetActorLocation(CurrentLocation);
}

// Called every frame
void ACollectible::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (HasAuthority()) {

		//Function that makes the object float smoothly
		MakeObjectFloat(DeltaTime);

		//Function that rotates the object
		MakeObjectRotate(DeltaTime);
	}
}
//This is how to debug to console in Unreal Engine
/*FString TheFloatStr = FString::SanitizeFloat(SinWave);
UE_LOG(LogClass, Log, TEXT("My Int Value: %s"), *TheFloatStr);*/
