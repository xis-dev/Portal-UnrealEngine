// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "PortalData.h"
#include "Project1GameMode.generated.h"

class UTextureRenderTarget2D;

/**
 *  Simple GameMode for a first person game
 */
UCLASS(abstract)
class AProject1GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AProject1GameMode();
	
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float PortalOffset{0.1f};
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<APortal> OrangePortal;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<APortal> BluePortal;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UTextureRenderTarget2D> OrangeRenderTarget;
		
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UTextureRenderTarget2D> BlueRenderTarget;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class TSubclassOf<APortal> PortalTypeToSpawn;
	
	UFUNCTION(BlueprintCallable, Category = "Utility")
	void GetPortalBounds(FVector& Min, FVector& Max);

	UFUNCTION(BlueprintCallable, Category = "Portal")
	void SpawnPortalWithColor(FVector hitPos, FVector impactNorm, bool isPortalOrange, bool& spawnedSuccessfully);

};



