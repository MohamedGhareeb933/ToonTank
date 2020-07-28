// Fill out your copyright notice in the Description page of Project Settings.


#include "TankGameModeBase.h"
#include "ToonTanks/Pawn/PawnTank.h"
#include "ToonTanks/Pawn/PawnTurret.h"
#include "ToonTanks/PlayerController/PlayerControllerBase.h"
#include "Kismet/GameplayStatics.h"


void ATankGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	HandleGameStart();
}

int32 ATankGameModeBase::GetRemainingTarget()
{
	TArray<AActor *> GetActorList;
	// passing APawnTurret As a Value in the Second Param As UClass::StaticClass.
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APawnTurret::StaticClass(), GetActorList);

	return GetActorList.Num();
}

void ATankGameModeBase::ActorDied(AActor * DeadActor)
{
	if (DeadActor == PlayerTank)
	{
		PlayerTank->HandleDestruction();
		HandleGameOver(false);

		if (PlayerControllerRef)
		{
			PlayerControllerRef->setPlayerEnablesState(false);
		}
	}
	else if (APawnTurret* TurretDestoryed = Cast<APawnTurret>(DeadActor))
	{
		TurretDestoryed->HandleDestruction();
		if (--TargetTurret == 0)
		{
			HandleGameOver(true);
		}
	}
}

void ATankGameModeBase::HandleGameStart()
{
	PlayerTank = Cast<APawnTank>(UGameplayStatics::GetPlayerPawn(this, 0));

	PlayerControllerRef = Cast<APlayerControllerBase>(UGameplayStatics::GetPlayerController(this, 0));

	TargetTurret = GetRemainingTarget();

	GameStart();

	if (PlayerControllerRef)
	{
		PlayerControllerRef->setPlayerEnablesState(false);

		FTimerHandle PlayerEnabledHandle;
		//creating delegate by ref to member function of type timer 
		// overriding setplayer to enablestate function to true after certain time which is gameloop = 2
		FTimerDelegate PlayerEnabledDelegate = FTimerDelegate::CreateUObject(PlayerControllerRef, &APlayerControllerBase::setPlayerEnablesState, true);
		GetWorld()->GetTimerManager().SetTimer(PlayerEnabledHandle, PlayerEnabledDelegate, GameLoop, false);
	}
}

void ATankGameModeBase::HandleGameOver(bool bPlayerWon)
{
	GameOver(bPlayerWon);
}
