// Copyright Soren Gilbertson & Brady Herzog

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SpellMenu.generated.h"

/**
 * 
 */
UCLASS()
class GAMEDEMO_API USpellMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	class AFirstPersonCharacter* Player;
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OpenMenu();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void CloseMenu();
};
