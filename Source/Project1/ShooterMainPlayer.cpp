// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterMainPlayer.h"

void AShooterMainPlayer::BeginPlay() {
	Super::BeginPlay();

	AShooterCharacter::AddWeaponClass(WeaponTypeToSpawnWith);

}




