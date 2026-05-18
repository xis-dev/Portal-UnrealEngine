// Fill out your copyright notice in the Description page of Project Settings.

#include "MainPlayer.h"

#include <iostream>
#include <ostream>

#include "Portal.h"
#include "Algo/Sort.h"
#include "PortalProjectile.h"

#include "EnhancedInputComponent.h"
#include "Project1GameMode.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/KismetMathLibrary.h"

#include "PortalPoint.h"

#include "GameFramework/ProjectileMovementComponent.h"

#define WallTraceChannel ECollisionChannel::ECC_GameTraceChannel2

void AMainPlayer::BeginPlay() {
	Super::BeginPlay();

	MaxPortalDistance = 1500.0f;
	AShooterCharacter::AddWeaponClass(WeaponTypeToSpawnWith);

	if (auto* gameMode = Cast<AProject1GameMode>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		gameMode->GetPortalBounds(PortalMin, PortalMax);
	}
	
	GetWorld()->GetTimerManager().SetTimer(WallCheckHandle, this, &AMainPlayer::CheckSpawnableWall, 0.1f, true);

}

void AMainPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(LeftClickAction, ETriggerEvent::Started, this, &AMainPlayer::LeftClickPortal);
		EnhancedInputComponent->BindAction(RightClickAction, ETriggerEvent::Started, this, &AMainPlayer::RightClickPortal);
	}
}

bool AMainPlayer::CanPortalSpawn(FVector location, FVector impactNormal, FVector surfacePos, FVector& outLoc)
{
	FRotator portalRot = APortal::GetRotatorFromNormal(impactNormal);
	
	FVector up = UKismetMathLibrary::GetUpVector(portalRot);
	FVector right = UKismetMathLibrary::GetRightVector(portalRot);
	TArray<FPortalPoint> PointsToCheck;
	
	PointsToCheck.Add({location + up * PortalMin.Z, up * PortalMin.Z});
	PointsToCheck.Add({location + up * PortalMax.Z, up * PortalMax.Z});
	PointsToCheck.Add({location + right * PortalMin.Y, right * PortalMin.Y});
	PointsToCheck.Add({location + right * PortalMax.Y, right * PortalMax.Y});
	
	// Algo::Sort(PointsToCheck, [&](const FPortalPoint& v1, const FPortalPoint& v2)
	// {
	// 	return FVector::DistSquared(surfacePos, v1.WorldPos) < FVector::DistSquared(surfacePos, v2.WorldPos);
	// });
	
	
	
	bool canSpawn = true;
	
	FVector firstNormal = impactNormal;
	// How far should the point check into
	const float inputDistance = 25.0f;
	// How far should the point start out of the normal
	const float outputDistance = 5.0f;
	
	FHitResult hitCenter;
	FVector start = location + (impactNormal * outputDistance);
	FVector end = location +(-impactNormal * (inputDistance + outputDistance)); 
	outLoc = location;
	if (!GetWorld()->LineTraceSingleByChannel(hitCenter,start,end, WallTraceChannel))
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot create portal here.")); 
		DrawDebugLine(GetWorld(), start, hitCenter.Location, FColor(255, 0, 0), false, 2.0f, 0, 5.0f); 
		return false;
	}
	for (const FPortalPoint& point: PointsToCheck)
	{
		if (!canSpawn)
		{
			UE_LOG(LogTemp, Warning, TEXT("Can't spawn portal"));
			break;
		}
		FVector startPos = point.WorldPos + (impactNormal * outputDistance);
		FVector endPos = startPos +(-impactNormal * (inputDistance + outputDistance)); 
		
		FHitResult hit;

			if (!GetWorld()->LineTraceSingleByChannel(hit, startPos, endPos, WallTraceChannel))
			{
				FVector pointToCenter = location - point.WorldPos;
				if (CanPortalSpawn(location + pointToCenter, impactNormal, surfacePos, outLoc))
				{
					return true;
				}
				canSpawn = false;
				DrawDebugLine(GetWorld(), startPos, hit.Location, FColor(255, 0, 0), false, 2.0f, 0, 5.0f);
				break;
			}
			
			if (!hit.ImpactNormal.Equals(firstNormal, 0.01f) || !hit.bBlockingHit)
			{
				canSpawn = false;
				break;
			};
			DrawDebugLine(GetWorld(), startPos, hit.Location, FColor(0, 255, 0), false, 2.0f, 0, 5.0f);
		

	}
	// if (!canSpawn)
	// {
	// 	canSpawn = TrySnapPortal(outLoc, PointsToCheck, location, surfacePos, impactNormal, inputDistance, outputDistance);
	// }
	return canSpawn;
}

