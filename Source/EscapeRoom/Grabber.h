// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPEROOM_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	float Reach = 200.f;

	UPROPERTY(EditAnywhere)
	float ForwardForceToAdd = 200.f;

	FVector DistanceToGrabbedObject = {0.f, 0.f, 0.f};

	FHitResult HitResult;

	UPROPERTY()
	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UPROPERTY()
	UInputComponent* InputComponent = nullptr;

	void Grab();
	void Release();
	void FindPhysicsHandle();
	void SetupInputComponent();

	void RenderLineTrace();
	FHitResult GetFirstPhysicsBodyInReach() const;

	FVector GetPlayersReach() const;

	FVector GetPlayersWorldPos() const;

	void AddForceToGrabbedObject();
	void AddDistanceToGrabbedObject();
};
