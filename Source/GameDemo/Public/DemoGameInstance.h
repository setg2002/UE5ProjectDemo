// Copyright Soren Gilbertson & Brady Herzog

#pragma once

#include "CoreMinimal.h"
#include "InputModeDetector.h"
#include "Engine/GameInstance.h"
#include "DemoGameInstance.generated.h"


/**
 * 
 */
UCLASS()
class GAMEDEMO_API UDemoGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;

	virtual void OnStart() override;

// Loading screen helper functions
public:
	UFUNCTION(BlueprintCallable)
	static void PlayLoadingScreen(bool bPlayUntilStopped, float PlayTime);

	UFUNCTION(BlueprintCallable)
	static void StopLoadingScreen();
	
// Input helpers
protected:
	static TSharedPtr<FInputModeDetector> InputDetector;

public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static EInputMode GetLastInputModeUsed();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static bool LastInputWasGamePad();
};
