// Fill out your copyright notice in the Description page of Project Settings.


#include "ExtractionZone.h"

#include "FPSCharacter.h"
#include "FPSGameMode.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AExtractionZone::AExtractionZone()
{
	OverlappingComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Overlapping Component"));
	RootComponent = OverlappingComponent;

	OverlappingComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlappingComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	OverlappingComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	OverlappingComponent->SetBoxExtent(FVector(200));
	OverlappingComponent->SetHiddenInGame(true);
	OverlappingComponent->OnComponentBeginOverlap.AddDynamic(this, &AExtractionZone::HandleOverlap);

	DecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("Decal Component"));
	DecalComponent->SetupAttachment(RootComponent);
	DecalComponent->DecalSize = FVector(200);
}


void AExtractionZone::HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                    const FHitResult& SweepResult)
{
	AFPSCharacter* Character = Cast<AFPSCharacter>(OtherActor);
	if (!Character)
	{
		return;
	}
	if(Character->bIsCarryingObjective)
	{
		AFPSGameMode* GameMode = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
		if (GameMode)
		{
			GameMode->CompleteMission(Character, true);
		}	
	}
	else
	{
		UGameplayStatics::PlaySound2D(this, ObjectiveMissingSound);
		UGameplayStatics::PlayWorldCameraShake(GetWorld(), CameraShake, GetActorLocation(), 0.0f, 500.0f);
	}

}
