#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "ColonySimulationTypes.h"
#include "ColonySimulationSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSimulationUpdated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAlertAdded, const FAlertRecord&, Alert);

UCLASS()
class NEURALCOLONY_API UColonySimulationSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;
	virtual bool DoesSupportWorldType(const EWorldType::Type WorldType) const override;

	UFUNCTION(BlueprintPure, Category = "NeuralColony|Simulation")
	const TArray<FNodeRecord>& GetNodes() const { return Nodes; }

	UFUNCTION(BlueprintPure, Category = "NeuralColony|Simulation")
	const TArray<FProjectRecord>& GetProjects() const { return Projects; }

	UFUNCTION(BlueprintPure, Category = "NeuralColony|Simulation")
	const TArray<FAlertRecord>& GetAlerts() const { return Alerts; }

	UFUNCTION(BlueprintPure, Category = "NeuralColony|Simulation")
	FString GetPlayerGoal() const { return PlayerGoal; }

	UFUNCTION(BlueprintPure, Category = "NeuralColony|Simulation")
	FString GetNeuralNetworkGoal() const { return NeuralNetworkGoal; }

	UFUNCTION(BlueprintPure, Category = "NeuralColony|Simulation")
	ESimulationTimeRate GetTimeRate() const { return TimeRate; }

	UFUNCTION(BlueprintCallable, Category = "NeuralColony|Simulation")
	void SetPlayerGoal(const FString& NewGoal);

	UFUNCTION(BlueprintCallable, Category = "NeuralColony|Simulation")
	void SetNeuralNetworkGoal(const FString& NewGoal);

	UFUNCTION(BlueprintCallable, Category = "NeuralColony|Simulation")
	void SetTimeRate(ESimulationTimeRate NewTimeRate);

	UPROPERTY(BlueprintAssignable, Category = "NeuralColony|Simulation")
	FOnSimulationUpdated OnSimulationUpdated;

	UPROPERTY(BlueprintAssignable, Category = "NeuralColony|Simulation")
	FOnAlertAdded OnAlertAdded;

private:
	void SeedSimulationData();
	void SimulateStep(float SimDeltaSeconds);
	void AddAlert(const FString& Message);
	void SaveGoals() const;
	void LoadGoals();
	float GetTimeRateMultiplier() const;
	bool ShouldAutoSpawnBabyNode() const;

private:
	UPROPERTY()
	TArray<FNodeRecord> Nodes;

	UPROPERTY()
	TArray<FProjectRecord> Projects;

	UPROPERTY()
	TArray<FAlertRecord> Alerts;

	UPROPERTY()
	FString PlayerGoal;

	UPROPERTY()
	FString NeuralNetworkGoal;

	UPROPERTY()
	ESimulationTimeRate TimeRate = ESimulationTimeRate::OneX;

	float SimulationClockSeconds = 0.0f;
	float ConversationAccumulator = 0.0f;
	float StateShiftAccumulator = 0.0f;
	float BabySpawnAccumulator = 0.0f;
};
