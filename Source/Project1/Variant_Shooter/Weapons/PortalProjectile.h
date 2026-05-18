// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Variant_Shooter/Weapons/ShooterProjectile.h"
#include "Teleportable.h"
#include "PortalProjectile.generated.h"

/**
 * Projectile that spawns a portal on impact with a wall
 */
UCLASS()
class PROJECT1_API APortalProjectile : public AShooterProjectile
{
	GENERATED_BODY()

public:
	// Set this when firing the projectile - true for orange, false for blue
	UPROPERTY(BlueprintReadWrite, Category = "Portal")
	bool bIsOrangePortal = true;
	
	
	UFUNCTION(BlueprintNativeEvent)
	void ChangeColour(bool bIsOrange);
	
	

protected:
	// Override NotifyHit to catch all blocking collisions
	virtual void NotifyHit(
		class UPrimitiveComponent* MyComp,
		AActor* Other,
		class UPrimitiveComponent* OtherComp,
		bool bSelfMoved,
		FVector HitLocation,
		FVector HitNormal,
		FVector NormalImpulse,
		const FHitResult& Hit) override;

private:
	bool CanPlacePortal(const FVector& Location, const FVector& Normal, AActor* HitActor) const;
	
	
	// Helper to notify the owning player about validation result (UI feedback)
	void NotifyPlayerValidation(bool bCanSpawn);
};