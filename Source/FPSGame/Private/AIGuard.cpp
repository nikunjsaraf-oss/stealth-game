// Fill out your copyright notice in the Description page of Project Settings.


#include "AIGuard.h"

#include "FPSGameMode.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Perception/PawnSensingComponent.h"
#include "TimerManager.h"
#include "UnrealNetwork.h"

// Sets default values
AAIGuard::AAIGuard()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"));

	PawnSensingComponent->OnSeePawn.AddDynamic(this, &AAIGuard::OnPawnSeen);
	GuardState = EAIState::Idle;
}

// Called when the game starts or when spawned
void AAIGuard::BeginPlay()
{
	Super::BeginPlay();

	OriginalRotation = GetActorRotation();
	PawnSensingComponent->OnHearNoise.AddDynamic(this, &AAIGuard::OnNoiseHeard);

	if (bPatrol)
	{
		MoveToCurrentPatrolPoint();
	}
}

void AAIGuard::OnRep_GuardState()
{
	OnStateChanged(GuardState);
}

void AAIGuard::SetGuardState(EAIState NewState)
{	
	if (GuardState == NewState) return;
	GuardState = NewState;
	OnRep_GuardState();

	OnStateChanged(NewState);
}

void AAIGuard::OnPawnSeen(APawn* PawnSeen)
{
	if (!PawnSeen) return;


	AFPSGameMode* GameMode = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		GameMode->CompleteMission(PawnSeen, false);
	}

	SetGuardState(EAIState::Alerted);

	AController* Controller = GetController();
	if (Controller)
	{
		Controller->StopMovement();
	}
}

void AAIGuard::OnNoiseHeard(APawn* NoiseInstigator, const FVector& Location, float Volume)
{
	if (GuardState == EAIState::Alerted) return;

	FVector Direction = Location - GetActorLocation();
	Direction.Normalize();

	FRotator NewLookAt = FRotationMatrix::MakeFromX(Direction).Rotator();
	NewLookAt.Pitch = 0;
	NewLookAt.Roll = 0;

	SetActorRotation(NewLookAt);

	GetWorldTimerManager().ClearTimer(ResetOrientation);
	GetWorldTimerManager().SetTimer(ResetOrientation, this, &AAIGuard::ResetRotation, ResetOrientationTimeInterval);

	SetGuardState(EAIState::Suspicious);

	AController* Controller = GetController();
	if (Controller)
	{
		Controller->StopMovement();
	}
}

void AAIGuard::ResetRotation()
{
	if (GuardState == EAIState::Alerted) return;

	SetActorRotation(OriginalRotation);
	SetGuardState(EAIState::Idle);

	if (bPatrol)
	{
		MoveToCurrentPatrolPoint();
	}
}

void AAIGuard::MoveToCurrentPatrolPoint()
{
	if (CurrentPatrolPoint == nullptr || CurrentPatrolPoint == SecondPatrolPoint)
	{
		CurrentPatrolPoint = FirstPatrolPoint;
	}
	else
	{
		CurrentPatrolPoint = SecondPatrolPoint;
	}

	UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), CurrentPatrolPoint);
}

// Called every frame
void AAIGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentPatrolPoint)
	{
		const FVector Delta = GetActorLocation() - CurrentPatrolPoint->GetActorLocation();
		const float DistanceToGoal = Delta.Size();

		if (DistanceToGoal < 50)
		{
			MoveToCurrentPatrolPoint();
		}
	}
}

void AAIGuard::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAIGuard, GuardState);
}

