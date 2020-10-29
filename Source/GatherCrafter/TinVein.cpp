// Fill out your copyright notice in the Description page of Project Settings.

#include "TinVein.h"
#include "Components/StaticMeshComponent.h"

ATinVein::ATinVein()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//Create the static mesh component
	gatheringVeinMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GatheringVeinMesh"));
	RootComponent = gatheringVeinMesh;
}

// Called when the game starts or when spawned
void ATinVein::BeginPlay()
{
	Super::BeginPlay();

	isActive = true;

	respawnTime = 60.f;

	levelRequirement = 3;

	expYield = 45.f;

	name = FString(TEXT("Gathering from Tin Vein"));

	veinType = typeOfVein::Tin;
}

// Called every frame
void ATinVein::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}