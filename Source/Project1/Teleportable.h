#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Teleportable.generated.h"

UINTERFACE(BlueprintType, Blueprintable)
class UTeleportable : public UInterface
{
	GENERATED_BODY()
};

class ITeleportable
{
	GENERATED_BODY()

public:
	virtual void HasBeenTeleported();
	
	virtual bool GetCanTeleport();
	
	
};