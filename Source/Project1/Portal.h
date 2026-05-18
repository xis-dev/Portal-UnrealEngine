// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Portal.generated.h"

class AMainPlayer;
class UBoxComponent;
class USceneComponent;
class USceneCaptureComponent2D;
class UStaticMeshComponent;
UCLASS()
class PROJECT1_API APortal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APortal();

	

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* Root;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UBoxComponent> BoxTrigger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<USceneCaptureComponent2D> SceneCapture;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UStaticMeshComponent> FinalPortalMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UStaticMeshComponent> RenderTargetMesh;
	
	
protected:


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	 // Blueprint Event for overlap - we will bind to it in C++
    UFUNCTION()
    void ActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UPROPERTY(BlueprintReadOnly, Category = "Portal")
	FTransform OtherPortalTransform;
	
	UPROPERTY(BlueprintReadOnly, Category = "Portal")
	APortal* OtherPortal;
	
	UPROPERTY()
	AMainPlayer* MainPlayer;
	

    // The variable that decides if this is the Orange or Blue portal
    UPROPERTY(BlueprintReadOnly,Category = "Portal")
    bool canTeleport{};

private:



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// UFUNCTION(BlueprintCallable, Category = "Utility")
	// static bool CanPortalSpawn(FVector location, FVector impactNormal);
	
	
	
	
	UFUNCTION(BlueprintCallable, Category = "Utility")
	void SetPortalParams(bool canTp, APortal* other, UTextureRenderTarget2D* currentRT);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ColourChange(bool isOrange);
	
	static FRotator GetRotatorFromNormal(FVector impactNormal);
	
	
};
