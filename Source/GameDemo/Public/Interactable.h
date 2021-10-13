// Copyright Soren Gilbertson & Brady Herzog

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractable : public UInterface
{
	GENERATED_BODY()
};

/**
 *  Interface for interactable objects
 */
class GAMEDEMO_API IInteractable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void Interact(class AFirstPersonCharacter* Interactor);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	bool GetCanInteract();
};
