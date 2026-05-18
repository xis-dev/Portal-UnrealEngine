// Copyright Epic Games, Inc. All Rights Reserved.

#include "Project1GameMode.h"

#include "Portal.h"

AProject1GameMode::AProject1GameMode()
{
	// stub
}

void AProject1GameMode::GetPortalBounds(FVector& Min, FVector& Max)
{
	if (auto* tempPortal = GetWorld()->SpawnActor<APortal>(PortalTypeToSpawn, FVector(999999999.0f), FRotator(0.0f)))
	{
		tempPortal->FinalPortalMesh->GetLocalBounds(Min, Max);
		FTransform rot = FTransform{tempPortal->FinalPortalMesh->GetRelativeRotation()};
		rot.SetScale3D(tempPortal->FinalPortalMesh->GetRelativeScale3D());
		Min = rot.TransformVector(Min);
		Max = rot.TransformVector(Max);
		tempPortal->Destroy();
		return;
	}
	Min = Max = FVector(0.0f);
}

void AProject1GameMode::SpawnPortalWithColor(FVector hitPos, FVector impactNorm, bool isPortalOrange,
                                             bool& spawnedSuccessfully)
{
	auto portalRot = APortal::GetRotatorFromNormal(impactNorm);
	
	FVector spawnLoc = hitPos;
	if (auto* portal = GetWorld()->SpawnActor<APortal>(PortalTypeToSpawn, spawnLoc, portalRot))
	{
		if (isPortalOrange)
		{
			if (IsValid(OrangePortal)) OrangePortal->Destroy();
			
			OrangePortal = portal;

		}
		else
		{
			if (IsValid(BluePortal)) BluePortal->Destroy();
			
			
			BluePortal = portal;
		}
		
		portal->ColourChange(isPortalOrange);
		
		if (IsValid(BluePortal) && IsValid(OrangePortal))
		{
			OrangePortal->SetPortalParams(true, BluePortal, OrangeRenderTarget);
			BluePortal->SetPortalParams(true, OrangePortal, BlueRenderTarget);
		}
		
		spawnedSuccessfully = true; 
		return;
	}
	
	spawnedSuccessfully = false;
	
	
}
