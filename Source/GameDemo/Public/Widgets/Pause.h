// Copyright Soren Gilbertson & Brady Herzog

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Pause.generated.h"

/**
 * 
 */
UCLASS()
class GAMEDEMO_API UPause : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OpenPauseScreen();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void ClosePauseScreen();
};
