#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ColonyCameraPawn.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class NEURALCOLONY_API AColonyCameraPawn : public APawn
{
	GENERATED_BODY()

public:
	AColonyCameraPawn();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaTime) override;

private:
	void MoveForward(float Value);
	void MoveRight(float Value);
	void Zoom(float Value);

private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* Camera;

	float MoveInputForward = 0.0f;
	float MoveInputRight = 0.0f;
	float ZoomInput = 0.0f;

	UPROPERTY(EditAnywhere, Category = "NeuralColony|Camera")
	float MoveSpeed = 1200.0f;

	UPROPERTY(EditAnywhere, Category = "NeuralColony|Camera")
	float ZoomSpeed = 1800.0f;

	UPROPERTY(EditAnywhere, Category = "NeuralColony|Camera")
	float MinZoom = 1400.0f;

	UPROPERTY(EditAnywhere, Category = "NeuralColony|Camera")
	float MaxZoom = 4200.0f;
};
