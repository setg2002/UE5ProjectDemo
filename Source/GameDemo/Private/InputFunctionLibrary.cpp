// Copyright Soren Gilbertson & Brady Herzog


#include "InputFunctionLibrary.h"
#include "GameFramework/InputSettings.h"


void UInputFunctionLibrary::IsKeyInputAction(const FKeyEvent& InKeyEvent, FName InActionName, EInputAction& Branches)
{
	TArray<FInputActionKeyMapping> ActionMappings;
	UInputSettings::GetInputSettings()->GetActionMappingByName(InActionName, ActionMappings);
	for (FInputActionKeyMapping Input : ActionMappings)
	{
		if (Input.Key == InKeyEvent.GetKey())
		{
			Branches = EInputAction::IsInput;
			return;
		}
	}
	Branches = EInputAction::IsNotInput;
}
