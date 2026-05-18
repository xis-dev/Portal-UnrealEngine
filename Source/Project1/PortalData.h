#pragma once


#include "Portal.h"

#include "PortalData.generated.h"


USTRUCT(BlueprintType)
struct FPortalData
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<APortal> PortalObj;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FTransform PortalTransform;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FTransform TestTransform;
	
	
};
