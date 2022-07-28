// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"
#include "Tank.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

void ATower::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (IsInFireCondition())
    {
        RotateTurret(Tank->GetActorLocation());
    }
}

void ATower::BeginPlay()
{
    Super::BeginPlay();
    Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
    GetWorldTimerManager().SetTimer(FireRateRTimerHandle, this, &ATower::Fire, FireRate, true);
}

void ATower::HandleDestruction()
{
    Super::HandleDestruction();
    Destroy();
}

bool ATower::IsInFireCondition()
{
    if (Tank && Tank->IsHidden() == false)
    {
        float Distance = FVector::Dist(GetActorLocation(), Tank->GetActorLocation());
        if (Distance <= FireRange)
        {
            return true;
        }
    }
    return false;
}

void ATower::Fire()
{
    if (IsInFireCondition())
    {
        Super::Fire();
    }
}
