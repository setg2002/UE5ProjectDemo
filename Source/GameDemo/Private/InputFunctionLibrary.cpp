// Copyright Soren Gilbertson & Brady Herzog


#include "InputFunctionLibrary.h"
#include "DemoGameInstance.h"
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

EInputMode UInputFunctionLibrary::GetLastInputModeUsed(const UObject* WorldContextObject)
{
	if (WorldContextObject)
	{
		return Cast<UDemoGameInstance>(WorldContextObject->GetWorld()->GetGameInstance())->GetInputDetector()->GetLastInputMode();
	}
	return EInputMode::Unknown;
}

bool UInputFunctionLibrary::LastInputWasGamePad(const UObject* WorldContextObject)
{
	return GetLastInputModeUsed(WorldContextObject) == EInputMode::Gamepad;
}
