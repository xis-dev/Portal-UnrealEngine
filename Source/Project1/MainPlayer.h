// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PortalPoint.h"
#include "Teleportable.h"
#include "Variant_Shooter/ShooterCharacter.h"
#include "MainPlayer.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT1_API AMainPlayer : public AShooterCharacter, public ITeleportable
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* LeftClickAction;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* RightClickAction;
	
	UPROPERTY(EditAnywhere, Category = "Weapons")
	TSubclassOf<AShooterWeapon> WeaponTypeToSpawnWith;

	UPROPERTY()
	FVector PortalMin;
	UPROPERTY()
	FVector PortalMax;
	// Projectile class to spawn for portal shots
	UPROPERTY(EditDefaultsOnly, Category = "Weapons")
	TSubclassOf<class APortalProjectile> PortalProjectileClass;

	// Speed of the portal projectile
	UPROPERTY(EditDefaultsOnly, Category = "Weapons")
	float PortalProjectileSpeed = 3000.0f;
	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Utility")
	float MaxPortalDistance;

protected:
	virtual void BeginPlay() override;
	
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
public:
	
	UPROPERTY()
	bool CanTP{true};
	
	UPROPERTY()
	FTimerHandle TeleportTimerHandle;
	
	UPROPERTY()
	FTimerHandle WallCheckHandle;
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Utility")
	void ValidatedPortal(bool canSpawn);
	
	UFUNCTION(BlueprintCallable, Category = "Utility")
	bool CanPortalSpawn(FVector location, FVector impactNormal, FVector surfacePos, FVector& outLoc);
	
	// UFUNCTION(BlueprintCallable, Category = "Utility")
	// bool TrySnapPortal(FVector& outLoc, const TArray<FPortalPoint>& allPoints, FVector location, FVector surfacePos, FVector impactNormal, float
	//                    inputDistance, float
	//                    outputDistance, int32 snapSteps
	// 	                   = 4);

	
	UFUNCTION(BlueprintCallable, Category = "Input")
	void LeftClickPortal();
	
	UFUNCTION(BlueprintCallable, Category = "Input")
	void RightClickPortal();
	
	UFUNCTION(BlueprintCallable, Category = "Utility")
	FVector GetLineTraceEnd(FVector startPos, FVector startDir, float distance);
	
	virtual void HasBeenTeleported() override;
	
	UFUNCTION(BlueprintCallable)
	virtual bool GetCanTeleport() override;
	
	
	const float MaxCheckDistance{2000.0f};
	
	UFUNCTION()
	void CheckSpawnableWall();
	
	UFUNCTION(BlueprintImplementableEvent)
	void CanSeeValidWall();
	
	UFUNCTION(BlueprintImplementableEvent)
	void CannotSeeValidWall();
	
	
	UFUNCTION()
	void ResetCanTeleport();

	// Fires a portal projectile
	UFUNCTION()
	void FirePortalProjectile(bool bIsOrange);
};