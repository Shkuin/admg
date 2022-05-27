// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "PaperFlipbook.h"
#include "ADPaperCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

UENUM(BlueprintType, Category = "Animation")
enum class EAnimationDirection : uint8
{
    Idle,
    Move,
    Jump,
    DoubleJump,
    Flip
};

USTRUCT(BlueprintType, Category = "Animation")
struct FAnimationFlipbooks
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UPaperFlipbook* AnimMove{};
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UPaperFlipbook* AnimIdle{};
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UPaperFlipbook* AnimJump{};
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UPaperFlipbook* AnimDoubleJump{};
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UPaperFlipbook* AnimFlip{};
};

UCLASS()
class ANDROIDDREAMS_API AADPaperCharacter : public APaperCharacter
{
    GENERATED_BODY()

public:
    AADPaperCharacter();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USpringArmComponent* SpringArmComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UCameraComponent* CameraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimationCharacter|Config")
    EAnimationDirection CurrentAnimationDirection = EAnimationDirection::Idle;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AnimationCharacter|Config")
    FAnimationFlipbooks Flipbooks;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimationCharacter|Config")
    bool IsMoving = true;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimationCharacter|Config")
    bool IsFlipButtonPushed = false;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AnimationCharacter|Config")
    float JumpHeight = 500.0f;

    UFUNCTION(BlueprintCallable, Category = "AnimationCharacter|Animation")
    void SetCurrentAnimationDirection();

    void BeginPlay() override;

public:
    void Tick(float DeltaSeconds) override;
    void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:
    UFUNCTION(BlueprintCallable, Category = "AnimationCharacter|Animation")
    void Animate(float DeltaTime, FVector OldLocation, FVector const OldVelocity);

    void MoveForward(float Amount);
    void Jump() override;
    void FlipStart();
    void FlipStop();
};
