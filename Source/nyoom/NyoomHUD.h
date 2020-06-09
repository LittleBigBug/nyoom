// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "NyoomHUD.generated.h"

UCLASS()
class NYOOM_API ANyoomHUD : public AHUD
{
	GENERATED_BODY()

public:
    // To find the HUD asset
    ANyoomHUD();

	// Add widget to screen
    void PostInitializeComponents() override;

private:
    // Default HUD class to add
    TSubclassOf<UUserWidget> defaultHUDClass;

};
