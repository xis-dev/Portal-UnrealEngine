// Fill out your copyright notice in the Description page of Project Settings.


#include "Portal.h"

#include "MainPlayer.h"
#include "Project1GameMode.h"
#include "SAdvancedRotationInputBox.h"
#include "Teleportable.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Math/Quat.h"

#include "Components/SceneCaptureComponent2D.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Components/MeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Materials/MaterialInstanceDynamic.h"

// Sets default values
APortal::APortal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
		
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRoot"));
	SetRootComponent(Root);
	BoxTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Box T"));
	BoxTrigger->SetupAttachment(RootComponent);
	 BoxTrigger->SetBoxExtent(FVector(32.0f));
	BoxTrigger->SetRelativeScale3D(FVector(2.5f, 0.001f, 5.25f));
	
	SceneCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCapture"));
	SceneCapture->SetupAttachment(RootComponent);
	
	
	FinalPortalMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FinalPortalMesh"));
	FinalPortalMesh->SetupAttachment(RootComponent);
	
	RenderTargetMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RenderTargetMesh"));
	RenderTargetMesh->SetupAttachment(RootComponent);
	// SceneCapture->SetRelativeRotation(FRotator(360.0f, 0.0f, 90.0f));
	// PortalMesh->SetRelativeRotation(FRotator(-90.0f, 0.0f, -90.0f));
	// PortalMesh->SetRelativeScale3D(FVector(1.6f, 3.4425f, 1.0f));
	
	
}







void APortal::BeginPlay()
{
	Super::BeginPlay();

	BoxTrigger->OnComponentBeginOverlap.AddDynamic(this, &APortal::ActorBeginOverlap);
	
	ACharacter* player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (!player) return;
	if (AMainPlayer* mainP = Cast<AMainPlayer>(player))
	{
		MainPlayer = mainP;
	}
}




// Called every frame
void APortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (!OtherPortal) return;
	if (!MainPlayer) return;
	
	
	FQuat fixRot = FQuat(FRotator(0.0f, 180.0f, 0.0f));
	FTransform currentPortalTransform = GetTransform();
	FTransform otherPortalT = OtherPortal->GetTransform();
	
	FVector actorLocal = MainPlayer->GetActorLocation();
	actorLocal = currentPortalTransform.InverseTransformPosition(actorLocal);
	actorLocal = fixRot.RotateVector(actorLocal);
	FVector actorTarget = otherPortalT.TransformPosition(actorLocal);
	
	SceneCapture->SetWorldLocation(actorTarget);
	
	FQuat actorRot = MainPlayer->GetActorRotation().Quaternion();
	
	actorRot = currentPortalTransform.InverseTransformRotation(actorRot);
	actorRot = fixRot * actorRot;
	FQuat actorTargetRot = otherPortalT.TransformRotation(actorRot);
	
	SceneCapture->SetWorldRotation(actorTargetRot);
	SceneCapture->SetWorldLocation(OtherPortal->GetActorLocation() + (OtherPortal->GetActorForwardVector() * 12.5f));
	
}


void APortal::SetPortalParams(bool canTp, APortal* other, UTextureRenderTarget2D* currentRT)
{
	canTeleport = canTp;
	if (!other) return;
	OtherPortal = other;
	OtherPortalTransform = OtherPortal->GetTransform();
	
	SceneCapture->TextureTarget = currentRT;
	
	// Init with blue colour
	// FLinearColor portalColour{0.117, 0.117, 1.0};
	//
	// if (isOrange) portalColour = FLinearColor{1.0, 0.427, 0.0};
	//
	auto* matInst = RenderTargetMesh->CreateDynamicMaterialInstance(0, RenderTargetMesh->GetMaterial(0));
	if (matInst)
	{
		matInst->SetTextureParameterValue("RenderTexture", currentRT);
	}
}


void APortal::ColourChange_Implementation(bool isOrange)
{
	// Init with blue colour
	FLinearColor portalColour{0.117, 0.117, 1.0};
	if (isOrange) portalColour = FLinearColor{1.0, 0.250, 0.008};
	
	FinalPortalMesh->CreateDynamicMaterialInstance(0, FinalPortalMesh->GetMaterial(0));
	FinalPortalMesh->SetColorParameterValueOnMaterials("PortalColour", portalColour);
}



// bool APortal::CanPortalSpawn(FVector location, FVector impactNormal)
// {
// 	
// }




