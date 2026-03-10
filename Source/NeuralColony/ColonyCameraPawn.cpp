#include "ColonyCameraPawn.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

AColonyCameraPawn::AColonyCameraPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(Root);
	SpringArm->TargetArmLength = 2400.0f;
	SpringArm->SetRelativeRotation(FRotator(-60.0f, 0.0f, 0.0f));
	SpringArm->bDoCollisionTest = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

void AColonyCameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AColonyCameraPawn::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AColonyCameraPawn::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("Zoom"), this, &AColonyCameraPawn::Zoom);
}

void AColonyCameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const FVector MoveDelta = FVector(MoveInputForward, MoveInputRight, 0.0f) * MoveSpeed * DeltaTime;
	AddActorWorldOffset(MoveDelta, true);

	const float NewArmLength = FMath::Clamp(SpringArm->TargetArmLength + (-ZoomInput * ZoomSpeed * DeltaTime), MinZoom, MaxZoom);
	SpringArm->TargetArmLength = NewArmLength;
}

void AColonyCameraPawn::MoveForward(float Value)
{
	MoveInputForward = Value;
}

void AColonyCameraPawn::MoveRight(float Value)
{
	MoveInputRight = Value;
}

void AColonyCameraPawn::Zoom(float Value)
{
	ZoomInput = Value;
}
