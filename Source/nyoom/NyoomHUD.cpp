// Fill out your copyright notice in the Description page of Project Settings.

#include "NyoomHUD.h"

#include "Engine/Engine.h"

ANyoomHUD::ANyoomHUD() {
    static ConstructorHelpers::FClassFinder<UUserWidget> defHUD(TEXT("/Game/Nyoom/HUD/DefaultHUD"));
    defaultHUDClass = defHUD.Class;
};

void ANyoomHUD::PostInitializeComponents() {
    Super::PostInitializeComponents();

    auto* widget = CreateWidget<UUserWidget>(this->GetOwningPlayerController(), defaultHUDClass);
    if (widget) {
        widget->AddToPlayerScreen();
    }
}