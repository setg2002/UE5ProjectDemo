// Copyright Soren Gilbertson & Brady Herzog

#include "FirstPersonCharacter.h"
#include "Spells/SpellBase.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// AFirstPersonCharacter

AFirstPersonCharacter::AFirstPersonCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

	FP_SpellLocation = CreateDefaultSubobject<USceneComponent>(TEXT("SpellLocation"));
	FP_SpellLocation->SetupAttachment(RootComponent);
}

void AFirstPersonCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void AFirstPersonCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FHitResult OutHit;
	FVector Start = FirstPersonCameraComponent->GetComponentLocation();
	FVector ForwardVector = FirstPersonCameraComponent->GetForwardVector();
	FVector End = ((ForwardVector * InteractionDistance) + Start);
	FCollisionQueryParams CollisionParams;

	if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams))
	{
		if (OutHit.GetActor()->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
			Interactable = OutHit.GetActor();
		else
			Interactable = nullptr;
	}
	else
		Interactable = nullptr;
}

void AFirstPersonCharacter::Restart()
{
	Super::Restart();

}

//////////////////////////////////////////////////////////////////////////
// Input

void AFirstPersonCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind cast spell event
	InputComponent->BindAction("Fire", IE_Pressed, this, &AFirstPersonCharacter::Fire);
	InputComponent->BindAction("Fire", IE_Released, this, &AFirstPersonCharacter::EndFire);

	// Bind interaction event
	InputComponent->BindAction("Interact", IE_Pressed, this, &AFirstPersonCharacter::Interact);
}

void AFirstPersonCharacter::Fire()
{
	if (EquippedSpell)
		EquippedSpell->StartSpellCast();
}

void AFirstPersonCharacter::EndFire()
{
	if (EquippedSpell)
		EquippedSpell->EndSpellCast();
}

void AFirstPersonCharacter::SetNewSpell(UClass* NewSpellClass)
{
	// First delete the old spell component
	if (EquippedSpell)
	{
		//EquippedSpell->UnregisterComponent();
		EquippedSpell->ConditionalBeginDestroy();
		EquippedSpell->DestroyComponent();
	}

	FName SpellName = FName(NewSpellClass->GetFName().ToString() + FString::FromInt(FMath::Rand()));

	// Make the new spell component from the given class
	EquippedSpell = NewObject<USpellBase>(this, NewSpellClass, SpellName, RF_Public);
	EquippedSpell->AttachToComponent(FP_SpellLocation, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	EquippedSpell->SetRelativeTransform(FTransform(FRotator(0), FVector(0), FVector::OneVector));
	EquippedSpell->OnComponentCreated();
}
