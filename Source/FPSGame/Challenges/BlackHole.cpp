// Fill out your copyright notice in the Description page of Project Settings.


#include "BlackHole.h"

#include "Components/PrimitiveComponent.h"
#include "Components/SphereComponent.h"


// Sets default values
ABlackHole::ABlackHole()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	RootComponent = MeshComponent;

	InnerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Inner Sphere"));
	InnerSphere->SetupAttachment(RootComponent);

	OuterSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Outer Sphere"));
	OuterSphere->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABlackHole::BeginPlay()
{
	Super::BeginPlay();

	InnerSphere->OnComponentBeginOverlap.AddDynamic(this, &ABlackHole::InnerSphereOnOverlapBegin);
}

void ABlackHole::InnerSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                           UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                           const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		OtherActor->Destroy();
	}
}

// Called every frame
void ABlackHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<UPrimitiveComponent*> OutOverlappingComponents;
	OuterSphere->GetOverlappingComponents(OutOverlappingComponents);

	for (UPrimitiveComponent* OverlappingComponent : OutOverlappingComponents)
	{
		if (OverlappingComponent && OverlappingComponent->IsSimulatingPhysics())
		{
			const float Radius = OuterSphere->GetScaledSphereRadius();
			OverlappingComponent->AddRadialForce(GetActorLocation(), Radius, Strength,
			                                     ERadialImpulseFalloff::RIF_Constant, true);
		}
	}
}
