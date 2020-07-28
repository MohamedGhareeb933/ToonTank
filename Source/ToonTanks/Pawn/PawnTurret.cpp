// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnTurret.h"
#include "Kismet/GameplayStatics.h"
#include "PawnTank.h"

void APawnTurret::BeginPlay()
{
	Super::BeginPlay();

	// Creating Custom Timer That Called Function Every 2 Second
	GetWorld()->GetTimerManager().SetTimer(FireRateTimer, this, &APawnTurret::CheckFireCondition, FireRate, true);

	// Casting PawnTank Refrence Into GetPlayer At given Index. which is 0 same as the Editor
	GetPlayerPawnTank = Cast<APawnTank>(UGameplayStatics::GetPlayerPawn(this, 0));
}

void APawnTurret::HandleDestruction()
{
	Super::HandleDestruction();
	Destroy();
}

void APawnTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!GetPlayerPawnTank || ReturnDistanceToPlayer() > DistanceRange) { return; }

	RotateTurretFunction(GetPlayerPawnTank->GetActorLocation());
}

void APawnTurret::CheckFireCondition() 
{
	if (!GetPlayerPawnTank || !GetPlayerPawnTank->GetIsPlayerAlive()) { return; }


	if (ReturnDistanceToPlayer() <= DistanceRange)
	{
		Fire();
	}
}


float APawnTurret::ReturnDistanceToPlayer()
{
	if (!GetPlayerPawnTank) return 0.0f;

	return FVector::Dist(GetPlayerPawnTank->GetActorLocation(), GetActorLocation());
}
