// Copyright Soren Gilbertson & Brady Herzog

#pragma once

#include "CoreMinimal.h"
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
};
