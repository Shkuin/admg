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
    Left,
    Right,
    Jump,
    DoubleJump,
    Flip
};

USTRUCT(BlueprintType, Category = "Animation")
struct FAnimationFlipbooks
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UPaperFlipbook* AnimJump{};
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UPaperFlipbook* AnimLeft{};
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UPaperFlipbook* AnimRight{};
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UPaperFlipbook* AnimStay{};
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
    EAnimationDirection CurrentAnimationDirection;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AnimationCharacter|Config")
    FAnimationFlipbooks Flipbooks;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimationCharacter|Config")
    bool IsMoving;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimationCharacter|Config")
    bool IsFlipButtonPushed;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AnimationCharacter|Config")
    float JumpHeight;

    UFUNCTION(BlueprintCallable, Category = "AnimationCharacter|Animation")
    void SetCurrentAnimationDirection(FVector const& Velocity);

    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaSeconds) override;
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
    virtual void Landed(const FHitResult& hit) override;

private:
    UFUNCTION(BlueprintCallable, Category = "AnimationCharacter|Animation")
    void Animate(float DeltaTime, FVector OldLocation, FVector const OldVelocity);

    uint8 DoubleJumpCount;

    void MoveForward(float Amount);
    void DoubleJump();
    void FlipStart();
    void FlipStop();
};
