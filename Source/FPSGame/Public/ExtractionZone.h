// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "ExtractionZone.generated.h"

class UBoxComponent;

UCLASS()
class FPSGAME_API AExtractionZone : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category="Components")
	UBoxComponent* OverlappingComponent;

	UPROPERTY(VisibleAnywhere, Category="Components")
	UDecalComponent* DecalComponent;

	UPROPERTY(EditDefaultsOnly, Category="Components") 
	USoundBase* ObjectiveMissingSound;

	UPROPERTY(EditDefaultsOnly, Category="Components")
	TSubclassOf<UCameraShake> CameraShake;

public:
	// Sets default values for this actor's properties
	AExtractionZone();

protected:
	UFUNCTION()
	void HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                   int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
