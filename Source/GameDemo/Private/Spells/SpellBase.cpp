// Copyright Soren Gilbertson & Brady Herzog


#include "Spells/SpellBase.h"

// Sets default values for this component's properties
USpellBase::USpellBase(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

}

// Called when the game starts
void USpellBase::BeginPlay()
{
	Super::BeginPlay();

}

void USpellBase::OnComponentCreated()
{
	Super::OnComponentCreated();

	if (GetOwner())
	{
		// Only create the required components
		if (StaticMesh)
			CreateStaticMesh();
		if (SkeletalMesh)
			CreateSkeletalMesh();
		if (ParticleTemplate)
			CreateParticles();
	}
}

void USpellBase::BeginDestroy()
{
	// Destroy all components
	if (GetOwner())
	{
		if (SkMesh)
		{
			SkMesh->UnregisterComponent();
			SkMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
			SkMesh->DestroyComponent();
			SkMesh = nullptr;
		}

		if (StMesh)
		{
			StMesh->UnregisterComponent();
			StMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
			StMesh->DestroyComponent();
			StMesh = nullptr;
		}

		if (ParticleComp)
		{
			ParticleComp->UnregisterComponent();
			ParticleComp->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
			ParticleComp->DestroyComponent();
			ParticleComp = nullptr;
		}
	}

	Super::BeginDestroy();
}

// Called every frame
/*void USpellBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}*/

#define TimerManager GetWorld()->GetTimerManager()

void USpellBase::StartSpellCast()
{
	bCasting = true;
	UE_LOG(LogSpell, Display, TEXT("Spell (%s) cast started"), *this->GetFName().ToString());

	if (bDoesCharge)
		TryCharge();
	else
		TryFire();
}

void USpellBase::EndSpellCast()
{
	bCasting = false;
	UE_LOG(LogSpell, Display, TEXT("Spell (%s) cast ended"), *this->GetFName().ToString());

	if (bDoesCharge)
	{
		TryFire();
		TimerManager.ClearTimer(ChargeTimer);
	}
}

void USpellBase::TryCharge()
{
	if (!TimerManager.IsTimerActive(CooldownTimer) || TimerManager.GetTimerRemaining(CooldownTimer) == 0 && bCasting)
	{
		Charge();
	}
	// This might not be necessary if we end up not wanting chargable spells to be queued up for casting while on cooldown
	else
	{
		float RemainingTime = TimerManager.GetTimerRemaining(CooldownTimer);
		TimerManager.SetTimer(CooldownTimer, this, &USpellBase::TryCharge, RemainingTime);
	}
}

void USpellBase::Charge_Implementation()
{
	UE_LOG(LogSpell, Display, TEXT("Spell (%s) charging"), *this->GetFName().ToString());
	TimerManager.SetTimer(ChargeTimer, this, &USpellBase::ChargeFull, MaxChargeTime);
}

void USpellBase::ChargeFull_Implementation()
{ 
	UE_LOG(LogSpell, Display, TEXT("Spell (%s) max charge reached"), *this->GetFName().ToString());
	if (bDoesRepeat) TryFire(); 
}

void USpellBase::TryFire()
{
	if (!TimerManager.IsTimerActive(CooldownTimer) || TimerManager.GetTimerRemaining(CooldownTimer) == 0)
	{
		if (bCasting && bDoesRepeat && !bDoesCharge)
			TimerManager.SetTimer(CooldownTimer, this, &USpellBase::TryFire, CooldownTime);
		else if (bCasting && bDoesRepeat && bDoesCharge)
			TimerManager.SetTimer(CooldownTimer, this, &USpellBase::TryCharge, CooldownTime);
		else if (bDoesCharge || bCasting)
			TimerManager.SetTimer(CooldownTimer, CooldownTime, false);
		else return;
		UE_LOG(LogSpell, Display, TEXT("Spell (%s) fired"), *this->GetFName().ToString());
		Fire();
	}
	else if (bCasting && bDoesRepeat)
	{
		float RemainingTime = TimerManager.GetTimerRemaining(CooldownTimer);
		if (!bDoesCharge)
			TimerManager.SetTimer(CooldownTimer, this, &USpellBase::TryFire, RemainingTime);
		else
			TimerManager.SetTimer(CooldownTimer, this, &USpellBase::TryCharge, RemainingTime);
		return;
	}
	else return;
}

void USpellBase::Fire_Implementation()
{
}

#undef TimerManager


