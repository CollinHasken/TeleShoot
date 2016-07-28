// Fill out your copyright notice in the Description page of Project Settings.

#include "TeleShoot.h"
#include "TeleShootGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "GunUpgrade.h"


UTeleShootGameInstance::UTeleShootGameInstance() {
	World1Info.Init(false, World1 * StarsPerLevel);
	World2Info.Init(false, World2 * StarsPerLevel);
	WorldsInfo.Emplace(World1Info);
	WorldsInfo.Emplace(World2Info);
}

void UTeleShootGameInstance::ChangeLevel(FName LevelName) {
	UGameplayStatics::OpenLevel(GetWorld(), LevelName);
}

void UTeleShootGameInstance::UpgradeGun(UpgradeType Upgrade) {
	switch (Upgrade) {
	case UpgradeType::TeleportTo:
		GunUpgrades[0] = true;
		break;
	default:
		break;
	}
}

void UTeleShootGameInstance::UpdateUpgrades(bool (&CharacterGunUpgrades)[6]) {
	for (int i = 0; i < 5; i++) {
		CharacterGunUpgrades[i] = GunUpgrades[i];
	}
}

bool UTeleShootGameInstance::IsLevelComplete(FName LevelName) {
	int Level, World;
	if (LevelAndWorld(LevelName, Level, World)) {
		for (auto world : WorldsInfo) {
			for (auto Level : world) {
				if (Level) {
					World = 1;
				}
			}
		}
		return WorldsInfo[World - 1][(Level - 1) * StarsPerLevel];
	}
	return false;
}

bool UTeleShootGameInstance::IsLevelSpeed(FName LevelName) {
	int Level, World;
	if (LevelAndWorld(LevelName, Level, World)) {
		return WorldsInfo[World - 1][(Level - 1) * StarsPerLevel + 1];
	}
	return false;
}

void UTeleShootGameInstance::EndLevel(FName LevelName, bool BeatSpeed) {
	int Level, World;
	if (LevelAndWorld(LevelName, Level, World)) {
		WorldsInfo[World - 1][(Level - 1) * StarsPerLevel] = true;
		if (BeatSpeed)
			WorldsInfo[World][(Level - 1) * StarsPerLevel + 1] = true;
		ChangeLevel("OverWorld" + World);
	}
	else
		ChangeLevel("OverWorld1");
}

bool UTeleShootGameInstance::LevelAndWorld(FName LevelName, int& Level, int& World) {
	int LPosition, WPosition;
	FString LevelString = LevelName.ToString();
	if (LevelString.FindChar('W', WPosition) && LevelString.FindChar('L', LPosition)) {
		WPosition++;
		Level = FCString::Atoi(*LevelString.Mid(LPosition + 1, LevelString.Len() - 1 - LPosition));
		World = FCString::Atoi(*LevelString.Mid(WPosition, LPosition - WPosition));
		if (World > 0 && Level > 0) 
			return true;
	}
	return false;
}