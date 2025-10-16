// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/CapsuleComponent.h"
#include "EGStartingButton.generated.h"

UCLASS()
class EG_API AEGStartingButton : public AActor
{
	GENERATED_BODY()

public:
	AEGStartingButton();
	
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,
						AActor* OtherActor,
						UPrimitiveComponent* OtherComp,
						int32 OtherBodyIndex,
						bool bFromSweep,
						const FHitResult& SweepResult);

	
	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* CapsuleComponent;
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComponent;

	
};
