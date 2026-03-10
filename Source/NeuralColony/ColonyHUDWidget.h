#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ColonySimulationTypes.h"
#include "ColonyHUDWidget.generated.h"

class UEditableTextBox;
class UTextBlock;
class UScrollBox;
class UButton;
class UColonySimulationSubsystem;

UCLASS()
class NEURALCOLONY_API UColonyHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitializeForSimulation(UColonySimulationSubsystem* InSubsystem);
	void SetSelectedNodeId(const FGuid& InSelectedNodeId);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	void BuildWidgetTree();
	void RefreshAll();
	void RefreshInspector(const TArray<FNodeRecord>& Nodes);

	UFUNCTION()
	void HandlePlayerGoalCommitted(const FText& Text, ETextCommit::Type CommitMethod);

	UFUNCTION()
	void HandleNeuralGoalCommitted(const FText& Text, ETextCommit::Type CommitMethod);

	UFUNCTION()
	void HandlePauseClicked();

	UFUNCTION()
	void HandleOneXClicked();

	UFUNCTION()
	void HandleTwoXClicked();

	UFUNCTION()
	void HandleThreeXClicked();

private:
	UPROPERTY()
	UColonySimulationSubsystem* SimulationSubsystem;

	FGuid SelectedNodeId;
	float RefreshAccumulator = 0.0f;

	UPROPERTY()
	UEditableTextBox* PlayerGoalTextBox;

	UPROPERTY()
	UEditableTextBox* NeuralGoalTextBox;

	UPROPERTY()
	UTextBlock* TopStatusText;

	UPROPERTY()
	UTextBlock* InspectorText;

	UPROPERTY()
	UTextBlock* AlertsText;

	UPROPERTY()
	UTextBlock* ProjectText;

	UPROPERTY()
	UButton* PauseButton;

	UPROPERTY()
	UButton* OneXButton;

	UPROPERTY()
	UButton* TwoXButton;

	UPROPERTY()
	UButton* ThreeXButton;
};