#if WITH_EDITOR
void USpellBase::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property != nullptr)
	{
		const FName PropertyName(PropertyChangedEvent.Property->GetName());

		if (GetOwner())
		{
			if (PropertyName == GET_MEMBER_NAME_CHECKED(USpellBase, StaticMesh))
			{
				if (StaticMesh)
				{
					if (StMesh)
					{
						StMesh->SetStaticMesh(StaticMesh);
						if (StMaterial) StMesh->SetMaterial(0, StMaterial);
					}
					else
						CreateStaticMesh();
				}
				else if (StMesh) 
				{
					StMesh->UnregisterComponent();
					StMesh->DestroyComponent();
					StMesh = nullptr;
				}
			}
			if (PropertyName == GET_MEMBER_NAME_CHECKED(USpellBase, SkeletalMesh))
			{
				if (SkeletalMesh)
				{
					if (SkMesh)
						SkMesh->SetSkeletalMesh(SkeletalMesh);
					else
						CreateSkeletalMesh();
				}
				else if (SkMesh)
				{
					SkMesh->UnregisterComponent();
					SkMesh->DestroyComponent();
					SkMesh = nullptr;
				}
			}
			if (PropertyName == GET_MEMBER_NAME_CHECKED(USpellBase, ParticleTemplate))
			{
				if (ParticleTemplate)
				{
					if (ParticleComp)
					{
						ParticleComp->SetAsset(ParticleTemplate);
						ParticleComp->ReinitializeSystem();
					}
					else
						CreateParticles();
				}
				else if (ParticleComp)
				{
					ParticleComp->UnregisterComponent();
					ParticleComp->DestroyComponent();
					ParticleComp = nullptr;
				}
			}

			if (PropertyName == GET_MEMBER_NAME_CHECKED(USpellBase, StMeshTransform) && StMesh)
				StMesh->SetRelativeTransform(StMeshTransform);
			if (PropertyName == GET_MEMBER_NAME_CHECKED(USpellBase, SkMeshTransform) && SkMesh)
				SkMesh->SetRelativeTransform(SkMeshTransform);
			if (PropertyName == GET_MEMBER_NAME_CHECKED(USpellBase, ParticleTransform) && ParticleComp)
				ParticleComp->SetRelativeTransform(ParticleTransform);

			if (PropertyName == GET_MEMBER_NAME_CHECKED(USpellBase, StMaterial) && StMesh)
				StMesh->SetMaterial(0, StMaterial);
		}
	}
}
#endif

void USpellBase::CreateSkeletalMesh()
{
	if (SkMesh) 
	{ 
		SkMesh->UnregisterComponent(); 
		SkMesh->DestroyComponent();
	}
	SkMesh = NewObject<USkeletalMeshComponent>(this, "SpellSkeletalMesh", RF_Public);
#if WITH_EDITOR
	SkMesh->CreationMethod = EComponentCreationMethod::Native;
#endif
	SkMesh->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	SkMesh->OnComponentCreated();
	SkMesh->SetSkeletalMesh(SkeletalMesh);
	SkMesh->SetRelativeTransform(SkMeshTransform);
	SkMesh->RegisterComponent();
}

void USpellBase::CreateStaticMesh()
{
	if (StMesh)
	{
		StMesh->UnregisterComponent();
		StMesh->DestroyComponent();
	}
	StMesh = NewObject<UStaticMeshComponent>(this, "SpellStaticMesh", RF_Public);
#if WITH_EDITOR
	StMesh->CreationMethod = EComponentCreationMethod::Native;
#endif
	StMesh->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	StMesh->OnComponentCreated();
	StMesh->SetStaticMesh(StaticMesh);
	StMesh->SetRelativeTransform(StMeshTransform);
	if (StMaterial) StMesh->SetMaterial(0, StMaterial);
	StMesh->RegisterComponent();
}

void USpellBase::CreateParticles()
{
	if (ParticleComp)
	{
		ParticleComp->UnregisterComponent();
		ParticleComp->DestroyComponent();
	}
	ParticleComp = NewObject<UNiagaraComponent>(this, "SpellParticles", RF_Public);
#if WITH_EDITOR
	ParticleComp->CreationMethod = EComponentCreationMethod::Native;
#endif
	ParticleComp->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	ParticleComp->OnComponentCreated();
	ParticleComp->SetAsset(ParticleTemplate);
	ParticleComp->SetRelativeTransform(ParticleTransform);
	ParticleComp->ReinitializeSystem();
	ParticleComp->RegisterComponent();
}
