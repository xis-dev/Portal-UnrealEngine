// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PortalManager.generated.h"

enum class PortalColor: uint8;


class APortal;

UCLASS()
class PROJECT1_API UPortalManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	UPortalManager();


	UFUNCTION(BlueprintCallable) //Delete after we translate the Blueprint into C++ Code
	void NewPortal(APortal* Portal, PortalColor color);

private:
	TArray<TWeakObjectPtr<APortal>> ActivePortals;

};
