#include "ColonyHUDWidget.h"

#include "ColonySimulationSubsystem.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/HorizontalBox.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Blueprint/WidgetTree.h"

void UColonyHUDWidget::InitializeForSimulation(UColonySimulationSubsystem* InSubsystem)
{
	SimulationSubsystem = InSubsystem;
	RefreshAll();
}

void UColonyHUDWidget::SetSelectedNodeId(const FGuid& InSelectedNodeId)
{
	SelectedNodeId = InSelectedNodeId;
	RefreshAll();
}

void UColonyHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
	BuildWidgetTree();
	RefreshAll();
}

void UColonyHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	RefreshAccumulator += InDeltaTime;
	if (RefreshAccumulator >= 0.2f)
	{
		RefreshAccumulator = 0.0f;
		RefreshAll();
	}
}

void UColonyHUDWidget::BuildWidgetTree()
{
	if (WidgetTree->RootWidget)
	{
		return;
	}

	UOverlay* RootOverlay = WidgetTree->ConstructWidget<UOverlay>();
	WidgetTree->RootWidget = RootOverlay;

	UTextBlock* TopBar = WidgetTree->ConstructWidget<UTextBlock>();
	TopStatusText = TopBar;
	TopBar->SetColorAndOpacity(FSlateColor(FLinearColor::Yellow));
	RootOverlay->AddChildToOverlay(TopBar);

	UHorizontalBox* Columns = WidgetTree->ConstructWidget<UHorizontalBox>();
	UOverlaySlot* ColumnsSlot = RootOverlay->AddChildToOverlay(Columns);
	ColumnsSlot->SetPadding(FMargin(10.0f, 40.0f, 10.0f, 10.0f));

	UVerticalBox* LeftPanel = WidgetTree->ConstructWidget<UVerticalBox>();
	Columns->AddChildToHorizontalBox(LeftPanel);

	UTextBlock* LeftTitle = WidgetTree->ConstructWidget<UTextBlock>();
	LeftTitle->SetText(FText::FromString(TEXT("GOALS + TIME")));
	LeftPanel->AddChildToVerticalBox(LeftTitle);

	PlayerGoalTextBox = WidgetTree->ConstructWidget<UEditableTextBox>();
	PlayerGoalTextBox->SetHintText(FText::FromString(TEXT("My Goal")));
	PlayerGoalTextBox->OnTextCommitted.AddDynamic(this, &UColonyHUDWidget::HandlePlayerGoalCommitted);
	LeftPanel->AddChildToVerticalBox(PlayerGoalTextBox);

	NeuralGoalTextBox = WidgetTree->ConstructWidget<UEditableTextBox>();
	NeuralGoalTextBox->SetHintText(FText::FromString(TEXT("Neural Network Goal")));
	NeuralGoalTextBox->OnTextCommitted.AddDynamic(this, &UColonyHUDWidget::HandleNeuralGoalCommitted);
	LeftPanel->AddChildToVerticalBox(NeuralGoalTextBox);

	PauseButton = WidgetTree->ConstructWidget<UButton>();
	UTextBlock* PauseLabel = WidgetTree->ConstructWidget<UTextBlock>();
	PauseLabel->SetText(FText::FromString(TEXT("Pause")));
	PauseButton->AddChild(PauseLabel);
	PauseButton->OnClicked.AddDynamic(this, &UColonyHUDWidget::HandlePauseClicked);
	LeftPanel->AddChildToVerticalBox(PauseButton);

	OneXButton = WidgetTree->ConstructWidget<UButton>();
	UTextBlock* OneXLabel = WidgetTree->ConstructWidget<UTextBlock>();
	OneXLabel->SetText(FText::FromString(TEXT("1x")));
	OneXButton->AddChild(OneXLabel);
	OneXButton->OnClicked.AddDynamic(this, &UColonyHUDWidget::HandleOneXClicked);
	LeftPanel->AddChildToVerticalBox(OneXButton);

	TwoXButton = WidgetTree->ConstructWidget<UButton>();
	UTextBlock* TwoXLabel = WidgetTree->ConstructWidget<UTextBlock>();
	TwoXLabel->SetText(FText::FromString(TEXT("2x")));
	TwoXButton->AddChild(TwoXLabel);
	TwoXButton->OnClicked.AddDynamic(this, &UColonyHUDWidget::HandleTwoXClicked);
	LeftPanel->AddChildToVerticalBox(TwoXButton);

	ThreeXButton = WidgetTree->ConstructWidget<UButton>();
	UTextBlock* ThreeXLabel = WidgetTree->ConstructWidget<UTextBlock>();
	ThreeXLabel->SetText(FText::FromString(TEXT("3x")));
	ThreeXButton->AddChild(ThreeXLabel);
	ThreeXButton->OnClicked.AddDynamic(this, &UColonyHUDWidget::HandleThreeXClicked);
	LeftPanel->AddChildToVerticalBox(ThreeXButton);

	UVerticalBox* CenterPanel = WidgetTree->ConstructWidget<UVerticalBox>();
	UHorizontalBoxSlot* CenterSlot = Columns->AddChildToHorizontalBox(CenterPanel);
	CenterSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));

	ProjectText = WidgetTree->ConstructWidget<UTextBlock>();
	ProjectText->SetText(FText::FromString(TEXT("Project Progress")));
	CenterPanel->AddChildToVerticalBox(ProjectText);

	AlertsText = WidgetTree->ConstructWidget<UTextBlock>();
	AlertsText->SetText(FText::FromString(TEXT("Alerts")));
	CenterPanel->AddChildToVerticalBox(AlertsText);

	UVerticalBox* RightPanel = WidgetTree->ConstructWidget<UVerticalBox>();
	Columns->AddChildToHorizontalBox(RightPanel);

	UTextBlock* RightTitle = WidgetTree->ConstructWidget<UTextBlock>();
	RightTitle->SetText(FText::FromString(TEXT("INSPECTOR")));
	RightPanel->AddChildToVerticalBox(RightTitle);

	InspectorText = WidgetTree->ConstructWidget<UTextBlock>();
	InspectorText->SetText(FText::FromString(TEXT("Select a node")));
	RightPanel->AddChildToVerticalBox(InspectorText);
}

