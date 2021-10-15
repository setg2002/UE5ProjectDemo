// Copyright Soren Gilbertson & Brady Herzog

#pragma once

#include "CoreMinimal.h"
#include "InputModeDetector.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "InputFunctionLibrary.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EInputAction : uint8
{
	IsInput		UMETA(DisplayName = "KeyIsInput"),
	IsNotInput	UMETA(DisplayName = "KeyIsNotInput")
};

UCLASS()
class GAMEDEMO_API UInputFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// Returns whether or not the key event is assigned to the given action name
	UFUNCTION(BlueprintCallable, meta = (ExpandEnumAsExecs = "Branches"))
	static void IsKeyInputAction(const FKeyEvent& InKeyEvent, FName InActionName, EInputAction& Branches);

	// Returns the last input mode the player used
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (WorldContext = "WorldContextObject"))
	static EInputMode GetLastInputModeUsed(const UObject* WorldContextObject);

	// Returns true if the player's last input was from a gamepad, false otherwise.
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (WorldContext = "WorldContextObject"))
	static bool LastInputWasGamePad(const UObject* WorldContextObject);
};
