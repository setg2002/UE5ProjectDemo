// Copyright Soren Gilbertson & Brady Herzog

#include "InputModeDetector.h"
#include "Input/Events.h"

DEFINE_LOG_CATEGORY_STATIC(LogInputMode, Warning, All);

FInputModeDetector::FInputModeDetector()
{
    LastInputMode = EInputMode::Unknown;
}

FInputModeDetector::~FInputModeDetector()
{
}

bool FInputModeDetector::HandleKeyDownEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent)
{
    // Key down also registers for gamepad buttons
    ProcessKeyOrButton(InKeyEvent.GetKey());

    // Don't consume
    return false;
}

bool FInputModeDetector::HandleAnalogInputEvent(FSlateApplication& SlateApp,
    const FAnalogInputEvent& InAnalogInputEvent)
{
    if (InAnalogInputEvent.GetAnalogValue() > GamepadAxisThreshold)
        SetMode(EInputMode::Gamepad);
    // Don't consume
    return false;
}

bool FInputModeDetector::HandleMouseMoveEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent)
{
    FVector2D Dist = MouseEvent.GetScreenSpacePosition() - MouseEvent.GetLastScreenSpacePosition();
    if (FMath::Abs(Dist.X) > MouseMoveThreshold || FMath::Abs(Dist.Y) > MouseMoveThreshold)
    {
        SetMode(EInputMode::Mouse);
    }
    // Don't consume
    return false;
}

bool FInputModeDetector::HandleMouseButtonDownEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent)
{
    // We don't care which button
    SetMode(EInputMode::Mouse);
    // Don't consume
    return false;
}

bool FInputModeDetector::HandleMouseWheelOrGestureEvent(FSlateApplication& SlateApp, const FPointerEvent& InWheelEvent,
    const FPointerEvent* InGestureEvent)
{
    SetMode(EInputMode::Mouse);
    // Don't consume
    return false;
}

void FInputModeDetector::Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor)
{
    // Required, but do nothing
}

EInputMode FInputModeDetector::GetLastInputMode()
{
    return LastInputMode;
}

void FInputModeDetector::ProcessKeyOrButton(FKey Key)
{
    if (Key.IsGamepadKey())
    {
        SetMode(EInputMode::Gamepad);
    }
    else if (Key.IsMouseButton())
    {
        // Assuming mice don't have analog buttons!
        SetMode(EInputMode::Mouse);
    }
    else
    {
        // We assume anything that's not mouse and not gamepad is a keyboard
        // Assuming keyboards don't have analog buttons!
        SetMode(EInputMode::Keyboard);
    }
}

void FInputModeDetector::SetMode(EInputMode NewMode)
{
    if (NewMode != EInputMode::Unknown && NewMode != GetLastInputMode())
    {
        LastInputMode = NewMode;
        UE_LOG(LogInputMode, Log, TEXT("Input mode changed: %s"), *UEnum::GetValueAsString(NewMode));
    }
}
