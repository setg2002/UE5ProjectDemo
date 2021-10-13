// Copyright Soren Gilbertson & Brady Herzog


#include "DemoGameInstance.h"
#include "LoadingScreen.h"
#include "Framework/Application/SlateApplication.h"


TSharedPtr<FInputModeDetector> UDemoGameInstance::InputDetector;

void UDemoGameInstance::Init()
{
	Super::Init();

}

void UDemoGameInstance::OnStart()
{
	Super::OnStart();

	if (!InputDetector.IsValid())
	{
		InputDetector = MakeShareable(new FInputModeDetector());
		FSlateApplication::Get().RegisterInputPreProcessor(InputDetector);
	}
}

void UDemoGameInstance::PlayLoadingScreen(bool bPlayUntilStopped, float PlayTime)
{
	ILoadingScreenModule& LoadingScreenModule = ILoadingScreenModule::Get();
	LoadingScreenModule.StartInGameLoadingScreen(bPlayUntilStopped, PlayTime);
}

void UDemoGameInstance::StopLoadingScreen()
{
	ILoadingScreenModule& LoadingScreenModule = ILoadingScreenModule::Get();
	LoadingScreenModule.StopInGameLoadingScreen();
}

EInputMode UDemoGameInstance::GetLastInputModeUsed() 
{ 
	return InputDetector->GetLastInputMode();
}

bool UDemoGameInstance::LastInputWasGamePad()
{
	return GetLastInputModeUsed() == EInputMode::Gamepad;
}
