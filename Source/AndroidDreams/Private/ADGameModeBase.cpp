// Fill out your copyright notice in the Description page of Project Settings.


#include "ADGameModeBase.h"
#include "Player/ADPaperCharacter.h"
#include "Player/ADPlayerController.h"

AADGameModeBase::AADGameModeBase()
{
    DefaultPawnClass = AADPaperCharacter::StaticClass();
    PlayerControllerClass = AADPlayerController::StaticClass();
}
