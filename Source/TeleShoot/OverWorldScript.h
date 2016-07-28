// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/LevelScriptActor.h"
#include "OverWorldScript.generated.h"

/**
 * 
 */
UCLASS()
class TELESHOOT_API AOverWorldScript : public ALevelScriptActor
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, Category = Activates)
		int World;

	UPROPERTY(EditAnywhere, Category = Activates)
		int Levels;

protected:
	
	virtual void BeginPlay() override;
	
};
