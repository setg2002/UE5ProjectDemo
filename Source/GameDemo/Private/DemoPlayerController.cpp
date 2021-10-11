// Copyright Soren Gilbertson & Brady Herzog


#include "DemoPlayerController.h"


ADemoPlayerController::ADemoPlayerController()
{
	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;
}

void ADemoPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// set up gameplay key bindings
	check(InputComponent);

	// Bind movement events
	InputComponent->BindAxis("MoveForward", this, &ADemoPlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ADemoPlayerController::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	InputComponent->BindAxis("Turn", this, &APlayerController::AddYawInput);
	InputComponent->BindAxis("TurnRate", this, &ADemoPlayerController::TurnAtRate);
	InputComponent->BindAxis("LookUp", this, &APlayerController::AddPitchInput);
	InputComponent->BindAxis("LookUpRate", this, &ADemoPlayerController::LookUpAtRate);
}


void ADemoPlayerController::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		GetPawn()->AddMovementInput(GetPawn()->GetActorForwardVector(), Value);
	}
}

void ADemoPlayerController::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		GetPawn()->AddMovementInput(GetPawn()->GetActorRightVector(), Value);
	}
}

void ADemoPlayerController::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ADemoPlayerController::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}