// bool AMainPlayer::TrySnapPortal(FVector& outLoc, const TArray<FPortalPoint>& allPoints, FVector location,
//     FVector surfacePos, FVector impactNormal, float inputDistance, float outputDistance, int32 snapSteps)
// {
//     for (const FPortalPoint& ogPoint : allPoints)
//     {
//         for (int i = 1; i <= snapSteps; i++)
//         {
//             const float a = (float)i / (float)snapSteps;
//             const FVector nudge = ogPoint.Offset * a;
//             const FVector mainLoc = location + nudge;
//
//             bool allPass = true;
//
//             for (const FPortalPoint& point : allPoints)
//             {
//                 const FVector worldPoint = point.WorldPos + nudge;
//                 const FVector startPos   = worldPoint + (impactNormal * outputDistance);
//                 const FVector endPos     = startPos   - (impactNormal * (inputDistance + outputDistance));
//
//                 FHitResult hit;
//                 if (!GetWorld()->LineTraceSingleByChannel(hit, startPos, endPos, WallTraceChannel))
//                 {
//                     DrawDebugLine(GetWorld(), startPos, endPos, FColor(255, 0, 0), false, 2.0f, 0, 5.0f);
//                     allPass = false;
//                     break;
//                 }
//
//                 if (!hit.ImpactNormal.Equals(impactNormal, 0.01f) || !hit.bBlockingHit)
//                 {
//                     allPass = false;
//                     break;
//                 }
//
//                 DrawDebugLine(GetWorld(), startPos, hit.Location, FColor(0, 255, 0), false, 2.0f, 0, 5.0f);
//             }
//
//             if (allPass)
//             {
//                 outLoc = mainLoc;
//                 return true; 
//             }
//         }
//     }
//
//     return false; 
// }

	
void AMainPlayer::LeftClickPortal()
{
	FirePortalProjectile(true);
}

void AMainPlayer::RightClickPortal()
{
	FirePortalProjectile(false);
}

void AMainPlayer::FirePortalProjectile(bool bIsOrange)
{
	if (!PortalProjectileClass) 
	{
		UE_LOG(LogTemp, Error, TEXT("PortalProjectileClass is not set!"));
		return;
	}

	// Spawn location: camera position + a small offset forward (so it doesn't hit the player)
	const FVector CameraLocation = GetFirstPersonCameraComponent()->GetComponentLocation();
	const FRotator CameraRotation = GetFirstPersonCameraComponent()->GetComponentRotation();
	const FVector SpawnLocation = CameraLocation + CameraRotation.Vector() * 1.0f;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	APortalProjectile* Projectile = GetWorld()->SpawnActor<APortalProjectile>(
		PortalProjectileClass, SpawnLocation, CameraRotation, SpawnParams);

	if (Projectile)
	{
		UE_LOG(LogTemp, Warning, TEXT("Portal projectile spawned successfully! Orange: %d"), bIsOrange);
		
		Projectile->ChangeColour(bIsOrange);
		Projectile->bIsOrangePortal = bIsOrange;

		// Make the projectile ignore the player during movement
		if (UPrimitiveComponent* RootPrimitive = Cast<UPrimitiveComponent>(Projectile->GetRootComponent()))
		{
			RootPrimitive->IgnoreActorWhenMoving(this, true);
		}

		// Access the projectile movement component and set velocity
		if (UProjectileMovementComponent* ProjectileMovement = Projectile->FindComponentByClass<UProjectileMovementComponent>())
		{
			ProjectileMovement->Velocity = CameraRotation.Vector() * PortalProjectileSpeed;
			UE_LOG(LogTemp, Warning, TEXT("Projectile velocity set to: %s"), *(CameraRotation.Vector() * PortalProjectileSpeed).ToString());
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Portal projectile has no ProjectileMovementComponent!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to spawn portal projectile!"));
	}
}

FVector AMainPlayer::GetLineTraceEnd(FVector startPos, FVector startDir, float distance)
{	
	return FVector(startPos + (startDir * distance));
}

void AMainPlayer::HasBeenTeleported()
{
	ITeleportable::HasBeenTeleported();
	CanTP = false;
	GetWorld()->GetTimerManager().SetTimer(TeleportTimerHandle, this, &AMainPlayer::ResetCanTeleport, 0.25f);
}



bool AMainPlayer::GetCanTeleport()
{
	return CanTP;
}


void AMainPlayer::CheckSpawnableWall()
{
	FHitResult hit;
	
	FVector start = GetFirstPersonCameraComponent()->GetComponentLocation();
	FVector end = start + (GetFirstPersonCameraComponent()->GetForwardVector() * MaxCheckDistance);
	if (!GetWorld()->LineTraceSingleByChannel(hit, start, end, WallTraceChannel))
	{
		CannotSeeValidWall();
		return;
	}
	CanSeeValidWall();
}

void AMainPlayer::ResetCanTeleport()
{
	CanTP = true;
}
