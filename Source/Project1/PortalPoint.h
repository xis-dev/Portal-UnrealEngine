#pragma once

#include "PortalPoint.generated.h"
USTRUCT(BlueprintType)
struct FPortalPoint
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector WorldPos{};
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector Offset{};
};
