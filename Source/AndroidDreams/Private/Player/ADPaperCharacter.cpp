// Fill out your copyright notice in the Description page of Project Settings.
#include "Player/ADPaperCharacter.h"

#include "PaperFlipbookComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"

AADPaperCharacter::AADPaperCharacter()
{
    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
    SpringArmComponent->SetupAttachment(GetRootComponent());
    SpringArmComponent->TargetArmLength = 500.f;
    SpringArmComponent->bInheritPitch = false;
    SpringArmComponent->bInheritYaw = false;
    SpringArmComponent->bInheritRoll = false;

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->SetupAttachment(SpringArmComponent);
    JumpMaxCount = 2;
}

void AADPaperCharacter::BeginPlay()
{
    Super::BeginPlay();
    OnCharacterMovementUpdated.AddUniqueDynamic(this, &AADPaperCharacter::Animate);
}

void AADPaperCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

void AADPaperCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis("MoveForward", this, &AADPaperCharacter::MoveForward);
    PlayerInputComponent->BindAction("Flip", IE_Pressed, this, &AADPaperCharacter::FlipStart);
    PlayerInputComponent->BindAction("Flip", IE_Released, this, &AADPaperCharacter::FlipStop);
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AADPaperCharacter::Jump);
}

void AADPaperCharacter::FlipStart()
{
    IsFlipButtonPushed = true;
}

void AADPaperCharacter::FlipStop()
{
    IsFlipButtonPushed = false;
}

void AADPaperCharacter::MoveForward(float Amount)
{
    Amount = JumpCurrentCount > 0 ? Amount : 1.0f;
    AddMovementInput(GetActorForwardVector(), Amount);
}

void AADPaperCharacter::Jump()
{
    if (JumpCurrentCount <= 1)
    {
        ACharacter::LaunchCharacter(FVector(0, 0, JumpHeight), false, true);
        ++JumpCurrentCount;
    }

    bPressedJump = true;
    JumpKeyHoldTime = 0.0f;
}

void AADPaperCharacter::SetCurrentAnimationDirection()
{
    if (IsFlipButtonPushed && JumpCurrentCount > 0)
    {
        CurrentAnimationDirection = EAnimationDirection::Flip;
    }
    else if (JumpCurrentCount == 1)
    {
        CurrentAnimationDirection = EAnimationDirection::Jump;
    }
    else if (JumpCurrentCount == 2)
    {
        CurrentAnimationDirection = EAnimationDirection::DoubleJump;
    }
    else if (IsMoving)
    {
        CurrentAnimationDirection = EAnimationDirection::Move;
    }
    else
    {
        CurrentAnimationDirection = EAnimationDirection::Idle;
    }
}

void AADPaperCharacter::Animate(float DeltaTime, FVector OldLocation, FVector const OldVelocity)
{
    SetCurrentAnimationDirection();
    switch (CurrentAnimationDirection)
    {
        case EAnimationDirection::Move: GetSprite()->SetFlipbook(Flipbooks.AnimMove); break;
        case EAnimationDirection::Jump: GetSprite()->SetFlipbook(Flipbooks.AnimJump); break;
        case EAnimationDirection::DoubleJump: GetSprite()->SetFlipbook(Flipbooks.AnimDoubleJump); break;
        case EAnimationDirection::Flip: GetSprite()->SetFlipbook(Flipbooks.AnimFlip); break;
        case EAnimationDirection::Idle: GetSprite()->SetFlipbook(Flipbooks.AnimIdle); break;
        default: break;
    }
}