FRotator APortal::GetRotatorFromNormal(FVector impactNormal)
{
	FVector portalForward = impactNormal.GetSafeNormal(0.001);
	
	FVector portalUp{};
	FVector portalRight{};
	
	// Impact normal is nearly aligned with up vector
	if (FMath::Abs(FVector::DotProduct(portalForward, FVector::UpVector))> 0.99f)
	{
		// Cross with world forward as opposed to world up due to alignment.
		portalRight = FVector::CrossProduct(FVector::ForwardVector, portalForward);
	}
	else
	{
		portalRight = FVector::CrossProduct(FVector::UpVector, portalForward);
	}
	portalUp = FVector::CrossProduct(portalForward, portalRight); 
	
	return 	UKismetMathLibrary::MakeRotationFromAxes(portalForward, portalRight, portalUp);
}



void APortal::ActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{

	auto* actorTeleportable = Cast<ITeleportable>(OtherActor);
	if (!actorTeleportable || !OtherPortal) return;
	
	if (!actorTeleportable) return;
	if (!actorTeleportable->GetCanTeleport()) return;
	actorTeleportable->HasBeenTeleported();
	
	
	bool isCharacter{};
	auto* OverlappingCharacter = Cast<ACharacter>(OtherActor);
	if (OverlappingCharacter) isCharacter = true;
	
	FTransform currentPortalTransform = GetTransform();
	
	FTransform actorTransform = OtherActor->GetTransform();
	//actorTransform.SetRotation(OverlappingCharacter->GetControlRotation().Quaternion());
	
	FVector actorLocal = OtherActor->GetActorLocation();
	 actorLocal = currentPortalTransform.InverseTransformPosition(actorLocal);
	FVector actorTarget = OtherPortalTransform.TransformPosition(actorLocal);
	
	
	FVector dirTarget = actorTarget - OtherPortalTransform.GetLocation();
	
	FQuat actorRot = OtherActor->GetActorRotation().Quaternion();
	FQuat actorControlRot{};
	
	if (isCharacter) actorControlRot = OverlappingCharacter->GetControlRotation().Quaternion();
	
	FQuat fixRot = FQuat(FRotator(0.0f, 180.0f, 0.0f));
		
	FVector OtherPortalForward = UKismetMathLibrary::GetForwardVector((OtherPortalTransform.Rotator()));
	FVector currentForward = UKismetMathLibrary::GetForwardVector(currentPortalTransform.Rotator());
	
	actorRot = currentPortalTransform.InverseTransformRotation(actorRot);
	FQuat actorTargetRot = OtherPortalTransform.TransformRotation(actorRot);
	actorTargetRot = fixRot * actorTargetRot;
	
	actorControlRot = currentPortalTransform.InverseTransformRotation(actorControlRot);
	actorControlRot = fixRot * actorControlRot;
	FQuat actorTargetControlRot = OtherPortalTransform.TransformRotation(actorControlRot);
	
	FRotator actorTargetRot_Rotator{FRotator(actorTargetRot)};
	FRotator actorControlRot_Rotator{FRotator(actorTargetControlRot)};
	if (FMath::Abs(currentForward.Dot(FVector::UpVector)) > 0.99f && FMath::Abs(OtherPortalForward.Dot(FVector::UpVector)) < 0.99f)
	{
		actorTargetRot_Rotator = actorControlRot_Rotator = UKismetMathLibrary::MakeRotFromX(OtherPortalTransform.GetRotation().GetForwardVector()); 
	} 
	
	if (FMath::Abs(currentForward.Dot(FVector::UpVector)) > 0.99f && FMath::Abs(OtherPortalForward.Dot(FVector::UpVector)) > 0.99f ||
		FMath::Abs(currentForward.Dot(FVector::UpVector)) < 0.99f && FMath::Abs(OtherPortalForward.Dot(FVector::UpVector)) > 0.99f)
	{
		actorTargetRot_Rotator = actorControlRot_Rotator = UKismetMathLibrary::MakeRotFromXZ(OtherPortalTransform.GetRotation().GetUpVector(), FVector::UpVector);
	} 
	// Abitrary offset length to launch char
	const float teleportOffset = 10.0f;
	
    FVector Offset =  dirTarget.GetSafeNormal() * teleportOffset;

    // Add the offset to the target portal's location
    FVector NewLocation = actorTarget + Offset;
	

	OtherActor->SetActorRotation(actorTargetRot_Rotator);
    bool teleportSuccess = OtherActor->SetActorLocation(NewLocation);
	
	if (isCharacter)
	{
		auto* controller = OverlappingCharacter->GetController();
	
		FVector velocity = OverlappingCharacter->GetVelocity();
		
		if (UCharacterMovementComponent* moveComp = OverlappingCharacter->GetCharacterMovement())
		{
			moveComp->Velocity = velocity.Length() * dirTarget.GetSafeNormal();
			moveComp->UpdateComponentVelocity();
		}
		controller->SetControlRotation(actorControlRot_Rotator);
	}
	
	
	if (!teleportSuccess)
	{
        UE_LOG(LogTemp, Warning, TEXT("Portal teleport failed for %s"), *OtherActor->GetName());
	}
	
}





