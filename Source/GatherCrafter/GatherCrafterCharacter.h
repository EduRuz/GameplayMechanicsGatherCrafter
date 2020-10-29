// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "InGameMaterials.h" 
#include "GatheringVein.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GatherCrafterCharacter.generated.h"

UCLASS(config=Game)
class AGatherCrafterCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** Radius around the player to check if a gathering vein is in radius of the player*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* radiusSphere;

public:
	AGatherCrafterCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	/** Returns radius Sphere subobject **/
	FORCEINLINE class USphereComponent* GetradiusSphere() const { return radiusSphere; }


private:

	/**Gathering Variables*/
	float totalGatheringExp;
	int gatheringLevel;

	/**Crafting Variables*/
	float totalCraftingExp;
	int craftingLevel;

	FString errorMessage;
	int errorInt;
	bool showError;

	bool isHeld;
	void setIsHeld();

	void proccessExp(float expGained);
	void gatherFromVein();
	void initValues();

	void increaseProgress(float progressValue) { gatheringProgress = gatheringProgress + progressValue; }
	void resetProgress();

	int amountOfCopperOre;
	int amountOfIronOre;
	int amountOfTinOre;

	float currentExpTarget;
	
	bool levelUp;
	float newLevelExp;
	float newLevelExpTarget;
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Progress", Meta = (BlueprintProtected = "true"))
	float gatheringProgress;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Progress", Meta = (BlueprintProtected = "true"))
	float maxProgress = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Progress", Meta = (BlueprintProtected = "true"))
	FString currentVeinName;


	/**HudWidget for gathering Info*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Progress", Meta = (BlueprintProtected = "true"))
	TSubclassOf<class UUserWidget> gatheringHUDWidgetclass;

	UPROPERTY()
	class UUserWidget* gatheringWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Progress", Meta = (BlueprintProtected = "true"))
	TSubclassOf<class UUserWidget> inventoryHUDWidgetclass;

	UPROPERTY()
	class UUserWidget* inventoryWidget;

public: 

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintPure, Category = "Progress")
	float getMaxProgress() { return maxProgress; }

	UFUNCTION(BlueprintPure, Category = "Progress")
	float getGatheringProgress() { return gatheringProgress; }

	UFUNCTION(BlueprintPure, Category = "Gathering Vein")
	FString getCurrentVeinName() { return currentVeinName; }

	UFUNCTION(BlueprintPure, Category = "Gathering Vein")
	int getGatheringLevel() { return gatheringLevel; }

	UFUNCTION(BlueprintPure, Category = "Gathering Vein")
	float getTotalGatheringExp() { return totalGatheringExp; }

	UFUNCTION(BlueprintPure, Category = "Gathering Vein")
	float getCurrentExpTarget() { return currentExpTarget; }

	UFUNCTION(BlueprintPure, Category = "Inventory")
	int getAmountOfIronOre() { return amountOfIronOre; }

	UFUNCTION(BlueprintPure, Category = "Inventory")
	int getAmountOfTinOre() { return amountOfTinOre; }

	UFUNCTION(BlueprintPure, Category = "Inventory")
	int getAmountOfCopperOre() { return amountOfCopperOre; }

	UFUNCTION(BlueprintPure, Category = "Inventory")
	FString getErrorMessage() { return errorMessage; }

	UFUNCTION(BlueprintPure, Category = "Gathering Vein")
	float getNewLevelExp() { return newLevelExp; }

	UFUNCTION(BlueprintPure, Category = "Gathering Vein")
	float getNewLevelExpTarget() { return newLevelExpTarget; }
};