void UColonyHUDWidget::RefreshAll()
{
	if (!SimulationSubsystem)
	{
		return;
	}

	PlayerGoalTextBox->SetText(FText::FromString(SimulationSubsystem->GetPlayerGoal()));
	NeuralGoalTextBox->SetText(FText::FromString(SimulationSubsystem->GetNeuralNetworkGoal()));

	const TArray<FNodeRecord>& Nodes = SimulationSubsystem->GetNodes();
	const TArray<FProjectRecord>& Projects = SimulationSubsystem->GetProjects();
	const TArray<FAlertRecord>& Alerts = SimulationSubsystem->GetAlerts();

	int32 RunningCount = 0;
	for (const FNodeRecord& Node : Nodes)
	{
		if (Node.State == ENodeLifecycleState::Running)
		{
			++RunningCount;
		}
	}

	int32 BabyCount = 0;
	int32 ErrorCount = 0;
	for (const FNodeRecord& Node : Nodes)
	{
		if (Node.State == ENodeLifecycleState::Baby)
		{
			++BabyCount;
		}
		if (Node.State == ENodeLifecycleState::Error)
		{
			++ErrorCount;
		}
	}
	TopStatusText->SetText(FText::FromString(FString::Printf(TEXT("Nodes: %d | Running: %d | Babies: %d | Errors: %d | Alerts: %d"), Nodes.Num(), RunningCount, BabyCount, ErrorCount, Alerts.Num())));

	if (!Projects.IsEmpty())
	{
		const FProjectRecord& Project = Projects[0];
		ProjectText->SetText(FText::FromString(FString::Printf(TEXT("Project: %s\nStage: %s\nProgress: %.0f%%"), *Project.Name, *Project.CurrentStage, Project.BuildProgress * 100.0f)));
	}

	FString AlertLines;
	for (int32 Index = 0; Index < FMath::Min(8, Alerts.Num()); ++Index)
	{
		AlertLines += FString::Printf(TEXT("[%d] %s\n"), Index + 1, *Alerts[Index].Message);
	}
	AlertsText->SetText(FText::FromString(AlertLines));

	RefreshInspector(Nodes);
}

void UColonyHUDWidget::RefreshInspector(const TArray<FNodeRecord>& Nodes)
{
	for (const FNodeRecord& Node : Nodes)
	{
		if (Node.Id == SelectedNodeId)
		{
			InspectorText->SetText(FText::FromString(FString::Printf(TEXT("%s\nRole: %s\nState: %s\nActivity: %s\nGeneration: %d\nLoad: %.2f\nHealth: %.1f"),
				*Node.DisplayName,
				*StaticEnum<ENodeRole>()->GetNameStringByValue(static_cast<int64>(Node.Role)),
				*StaticEnum<ENodeLifecycleState>()->GetNameStringByValue(static_cast<int64>(Node.State)),
				*Node.Activity,
				Node.Generation,
				Node.Load,
				Node.Health)));
			return;
		}
	}
	InspectorText->SetText(FText::FromString(TEXT("Select a node in world.")));
}

void UColonyHUDWidget::HandlePlayerGoalCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (SimulationSubsystem)
	{
		SimulationSubsystem->SetPlayerGoal(Text.ToString());
	}
}

void UColonyHUDWidget::HandleNeuralGoalCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (SimulationSubsystem)
	{
		SimulationSubsystem->SetNeuralNetworkGoal(Text.ToString());
	}
}

void UColonyHUDWidget::HandlePauseClicked()
{
	if (SimulationSubsystem)
	{
		SimulationSubsystem->SetTimeRate(ESimulationTimeRate::Paused);
	}
}

void UColonyHUDWidget::HandleOneXClicked()
{
	if (SimulationSubsystem)
	{
		SimulationSubsystem->SetTimeRate(ESimulationTimeRate::OneX);
	}
}

void UColonyHUDWidget::HandleTwoXClicked()
{
	if (SimulationSubsystem)
	{
		SimulationSubsystem->SetTimeRate(ESimulationTimeRate::TwoX);
	}
}

void UColonyHUDWidget::HandleThreeXClicked()
{
	if (SimulationSubsystem)
	{
		SimulationSubsystem->SetTimeRate(ESimulationTimeRate::ThreeX);
	}
}
