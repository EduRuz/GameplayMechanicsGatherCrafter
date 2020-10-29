// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GatheringVein.h"
#include "CopperVein.generated.h"

/**
 * 
 */
UCLASS()
class GATHERCRAFTER_API ACopperVein : public AGatheringVein
{
	GENERATED_BODY()
	
public:

	ACopperVein();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
private:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:

};
