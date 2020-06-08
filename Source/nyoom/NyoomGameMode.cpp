// Fill out your copyright notice in the Description page of Project Settings.

#include "NyoomGameMode.h"

#include "NyoomCharacter.h"

ANyoomGameMode::ANyoomGameMode() {
    DefaultPawnClass = ANyoomCharacter::StaticClass();
}