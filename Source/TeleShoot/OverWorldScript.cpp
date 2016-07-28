// Fill out your copyright notice in the Description page of Project Settings.

#include "TeleShoot.h"
#include "OverWorldScript.h"
#include "TeleShootGameInstance.h"
#include "Portal.h"
#include "EngineUtils.h"

void AOverWorldScript::BeginPlay()
{
	UTeleShootGameInstance* Game = Cast<UTeleShootGameInstance>(GetGameInstance());
	FName Level = Game->GetRecentLevel();
	if (!Level.IsNone()) {
		for (TActorIterator<APortal> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
			if (ActorItr->LevelName.IsEqual(Level)) {
				FVector SpawnLocation(ActorItr->GetActorLocation());
				SpawnLocation.Y = GetWorld()->GetFirstPlayerController()->GetCharacter()->GetActorLocation().Y;
				GetWorld()->GetFirstPlayerController()->GetCharacter()->SetActorLocation(SpawnLocation);
			}
		}
	}
}
