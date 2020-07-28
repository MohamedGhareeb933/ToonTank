// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "ToonTanks/GameMode/TankGameModeBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = DefaultHealth;

	GameModeRef = Cast<ATankGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	//Projectile Base Has OnHit Function and AddDamage Function That Cause Damage
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::TakeDamage);
}

void UHealthComponent::TakeDamage(AActor* DamagedActor, float BaseDamage, const UDamageType* DamageTypeClass, AController* EventInstigator, AActor* DamageCauser)
{
	if (BaseDamage == 0 || Health <= 0) return;

	Health = FMath::Clamp(Health - BaseDamage, 0.0f, DefaultHealth);
	UE_LOG(LogTemp, Warning, TEXT("Remaining Health is &f"), Health)

	if (Health <= 0)
	{
		if (GameModeRef)
		{
			GameModeRef->ActorDied(GetOwner());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No Valid Refrence to Game Mode "));
		}
	}
}
