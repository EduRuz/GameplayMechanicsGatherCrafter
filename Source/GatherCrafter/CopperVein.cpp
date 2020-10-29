// Fill out your copyright notice in the Description page of Project Settings.

#include "CopperVein.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

ACopperVein::ACopperVein()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//Create the static mesh component
	gatheringVeinMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GatheringVeinMesh"));
	RootComponent = gatheringVeinMesh;

}

// Called when the game starts or when spawned
void ACopperVein::BeginPlay()
{
	Super::BeginPlay();

	isActive = true;

	changedMesh = false;

	respawnTime = 20.f;

	levelRequirement = 1;

	expYield = 15.f;

	name = FString(TEXT("Gathering from Copper Vein"));

	veinType = typeOfVein::Copper;
}

// Called every frame
void ACopperVein::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}