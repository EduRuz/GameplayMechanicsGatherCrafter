// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "GatherCrafterCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/World.h"
#include "Blueprint/UserWidget.h"

//////////////////////////////////////////////////////////////////////////
// AGatherCrafterCharacter

AGatherCrafterCharacter::AGatherCrafterCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	//Create the character sphere radius 
	radiusSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Radius sphere to check if vein is in character radius"));
	radiusSphere->SetupAttachment(RootComponent);
	radiusSphere->SetSphereRadius(200.f);
	
	initValues();

	PrimaryActorTick.bCanEverTick = true;


	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void AGatherCrafterCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	//F key for trying to gather from a vein
	PlayerInputComponent->BindAction("Gather", IE_Pressed, this, &AGatherCrafterCharacter::setIsHeld);
	PlayerInputComponent->BindAction("Gather", IE_Released, this, &AGatherCrafterCharacter::resetProgress);

	PlayerInputComponent->BindAxis("MoveForward", this, &AGatherCrafterCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AGatherCrafterCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AGatherCrafterCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AGatherCrafterCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AGatherCrafterCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AGatherCrafterCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AGatherCrafterCharacter::OnResetVR);



}

void AGatherCrafterCharacter::BeginPlay() 
{
	Super::BeginPlay();

	//Set up Gathering Hud
	if (gatheringHUDWidgetclass != nullptr) {
		UE_LOG(LogClass, Log, TEXT("gatheringHUDclass success"));
		gatheringWidget = CreateWidget<UUserWidget>(GetWorld(), gatheringHUDWidgetclass);
		if (gatheringWidget != nullptr) {
			UE_LOG(LogClass, Log, TEXT("GatheringWidget success"));
			gatheringWidget->AddToViewport();
			gatheringWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	//Set up Inventory Hud
	if (inventoryHUDWidgetclass != nullptr) {
		UE_LOG(LogClass, Log, TEXT("InventoryHudclass success"));
		inventoryWidget = CreateWidget<UUserWidget>(GetWorld(), inventoryHUDWidgetclass);
		if (inventoryWidget != nullptr) {
			UE_LOG(LogClass, Log, TEXT("InventoryWidget success"));
			inventoryWidget->AddToViewport();
		}
	}
}

// Called every frame
void AGatherCrafterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isHeld == true) {
		gatherFromVein();
	}
	else {
		showError = false;
		errorMessage = FString(TEXT(""));
	}
}

void AGatherCrafterCharacter::setIsHeld()
{
	isHeld = true;
}

void AGatherCrafterCharacter::proccessExp(float expGained)
{

	totalGatheringExp += expGained;
	newLevelExp += expGained;


	switch (gatheringLevel) {
	
	case 1:
		if ((totalGatheringExp) >= 50.0f) 
		{
			gatheringLevel = 2;
			currentExpTarget = 120.0f;
			newLevelExp = totalGatheringExp - 50.0f;
			newLevelExpTarget = 120.f - 50.f;
		}
		break;
	case 2:
		if ((totalGatheringExp) >= 120.f) 
		{
			gatheringLevel = 3;
			currentExpTarget = 250.0f;
			newLevelExp = totalGatheringExp - 120.f;
			newLevelExpTarget = 250.f - 120.f;
		}
		break;
	case 3:
		if ((totalGatheringExp) >= 250.f)
		{
			gatheringLevel = 4;
			currentExpTarget = 500.0f;
			newLevelExp = totalGatheringExp - 250.f;
			newLevelExpTarget = 500 - 250.f;
		}
		break;
	case 4:
		if ((totalGatheringExp) >= 500.0f)
		{
			gatheringLevel = 5;
			currentExpTarget = 1000.0f;
			newLevelExp = totalGatheringExp - 500.f;
			newLevelExpTarget = 1000.f - 500.f;
		}
		break;
	case 5:
		if ((totalGatheringExp) >= 1000.f)
		{
			gatheringLevel = 6;
			currentExpTarget = 2500.f;
			newLevelExp = totalGatheringExp - 1000.f;
			newLevelExpTarget = 2500.f - 1000.f;
		}
		break;
	case 6:
		if ((totalGatheringExp) >= 7500.f)
		{
			gatheringLevel = 7;
			currentExpTarget = 7500.f;
			newLevelExp = totalGatheringExp - 2500.f;
			newLevelExpTarget = 7500.f - 2500.f;
		}
		break;
	}

}


void AGatherCrafterCharacter::gatherFromVein()
{
	TArray<AActor*> collectedActors;
	radiusSphere->GetOverlappingActors(collectedActors);

	for (int32 i = 0; i < collectedActors.Num(); i++) {
	
		AGatheringVein* const currentVein = Cast<AGatheringVein>(collectedActors[i]);

		if (currentVein) {
			if (currentVein->getIsActive()) {
				if (currentVein->getLevelRequirement() <= gatheringLevel) {
					gatheringWidget->SetVisibility(ESlateVisibility::Visible);
					currentVeinName = currentVein->getVeinName();
					increaseProgress(1.0f);

					if (gatheringProgress >= maxProgress) {
						UE_LOG(LogClass, Log, TEXT("Gather Complete"));
						resetProgress();
						currentVein->setIsActive(false);
						currentVein->startRespawnTimer();
						proccessExp(currentVein->getExpYield());

						switch (currentVein->getTypeOfVein()) {

						case typeOfVein::Copper:
							amountOfCopperOre++;
							break;

						case typeOfVein::Tin:
							amountOfTinOre++;
							break;

						case typeOfVein::Iron:
							amountOfIronOre++;
							break;

						}
					}
				}
				else {
					showError = true;
					errorMessage = FString(TEXT("You dont meet the level Requirement"));

				}
			}
			else {
				showError = true;
				errorMessage = FString(TEXT("The Vein is currently Tapped"));
			}
		}

	}
}

void AGatherCrafterCharacter::initValues()
{
	totalGatheringExp = 0.f;
	gatheringLevel = 1;
	gatheringProgress = 0.f;

	currentExpTarget = 50.f;
	amountOfCopperOre = 0;
	amountOfIronOre = 0;
	amountOfTinOre = 0;

	totalCraftingExp = 0.f;
	craftingLevel = 0;

	newLevelExp = 0.f;
	newLevelExpTarget = 50.f;

	errorMessage = FString(TEXT(""));
	errorInt = 0;
	showError = false;
	isHeld = false;
}

void AGatherCrafterCharacter::resetProgress()
{
	isHeld = false;
	gatheringProgress = 0.f;
	gatheringWidget->SetVisibility(ESlateVisibility::Hidden);
}


void AGatherCrafterCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AGatherCrafterCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void AGatherCrafterCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void AGatherCrafterCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AGatherCrafterCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AGatherCrafterCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AGatherCrafterCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
