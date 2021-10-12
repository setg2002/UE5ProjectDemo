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
class UAnimMontage;
class USoundBase;
class USpellBase;

UCLASS(config = Game)
class AFirstPersonCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* Mesh1P;

	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* FP_Gun;

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USceneComponent* FP_MuzzleLocation;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

public:
	AFirstPersonCharacter();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	// Called when the character is possessed. Used to set the OnAmmoChanged delegate
	virtual void Restart() override;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector GunOffset;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AActor> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* FireAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	USpellBase* EquippedSpell;

public:
	/** Fires a projectile. */
	UFUNCTION()
	void Fire();

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
	void Interact() { if (CanInteract()) Interactable->Execute_Interact(Interactable.GetObject()); }

	// The distance in unreal units that the interaction line-trace checks from the player's camera 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float InteractionDistance = 100.f;

protected:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool CanInteract() const { return Interactable.GetObject() ? Interactable->Execute_GetCanInteract(Interactable.GetObject()) : false; }

	// The current interactable object the player can interact with. Can be null
	TScriptInterface<class IInteractable> Interactable;
};
