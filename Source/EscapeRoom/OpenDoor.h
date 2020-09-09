// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPEROOM_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
	float TotalMassOfActors() const;
	void FindAudioComponent();

private:
	float InitialYaw;
	float CurrentYaw;

	UPROPERTY(EditAnywhere)
	float TargetYaw = 90.f;

	UPROPERTY(EditAnywhere)
	float MassToOpenDoors = 10.f;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* TriggerBox = nullptr;

	AActor* ActorThatOpen = nullptr;

	float DoorLastOpened = 0.f;

	bool bAudioPlayedOpenDoor = false;
	bool bAudioPlayedCloseDoor = true;

	UPROPERTY(EditAnywhere)
	float DoorCloseDelay = 2.f;

	UPROPERTY(EditAnywhere)
	float DoorOpenAngleSpeedPerSeconds = 30.f;

	UPROPERTY(EditAnywhere)
	float DoorCloseAngleSpeedPerSeconds = 45.f;

	UPROPERTY()
	UAudioComponent* AudioComponent = nullptr;
};
