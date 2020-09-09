// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoor.h"
#include "Engine/World.h"
#include "Components/AudioComponent.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"

// Define OUT to nothing, only for readability and for indentifying the out parameters.
#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	TargetYaw = InitialYaw + TargetYaw;

	FindAudioComponent();

	ActorThatOpen = GetWorld()->GetFirstPlayerController()->GetPawn();
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//UE_LOG(LogTemp, Warning, TEXT("%f"), GetOwner()->GetActorRotation().Yaw);

	if (TriggerBox){
		if (TriggerBox->IsOverlappingActor(ActorThatOpen) || TotalMassOfActors() >= MassToOpenDoors)
		{
			DoorLastOpened = GetWorld()->GetTimeSeconds();
			OpenDoor(DeltaTime);
		}
		else
		{
			if(GetWorld()->GetTimeSeconds() - DoorLastOpened > DoorCloseDelay)
				CloseDoor(DeltaTime);
		}
	}
	else
		UE_LOG(LogTemp, Error, TEXT("%s: You didn't setup the Trigger Box!!"), *GetOwner()->GetName());
	
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	CurrentYaw = FMath::FInterpConstantTo(CurrentYaw, TargetYaw, DeltaTime, DoorOpenAngleSpeedPerSeconds);
	FRotator DoorRotator = GetOwner()->GetActorRotation();
	DoorRotator.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotator);

	if (AudioComponent && bAudioPlayedOpenDoor == false)
	{
		AudioComponent->Play();
		bAudioPlayedOpenDoor = true;
		bAudioPlayedCloseDoor = false;
	}
	else return;
}


void UOpenDoor::CloseDoor(float DeltaTime)
{
	CurrentYaw = FMath::FInterpConstantTo(CurrentYaw, InitialYaw, DeltaTime, DoorCloseAngleSpeedPerSeconds);
	FRotator DoorRotator = GetOwner()->GetActorRotation();
	DoorRotator.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotator);

	if (AudioComponent && bAudioPlayedCloseDoor == false)
	{
		AudioComponent->Play();
		bAudioPlayedCloseDoor = true;
		bAudioPlayedOpenDoor = false;
	}
	else return;
}

float UOpenDoor::TotalMassOfActors() const
{
	float TotalMass = 0.f;

	TArray<AActor*> OverlappingActors;

	if (!TriggerBox) return 0.f;
	TriggerBox->GetOverlappingActors(OUT OverlappingActors);

	for (AActor* actor : OverlappingActors)
	{
		TotalMass += actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}
	return TotalMass;
}

void UOpenDoor::FindAudioComponent()
{
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();
	if (!AudioComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Player without AudioComponent component on %s"), *GetOwner()->GetName());
	}
}



