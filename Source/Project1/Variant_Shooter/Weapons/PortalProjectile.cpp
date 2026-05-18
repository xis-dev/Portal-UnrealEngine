// Fill out your copyright notice in the Description page of Project Settings.

#include "PortalProjectile.h"
#include "Portal.h"
#include "MainPlayer.h"
#include "Project1GameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "PortlableWall.h"

#define WallTraceChannel ECollisionChannel::ECC_Visibility

void APortalProjectile::ChangeColour_Implementation(bool bIsOrange)
{
	
}




void APortalProjectile::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp,
                                  bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	// Let the base class do its own thing
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	// Make sure we have a valid world and blocking hit
	if (!GetWorld() || !Hit.bBlockingHit)
		return;

	// Ignore hits against the firing player
	if (Other == GetInstigator())
		return;

	UE_LOG(LogTemp, Warning, TEXT("Portal projectile hit something: %s"), *Other->GetName());

	const FVector HitLoc = Hit.ImpactPoint + (Hit.ImpactNormal * 7.5f);
	const FVector HitNorm = Hit.ImpactNormal;

	if (CanPlacePortal(HitLoc, HitNorm, Other))
	{
		UE_LOG(LogTemp, Warning, TEXT("Valid portal location found!"));
		
		FVector locationToSpawn{};
		if (AMainPlayer* Player = Cast<AMainPlayer>(GetInstigator()))
		{
			if (Player->CanPortalSpawn(HitLoc, HitNorm, Other->GetActorLocation(), locationToSpawn))
			{
				if (auto* GameMode = Cast<AProject1GameMode>(UGameplayStatics::GetGameMode(GetWorld())))
				{	
					bool bSpawnSuccess = false;
					GameMode->SpawnPortalWithColor(locationToSpawn, HitNorm, bIsOrangePortal, bSpawnSuccess);
			
					if (!bSpawnSuccess)
					{
						UE_LOG(LogTemp, Warning, TEXT("Portal projectile failed to spawn portal"));
					}
				}
			}
		}
		
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot place portal at this location"));
	}

	// Notify player about validation (for UI)
	NotifyPlayerValidation(CanPlacePortal(HitLoc, HitNorm, Other));
	
	// Destroy the projectile
	Destroy();
}

bool APortalProjectile::CanPlacePortal(const FVector& Location, const FVector& Normal, AActor* HitActor) const
{
	// Only allow portals on actors tagged as "PortalWall"
	if (!HitActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot place portal: actor is not a PortalWall"));
		return false;
	}
	
	if (auto* hitWall = Cast<APortlableWall>(HitActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("Portal placement allowed on PortalWall!"));
		return true;	
	}
	
			UE_LOG(LogTemp, Warning, TEXT("Cannot place portal: actor is not a PortalWall"));
		return false;
}


void APortalProjectile::NotifyPlayerValidation(bool bCanSpawn)
{
	// The Instigator should be the AMainPlayer that fired this projectile
	if (AMainPlayer* Player = Cast<AMainPlayer>(GetInstigator()))
	{
		// ValidatedPortal is a BlueprintImplementableEvent on the player
		Player->ValidatedPortal(bCanSpawn);
	}
}