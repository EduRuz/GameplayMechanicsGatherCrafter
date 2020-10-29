// Fill out your copyright notice in the Description page of Project Settings.

#include "GatheringVein.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AGatheringVein::AGatheringVein()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AGatheringVein::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGatheringVein::startRespawnTimer()
{
	if (isActive == false) {
		GetWorldTimerManager().ClearTimer(respawnTimer);
		GetWorldTimerManager().SetTimer(respawnTimer, this, &AGatheringVein::resetIsActive, respawnTime, false);

	}
}

// Called every frame
void AGatheringVein::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

