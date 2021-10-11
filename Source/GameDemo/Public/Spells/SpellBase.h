// Copyright Soren Gilbertson & Brady Herzog

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "NiagaraComponent.h"
#include "GameDemo/GameDemo.h"
#include "SpellBase.generated.h"


UCLASS( Blueprintable, BlueprintType, ClassGroup=(Spells), meta=(BlueprintSpawnableComponent) )
class GAMEDEMO_API USpellBase : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USpellBase(const class FObjectInitializer& PCIP);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Called when a component is created (not loaded). This can happen in the editor or during gameplay
	virtual void OnComponentCreated() override;

	// Called before destroying the object.
	virtual void BeginDestroy() override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

protected:
	USkeletalMeshComponent* SkMesh;

	UStaticMeshComponent* StMesh;

	UNiagaraComponent* ParticleComp;
	
	// Class to use if this spell uses a projectile
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> ProjectileClass;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SkeletalMesh")
	USkeletalMesh* SkeletalMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SkeletalMesh", DisplayName = "Transform")
	FTransform SkMeshTransform;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StaticMesh")
	UStaticMesh* StaticMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StaticMesh", DisplayName = "Material")
	UMaterialInterface* StMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StaticMesh", DisplayName = "Transform")
	FTransform StMeshTransform;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Particles")
	class UNiagaraSystem* ParticleTemplate = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Particles", DisplayName = "Transform")
	FTransform ParticleTransform;

public:
	// User is trying to cast a spell. Should be bound to IE_Pressed
	UFUNCTION(BlueprintCallable)
	void StartSpellCast();

	// User is done casting a spell. Should be bound to IE_Released
	UFUNCTION(BlueprintCallable)
	void EndSpellCast();

	// Returns the power of the charged spell normalized to a range between 0-1
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetChargePower() const;

private:
	// Logic for deciding whether to actually fire the spell with Fire()
	void TryFire();

	// Logic for deciding whether to charge the spell with Charge()
	void TryCharge();

protected:
	// Starts charging the spell
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Charge();

	// Called when the charge reaches it's maximum as defined by MaxChargeTime
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ChargeFull();

	// Casts the spell
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Fire();

	// Controls whether the spell charges up before firing
	UPROPERTY(EditDefaultsOnly)
	bool bDoesCharge = false;

	// When true, holding down the fire input will repeatedly fire with interval CooldownTime
	UPROPERTY(EditDefaultsOnly)
	bool bDoesRepeat = false;

	// Timer used to delay time between casts of the spell
	UPROPERTY(BlueprintReadOnly)
	FTimerHandle CooldownTimer;

	// Timer used to "charge up" a spell before casting it
	UPROPERTY(BlueprintReadOnly)
	FTimerHandle ChargeTimer;

private:
	// How long the player has to wait in between casts of the spell
	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "0"))
	float CooldownTime = 1.f;

	//  Sets the maximum time a player can charge a spell until it gets to "full charge".
	// However, if both bDoesCharge and bDoesRepeat are true then this controls how long to
	// charge the spell before automatically casting it on repeat.
	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "0", EditCondition = "bDoesCharge"))
	float MaxChargeTime = 1.f;

	// True when the player is holding down the cast input
	bool bCasting;

	// True when the charge has reached MaxChargeTime
	bool bFullCharge;

	// Helper methods for creating the visual components

	void CreateSkeletalMesh();
	void CreateStaticMesh();
	void CreateParticles();
};
