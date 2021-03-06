// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSGameMode.h"

#include "EngineUtils.h"
#include "FPSHUD.h"
#include "FPSGameState.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

AFPSGameMode::AFPSGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_Player"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AFPSHUD::StaticClass();

	GameStateClass = AFPSGameState::StaticClass();
}

void AFPSGameMode::CompleteMission(APawn* InstigatorPawn, bool bMissionSuccess)
{
	if (InstigatorPawn)
	{
		if (SpectatingCamera)
		{
			TArray<AActor*> OutActors;
			UGameplayStatics::GetAllActorsOfClass(this, SpectatingCamera, OutActors);

			if (OutActors.Num() > 0)
			{
				AActor* NewViewTarget = OutActors[0];

				for (TActorIterator<APlayerController> Iterator(GetWorld()); Iterator; ++Iterator)
				{
					APlayerController* PC = *Iterator;
					if (PC)
					{
						PC->SetViewTargetWithBlend(NewViewTarget, 0.5f,
						                           EViewTargetBlendFunction::VTBlend_Cubic);
					}
				}
			}
		}
	}

	AFPSGameState* GS = GetGameState<AFPSGameState>();
	if (GS)
	{
		GS->MulticastOnMissionComplete(InstigatorPawn, bMissionSuccess);
	}

	OnMissionCompleted(InstigatorPawn, bMissionSuccess);
}
