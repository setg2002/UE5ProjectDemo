// Copyright Soren Gilbertson & Brady Herzog


#include "DemoGameInstance.h"
#include "LoadingScreen.h"


void UDemoGameInstance::Init()
{
	Super::Init();

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
