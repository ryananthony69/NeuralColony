#pragma once

#include "CoreMinimal.h"
#include "ColonySimulationTypes.generated.h"

UENUM(BlueprintType)
enum class ENodeLifecycleState : uint8
{
	Baby,
	Idle,
	Queued,
	Running,
	Complete,
	Retrying,
	Error
};

UENUM(BlueprintType)
enum class ENodeRole : uint8
{
	Architect,
	Planner,
	Builder,
	Verifier,
	Repairer,
	Researcher,
	UISpecialist,
	BackendSpecialist
};

UENUM(BlueprintType)
enum class EProjectState : uint8
{
	Queued,
	InProgress,
	Blocked,
	Complete,
	Failed
};

UENUM(BlueprintType)
enum class ESimulationTimeRate : uint8
{
	Paused,
	OneX,
	TwoX,
	ThreeX
};

USTRUCT(BlueprintType)
struct FNodeRecord
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FGuid Id = FGuid::NewGuid();

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FString DisplayName;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	ENodeRole Role = ENodeRole::Builder;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	ENodeLifecycleState State = ENodeLifecycleState::Idle;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FString Activity;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FGuid ParentA;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FGuid ParentB;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	int32 Generation = 0;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float Load = 0.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float Health = 100.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FString CurrentTask;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FGuid LinkedProject;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FString LastOutputSummary;
};

USTRUCT(BlueprintType)
struct FProjectRecord
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FGuid Id = FGuid::NewGuid();

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FString Name;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	EProjectState State = EProjectState::Queued;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float Health = 100.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FString CurrentStage;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float BuildProgress = 0.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FString FailureReason;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TArray<FGuid> AssignedNodes;
};

USTRUCT(BlueprintType)
struct FAlertRecord
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FDateTime Timestamp = FDateTime::UtcNow();

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FString Message;
};
