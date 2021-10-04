// Copyright Soren Gilbertson & Brady Herzog

#pragma once

#include "CoreMinimal.h"
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

// Loading screen helper functions
public:
	UFUNCTION(BlueprintCallable)
	static void PlayLoadingScreen(bool bPlayUntilStopped, float PlayTime);

	UFUNCTION(BlueprintCallable)
	static void StopLoadingScreen();
	
};
