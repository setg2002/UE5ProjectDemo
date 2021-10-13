// Copyright Soren Gilbertson & Brady Herzog

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "GameFramework/Character.h"
#include "FirstPersonCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class USpellBase;

UCLASS(config = Game)
class AFirstPersonCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* Mesh1P;

	/** Location on the actor where spell components are created. */
	UPROPERTY(VisibleDefaultsOnly, Category = Spells)
	USceneComponent* FP_SpellLocation;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

public:
	AFirstPersonCharacter();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION()
	void Fire();

	UFUNCTION()
	void EndFire();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }


// ===== Interaction =====
public:
	UFUNCTION(BlueprintCallable)
	void Interact() { if (CanInteract()) Interactable->Execute_Interact(Interactable.GetObject(), this); }

	// The distance in unreal units that the interaction line-trace checks from the player's camera 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float InteractionDistance = 100.f;

	FCollisionQueryParams InteractionCollisionParams;

protected:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool CanInteract() const { return Interactable.GetObject() ? Interactable->Execute_GetCanInteract(Interactable.GetObject()) : false; }

	// The current interactable object the player can interact with. Can be null
	TScriptInterface<class IInteractable> Interactable;


// ===== Spell =====
public:
	UFUNCTION(BlueprintCallable)
	void SetNewSpell(UClass* NewSpellClass);

	UFUNCTION(BlueprintCallable)
	USpellBase* GetEquippedSpell() const { return EquippedSpell; }

protected:
	UPROPERTY(BlueprintReadOnly, Category = Gameplay)
	USpellBase* EquippedSpell;
};
