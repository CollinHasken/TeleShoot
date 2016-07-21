// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "Control.generated.h"

/**
*
*/
UCLASS()
class TELESHOOT_API AControl : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;


};
