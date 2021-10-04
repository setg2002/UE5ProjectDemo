// Copyright Epic Games, Inc. All Rights Reserved.

#include "LoadingScreen.h"
#include "SlateBasics.h"
#include "SlateExtras.h"
#include "MoviePlayer.h"
#include "Widgets/Images/SThrobber.h"

// This module must be loaded "PreLoadingScreen" in the .uproject file, otherwise it will not hook in time!
struct FLoadingScreenBrush : public FSlateDynamicImageBrush, public FGCObject
{
	FLoadingScreenBrush(const FName InTextureName, const FVector2D& InImageSize)
		: FSlateDynamicImageBrush(InTextureName, InImageSize)
	{
		SetResourceObject(LoadObject<UObject>(NULL, *InTextureName.ToString()));
	}

	virtual void AddReferencedObjects(FReferenceCollector& Collector)
	{
		if (UObject* CachedResourceObject = GetResourceObject())
		{
			Collector.AddReferencedObject(CachedResourceObject);
		}
	}
};

#define LOCTEXT_NAMESPACE "Loading"

// Easy Font Macro
#define TTF_FONT( RelativePath, ... ) FSlateFontInfo( FPaths::ProjectConfigDir() / "Slate" / RelativePath + TEXT(".ttf"), __VA_ARGS__ ) // Straight from SGame 
#define OTF_FONT( RelativePath, ... ) FSlateFontInfo( FPaths::ProjectConfigDir() / "Slate" / RelativePath + TEXT(".otf"), __VA_ARGS__ ) 


class SLoadingScreen : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SLoadingScreen) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs)
	{
		// Load version of the logo with text baked in, path is hardcoded because this loads very early in startup
		//static const FName LoadingScreenName(TEXT("/Game/UI/Textures/T_GameIcon.T_GameIcon"));

		//LoadingScreenBrush = MakeShareable(new FLoadingScreenBrush(LoadingScreenName, FVector2D(256, 256)));
		
		FSlateBrush *BGBrush = new FSlateBrush();
		BGBrush->TintColor = FLinearColor(0.0f, 0.0f, 0.0f, 1.0f);

		FSlateBrush* LetterBoxBrush = new FSlateBrush();
		LetterBoxBrush->TintColor = FLinearColor(0.1f, 0.1f, 0.1f, 0.2f);

		ChildSlot
		[
			SNew(SOverlay)
			
			// Background
			+ SOverlay::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SNew(SBorder)	
				.BorderImage(BGBrush)
			]

			// Main Image
			+SOverlay::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(SImage)
				.Image(LoadingScreenBrush.Get())
			]

			// Bottom Letter-box
			+SOverlay::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Bottom)
			[
				SNew(SOverlay)

				+ SOverlay::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				[
					SNew(SBorder)
					.BorderImage(LetterBoxBrush)
				]

				+ SOverlay::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Bottom)
				[
					SNew(SHorizontalBox)

					// Tip Text
					/*+SHorizontalBox::Slot()
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					.Padding(15, 15)
					[
						SNew(STextBlock)
						.AutoWrapText(true)
						.ColorAndOpacity(FLinearColor::White)
						.Font(TTF_FONT("cour", 16))
						.Text(FText::FromString("Tip: "))
					]*/

				// Loading Stage Text & Throbber
				+ SHorizontalBox::Slot()
					.Padding(4, 4)
					[
						SNew(SHorizontalBox)

						+ SHorizontalBox::Slot()
						.VAlign(VAlign_Bottom)
						.HAlign(HAlign_Right)
						[
							SNew(STextBlock)
							.Justification(ETextJustify::Right)
							.ColorAndOpacity(FLinearColor::White)
							.Font(TTF_FONT("cour", 14))
							.Text(FText::FromString("Press any key to continue"))
							.Visibility(this, &SLoadingScreen::NotGetLoadIndicatorVisibility)
						]

						+ SHorizontalBox::Slot()
						.VAlign(VAlign_Bottom)
						.HAlign(HAlign_Right)
						[
							SNew(SThrobber)
							.Visibility(this, &SLoadingScreen::GetLoadIndicatorVisibility)
						]
					]
				]
			]
		];
	}

private:
	/** Whether to show the ... indicator */
	EVisibility GetLoadIndicatorVisibility() const
	{
		return GetMoviePlayer()->IsLoadingFinished() ? EVisibility::Collapsed : EVisibility::Visible;
	}

	EVisibility NotGetLoadIndicatorVisibility() const
	{
		return GetMoviePlayer()->IsLoadingFinished() ? EVisibility::Visible : EVisibility::Collapsed;
	}
	
	/** Loading screen image brush */
	TSharedPtr<FSlateDynamicImageBrush> LoadingScreenBrush;
};

#undef TTF_FONT
#undef OTF_FONT

#undef LOCTEXT_NAMESPACE

class FLoadingScreenModule : public ILoadingScreenModule
{
public:
	virtual void StartupModule() override
	{
		// Force load for cooker reference
		//LoadObject<UObject>(nullptr, TEXT("/Game/UI/Textures/T_GameIcon.T_GameIcon") );

		if (IsMoviePlayerEnabled())
		{
			CreateScreen();
		}
	}
	
	virtual bool IsGameModule() const override
	{
		return true;
	}

	virtual void StartInGameLoadingScreen(bool bPlayUntilStopped, float PlayTime) override
	{
		FLoadingScreenAttributes LoadingScreen;
		LoadingScreen.bAutoCompleteWhenLoadingCompletes = !bPlayUntilStopped;
		LoadingScreen.bWaitForManualStop = bPlayUntilStopped;
		LoadingScreen.bAllowEngineTick = bPlayUntilStopped;
		LoadingScreen.MinimumLoadingScreenDisplayTime = PlayTime;
		LoadingScreen.WidgetLoadingScreen = SNew(SLoadingScreen);
		GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
	}

	virtual void StopInGameLoadingScreen() override
	{
		GetMoviePlayer()->StopMovie();
	}

	virtual void CreateScreen()
	{
		FLoadingScreenAttributes LoadingScreen;
		LoadingScreen.bAutoCompleteWhenLoadingCompletes = true;
		LoadingScreen.MinimumLoadingScreenDisplayTime = 0.1f;
		//TODO Replace this widget with a real startup-type screen
		LoadingScreen.WidgetLoadingScreen = FLoadingScreenAttributes::NewTestLoadingScreenWidget();
		GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
	}

};

IMPLEMENT_GAME_MODULE(FLoadingScreenModule, LoadingScreen);
