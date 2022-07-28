// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonTanksGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Tank.h"
#include "Tower.h"
#include "ToonTanksPlayerController.h"

void AToonTanksGameMode::ActorDied(AActor* DeadActor)
{
    if (DeadActor == Tank)
    {
        Tank->HandleDestruction();
        if (PlayerController) 
        {
            PlayerController->SetPlayerEnabledState(false);
        }
        GameOver(false);
    }
    else if (ATower* DestroyedTower = Cast<ATower>(DeadActor))
    {
        DestroyedTower->HandleDestruction();
        TargetTowerCount--;
        if (TargetTowerCount == 0) GameOver(true);
    }

}

void AToonTanksGameMode::BeginPlay()
{
    Super::BeginPlay();
    HandleGameStart();
}

void AToonTanksGameMode::HandleGameStart()
{
    TargetTowerCount = GetTargetTowerCount();

    Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
    PlayerController = Cast<AToonTanksPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

    StartGame();

    if (PlayerController)
    {
        PlayerController->SetPlayerEnabledState(false);
        FTimerHandle PlayerEnableTimerHandle;
        FTimerDelegate PlayerEnabledTimerDelegate = FTimerDelegate::CreateUObject(
            PlayerController, 
            &AToonTanksPlayerController::SetPlayerEnabledState, 
            true
        );
        GetWorldTimerManager().SetTimer(
            PlayerEnableTimerHandle,
            PlayerEnabledTimerDelegate, 
            StartDelay,
            false
        );
    }
}

int32 AToonTanksGameMode::GetTargetTowerCount()
{
    TArray<AActor*> Towers;
    UGameplayStatics::GetAllActorsOfClass(this, ATower::StaticClass(), Towers);
    return Towers.Num();
}
