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

    CurrentAnimationDirection = EAnimationDirection::Idle;
    IsMoving = false;
    IsFlipButtonPushed = false;
    JumpHeight = 500.0f;
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
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AADPaperCharacter::DoubleJump);
}

void AADPaperCharacter::Landed(const FHitResult& hit)
{
    DoubleJumpCount = 0;
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
    AddMovementInput(GetActorForwardVector(), Amount);
}

void AADPaperCharacter::DoubleJump()
{
    if (DoubleJumpCount <= 1)
    {
        ACharacter::LaunchCharacter(FVector(0, 0, JumpHeight), false, true);
        DoubleJumpCount++;
    }
}

void AADPaperCharacter::SetCurrentAnimationDirection(FVector const& Velocity)
{
    const float x = Velocity.GetSafeNormal().X;
    const float z = Velocity.GetSafeNormal().Z;

    IsMoving = x != 0.0f || z != 0.0f;

    if (IsMoving)
    {
        if (IsFlipButtonPushed)
        {
            CurrentAnimationDirection = EAnimationDirection::Flip;
        }
        else if (x > 0.0f && z == 0.0f)
        {
            CurrentAnimationDirection = EAnimationDirection::Right;
        }
        else if (x < 0.0f && z == 0.0f)
        {
            CurrentAnimationDirection = EAnimationDirection::Left;
        }
        else if (z != 0 && DoubleJumpCount != 1)
        {
            CurrentAnimationDirection = EAnimationDirection::DoubleJump;
        }
        else
        {
            CurrentAnimationDirection = EAnimationDirection::Jump;
        }
    }
}

void AADPaperCharacter::Animate(float DeltaTime, FVector OldLocation, FVector const OldVelocity)
{
    SetCurrentAnimationDirection(OldVelocity);

    if (OldVelocity.Size() > 0.0f)
    {
        switch (CurrentAnimationDirection)
        {
            case EAnimationDirection::Left: GetSprite()->SetFlipbook(Flipbooks.AnimLeft); break;
            case EAnimationDirection::Right: GetSprite()->SetFlipbook(Flipbooks.AnimRight); break;

            case EAnimationDirection::Jump: GetSprite()->SetFlipbook(Flipbooks.AnimJump); break;
            case EAnimationDirection::DoubleJump: GetSprite()->SetFlipbook(Flipbooks.AnimDoubleJump); break;
            case EAnimationDirection::Flip: GetSprite()->SetFlipbook(Flipbooks.AnimFlip); break;
            default: break;
        }
    }
    else
    {
        GetSprite()->SetFlipbook(Flipbooks.AnimStay);
    }
}