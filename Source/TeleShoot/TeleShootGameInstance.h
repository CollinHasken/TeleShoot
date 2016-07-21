// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/GameInstance.h"
#include "TeleShootGameInstance.generated.h"

/**
*
*/
UCLASS()
class TELESHOOT_API UTeleShootGameInstance : public UGameInstance
{
	GENERATED_BODY()

	UTeleShootGameInstance();
	const int Worlds = 2;
	const int World1 = 3;
	const int World2 = 4;
	const int StarsPerLevel = 2;

public:

	void ChangeLevel(FName LevelName);
	void EndLevel(FName LevelName, bool BeatSpeed);
	bool IsLevelComplete(FName LevelName);
	bool IsLevelSpeed(FName LevelName);

private:
	bool LevelAndWorld(FName LevelName, int& Level, int& World);
	TArray<bool> World1Info;
	TArray<bool> World2Info;
	TArray<TArray<bool>> WorldsInfo;
};
