// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Variant_Shooter/ShooterCharacter.h"
#include "ShooterMainPlayer.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT1_API AShooterMainPlayer : public AShooterCharacter
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Weapons")
 	TSubclassOf<AShooterWeapon> WeaponTypeToSpawnWith;

protected:
	virtual void BeginPlay() override;
	

};
