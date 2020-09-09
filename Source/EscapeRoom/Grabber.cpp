// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

// Define OUT to nothing, only for readability and for indentifying the out parameters.
#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandle();
	SetupInputComponent();
}

void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
		InputComponent->BindAction("Push", IE_Pressed, this, &UGrabber::AddForceToGrabbedObject);
	}
}

void UGrabber::FindPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("Player without PhysicsHandle component on %s"), *GetOwner()->GetName());
	}
}
	
void UGrabber::Grab()
{
	HitResult = GetFirstPhysicsBodyInReach();

	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();

	AActor* ActorGrabbed = HitResult.GetActor();

	if (ActorGrabbed)
	{
		if (!PhysicsHandle) return;

		PhysicsHandle->GrabComponentAtLocationWithRotation(ComponentToGrab, NAME_None, ActorGrabbed->GetActorLocation(), ActorGrabbed->GetActorRotation());
		UE_LOG(LogTemp, Error, TEXT("SPINGO L'OGGETTO %s"), *ActorGrabbed->GetName());
	}
}


void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab release"));

	if (!PhysicsHandle) return;
	PhysicsHandle->ReleaseComponent();
	Reach = 200.f;
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	RenderLineTrace();

	if (!PhysicsHandle) return;
	if (PhysicsHandle->GrabbedComponent)
	{
		if (InputComponent)
		{
			InputComponent->BindAction("IncreaseGrabDistance", IE_Pressed, this, &UGrabber::AddDistanceToGrabbedObject);
		}


		PhysicsHandle->SetTargetLocation(GetPlayersReach());
	}
}

void UGrabber::RenderLineTrace()
{
	DrawDebugLine(
		GetWorld(),
		GetPlayersWorldPos(),
		GetPlayersReach(),
		FColor(0, 255, 0),
		false,
		0.f,
		0,
		5.f
	);
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(OUT Hit,
		GetPlayersWorldPos(),
		GetPlayersReach(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);

	return Hit;
}

FVector UGrabber::GetPlayersReach() const
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);

	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;	
}

FVector UGrabber::GetPlayersWorldPos() const
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);
	return PlayerViewPointLocation;
}


void UGrabber::AddForceToGrabbedObject()
{
	HitResult = GetFirstPhysicsBodyInReach();
	AActor* PushedActor = HitResult.GetActor();
	FVector Force(100.f, 100.f, 100.f);

	if (PushedActor)
	{
		UE_LOG(LogTemp, Error, TEXT("SPINGO L'OGGETTO %s"), *PushedActor->GetName());
		UStaticMeshComponent* StaticMesh = PushedActor->FindComponentByClass<UStaticMeshComponent>();
		StaticMesh->AddForce(GetPlayersReach() * 100 * StaticMesh->GetMass(), NAME_None, true);
	}
}


void UGrabber::AddDistanceToGrabbedObject()
{
	Reach = Reach + 0.01f;
	UE_LOG(LogTemp, Error, TEXT("ALLONTANO L'OGGETTO"));
}



