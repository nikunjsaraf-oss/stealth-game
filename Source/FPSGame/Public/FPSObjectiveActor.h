// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "FPSObjectiveActor.generated.h"

class USphereComponent;
class UParticleSystem;

UCLASS()
class FPSGAME_API AFPSObjectiveActor : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, Category="Components")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, Category="Components")
	USphereComponent* SphereComponent;

	UPROPERTY(EditDefaultsOnly, Category="Effects")
	UParticleSystem* PickupEffect;

	
public:	
	// Sets default values for this actor's properties
	AFPSObjectiveActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void PlayEffects() const;

public:	


	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

};
