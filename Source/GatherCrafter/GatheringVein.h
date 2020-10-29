// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Runtime/Engine/Public/TimerManager.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GatheringVein.generated.h"

UENUM(BlueprintType)
enum class typeOfVein : uint8
{
	Iron UMETA(DisplayName = "Iron"),
	Copper UMETA(DisplayName = "Copper"),
	Tin UMETA(DisplayName = "Tin")

};

UCLASS()
class GATHERCRAFTER_API AGatheringVein : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGatheringVein();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Return Mesh for the Gathering Vein
	FORCEINLINE class UStaticMeshComponent* getMesh() const { return gatheringVeinMesh; }

	UFUNCTION(BlueprintPure, Category = "Gathering Vein")
	bool getIsActive() { return isActive; }

	UFUNCTION(BlueprintCallable, Category = "Gathering Vein")
	void setIsActive(bool flag) { isActive = flag; }

	UFUNCTION(BlueprintPure, Category = "Gathering Vein")
	float getExpYield() { return expYield; }

	UFUNCTION(BlueprintPure, Category = "Gathering Vein")
	int getLevelRequirement() { return levelRequirement; }

	UFUNCTION(BlueprintPure, Category = "Gathering Vein")
	FString getVeinName() { return name; }

	UFUNCTION()
	void startRespawnTimer();

	UFUNCTION(BlueprintPure, Category = "Gathering Vein")
	typeOfVein getTypeOfVein() { return veinType; }

private:

	
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	typeOfVein veinType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gathering Vein", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* gatheringVeinMesh;

	//Timer for tracking respawn
	FTimerManager respawnManager;
	FTimerHandle respawnTimer;

	UFUNCTION()
	void resetIsActive() { isActive = true; }

	//boolean whether the node can be Mined
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gathering Vein", meta = (AllowPrivateAccess = "true"))
	bool isActive;

	//float for amount of time for a node to respawn
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Gathering Vein", meta = (AllowPrivateAccess = "true"))
	float respawnTime;

	//Level requirement for a vein to be gathered from
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Gathering Vein", meta = (AllowPrivateAccess = "true"))
	int levelRequirement = 1;

	//The experience yield from a vein
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Gathering Vein", meta = (AllowPrivateAccess = "true"))
	float expYield;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gathering Vein", meta = (AllowPrivateAccess = "true"))
	FString name;

	bool changedMesh;
};
