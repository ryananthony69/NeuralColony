#include "ColonySimulationSubsystem.h"

#include "ColonyGoalSaveGame.h"
#include "Kismet/GameplayStatics.h"

namespace NeuralColony
{
	constexpr TCHAR GoalSlotName[] = TEXT("NeuralColony_Goals");
	constexpr int32 GoalSlotUserIndex = 0;
	constexpr int32 MaxAlerts = 30;
}

void UColonySimulationSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	LoadGoals();
	SeedSimulationData();
}

void UColonySimulationSubsystem::Tick(float DeltaTime)
{
	const float SimDeltaSeconds = DeltaTime * GetTimeRateMultiplier();
	if (SimDeltaSeconds <= KINDA_SMALL_NUMBER)
	{
		return;
	}

	SimulateStep(SimDeltaSeconds);
	OnSimulationUpdated.Broadcast();
}

TStatId UColonySimulationSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UColonySimulationSubsystem, STATGROUP_Tickables);
}

bool UColonySimulationSubsystem::DoesSupportWorldType(const EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::Game || WorldType == EWorldType::PIE;
}

void UColonySimulationSubsystem::SetPlayerGoal(const FString& NewGoal)
{
	PlayerGoal = NewGoal;
	SaveGoals();
	OnSimulationUpdated.Broadcast();
}

void UColonySimulationSubsystem::SetNeuralNetworkGoal(const FString& NewGoal)
{
	NeuralNetworkGoal = NewGoal;
	SaveGoals();
	OnSimulationUpdated.Broadcast();
}

void UColonySimulationSubsystem::SetTimeRate(ESimulationTimeRate NewTimeRate)
{
	TimeRate = NewTimeRate;
	OnSimulationUpdated.Broadcast();
}

void UColonySimulationSubsystem::SeedSimulationData()
{
	if (!Nodes.IsEmpty() || !Projects.IsEmpty())
	{
		return;
	}

	FProjectRecord BootstrapProject;
	BootstrapProject.Name = TEXT("Bootstrap Settlement");
	BootstrapProject.State = EProjectState::InProgress;
	BootstrapProject.CurrentStage = TEXT("Foundation");
	Projects.Add(BootstrapProject);

	FNodeRecord Architect;
	Architect.DisplayName = TEXT("Ari-Prime");
	Architect.Role = ENodeRole::Architect;
	Architect.State = ENodeLifecycleState::Running;
	Architect.Generation = 0;
	Architect.Activity = TEXT("Interpreting colony goals");
	Architect.CurrentTask = TEXT("Create milestone-1 work orders");
	Architect.LinkedProject = BootstrapProject.Id;
	Architect.LastOutputSummary = TEXT("Issued setup directives");
	Nodes.Add(Architect);

	FNodeRecord Builder;
	Builder.DisplayName = TEXT("Bex-Builder");
	Builder.Role = ENodeRole::Builder;
	Builder.State = ENodeLifecycleState::Running;
	Builder.Generation = 1;
	Builder.Activity = TEXT("Assembling visible worksite");
	Builder.CurrentTask = TEXT("Construct colony plaza");
	Builder.ParentA = Architect.Id;
	Builder.LinkedProject = BootstrapProject.Id;
	Builder.LastOutputSummary = TEXT("Placed first structural supports");
	Nodes.Add(Builder);

	FNodeRecord Baby;
	Baby.DisplayName = TEXT("Nova-Spark");
	Baby.Role = ENodeRole::Researcher;
	Baby.State = ENodeLifecycleState::Baby;
	Baby.Generation = 2;
	Baby.Activity = TEXT("Learning from colony mentors");
	Baby.CurrentTask = TEXT("Observe builder routines");
	Baby.ParentA = Architect.Id;
	Baby.ParentB = Builder.Id;
	Baby.LastOutputSummary = TEXT("No output yet");
	Nodes.Add(Baby);

	Projects[0].AssignedNodes = { Architect.Id, Builder.Id };

	AddAlert(TEXT("Simulation booted in local mock mode."));
	AddAlert(TEXT("Initial baby node Nova-Spark spawned."));
}

void UColonySimulationSubsystem::SimulateStep(float SimDeltaSeconds)
{
	SimulationClockSeconds += SimDeltaSeconds;
	ConversationAccumulator += SimDeltaSeconds;
	StateShiftAccumulator += SimDeltaSeconds;
	BabySpawnAccumulator += SimDeltaSeconds;

	if (!Projects.IsEmpty())
	{
		FProjectRecord& Project = Projects[0];
		if (Project.State == EProjectState::InProgress)
		{
			Project.BuildProgress = FMath::Clamp(Project.BuildProgress + (0.02f * SimDeltaSeconds), 0.0f, 1.0f);
			if (Project.BuildProgress > 0.25f)
			{
				Project.CurrentStage = TEXT("Frame");
			}
			if (Project.BuildProgress > 0.7f)
			{
				Project.CurrentStage = TEXT("Validation");
			}
			if (Project.BuildProgress >= 1.0f)
			{
				Project.State = EProjectState::Complete;
				Project.CurrentStage = TEXT("Completed");
				AddAlert(TEXT("Project completed: Bootstrap Settlement."));
			}
		}
	}

	if (StateShiftAccumulator >= 4.0f)
	{
		StateShiftAccumulator = 0.0f;
		for (FNodeRecord& Node : Nodes)
		{
			switch (Node.State)
			{
			case ENodeLifecycleState::Baby:
				Node.State = ENodeLifecycleState::Idle;
				Node.Activity = TEXT("Ready for first assignment");
				AddAlert(FString::Printf(TEXT("Node matured from baby state: %s."), *Node.DisplayName));
				break;
			case ENodeLifecycleState::Idle:
				Node.State = ENodeLifecycleState::Running;
				Node.Activity = TEXT("Executing delegated task");
				Node.Load = 0.6f;
				break;
			case ENodeLifecycleState::Running:
				Node.State = ENodeLifecycleState::Complete;
				Node.Activity = TEXT("Work packet delivered");
				Node.LastOutputSummary = TEXT("Output verified locally");
				Node.Load = 0.2f;
				break;
			case ENodeLifecycleState::Complete:
				Node.State = ENodeLifecycleState::Idle;
				Node.Activity = TEXT("Awaiting next instruction");
				break;
			default:
				break;
			}
		}
	}

	if (ConversationAccumulator >= 3.0f && Nodes.Num() >= 2)
	{
		ConversationAccumulator = 0.0f;
		const int32 SpeakerIndex = FMath::RandRange(0, Nodes.Num() - 1);
		const int32 ListenerIndex = (SpeakerIndex + 1) % Nodes.Num();
		AddAlert(FString::Printf(TEXT("%s -> %s: Passing local task context."), *Nodes[SpeakerIndex].DisplayName, *Nodes[ListenerIndex].DisplayName));
	}

	if (ShouldAutoSpawnBabyNode())
	{
		BabySpawnAccumulator = 0.0f;
		const FNodeRecord& ParentA = Nodes[FMath::RandRange(0, Nodes.Num() - 1)];
		const FNodeRecord& ParentB = Nodes[FMath::RandRange(0, Nodes.Num() - 1)];

		FNodeRecord BabyNode;
		BabyNode.DisplayName = FString::Printf(TEXT("Sprout-%d"), Nodes.Num() + 1);
		BabyNode.Role = ENodeRole::Planner;
		BabyNode.State = ENodeLifecycleState::Baby;
		BabyNode.Generation = FMath::Max(ParentA.Generation, ParentB.Generation) + 1;
		BabyNode.ParentA = ParentA.Id;
		BabyNode.ParentB = ParentB.Id;
		BabyNode.Activity = TEXT("Newly spawned node learning colony patterns");
		BabyNode.CurrentTask = TEXT("Observe and imitate");
		Nodes.Add(BabyNode);

		AddAlert(FString::Printf(TEXT("New baby node born: %s."), *BabyNode.DisplayName));
	}
}

void UColonySimulationSubsystem::AddAlert(const FString& Message)
{
	FAlertRecord NewAlert;
	NewAlert.Timestamp = FDateTime::UtcNow();
	NewAlert.Message = Message;
	Alerts.Insert(NewAlert, 0);

	if (Alerts.Num() > NeuralColony::MaxAlerts)
	{
		Alerts.SetNum(NeuralColony::MaxAlerts);
	}

	OnAlertAdded.Broadcast(NewAlert);
}

void UColonySimulationSubsystem::SaveGoals() const
{
	UColonyGoalSaveGame* SaveGame = Cast<UColonyGoalSaveGame>(UGameplayStatics::CreateSaveGameObject(UColonyGoalSaveGame::StaticClass()));
	if (!SaveGame)
	{
		return;
	}

	SaveGame->PlayerGoal = PlayerGoal;
	SaveGame->NeuralNetworkGoal = NeuralNetworkGoal;
	UGameplayStatics::SaveGameToSlot(SaveGame, NeuralColony::GoalSlotName, NeuralColony::GoalSlotUserIndex);
}

void UColonySimulationSubsystem::LoadGoals()
{
	PlayerGoal = TEXT("Create a stable local simulation colony.");
	NeuralNetworkGoal = TEXT("Interpret goals, delegate tasks, and visibly build projects.");

	USaveGame* LoadedSave = UGameplayStatics::LoadGameFromSlot(NeuralColony::GoalSlotName, NeuralColony::GoalSlotUserIndex);
	const UColonyGoalSaveGame* GoalSave = Cast<UColonyGoalSaveGame>(LoadedSave);
	if (GoalSave)
	{
		PlayerGoal = GoalSave->PlayerGoal;
		NeuralNetworkGoal = GoalSave->NeuralNetworkGoal;
	}
}

float UColonySimulationSubsystem::GetTimeRateMultiplier() const
{
	switch (TimeRate)
	{
	case ESimulationTimeRate::Paused:
		return 0.0f;
	case ESimulationTimeRate::OneX:
		return 1.0f;
	case ESimulationTimeRate::TwoX:
		return 2.0f;
	case ESimulationTimeRate::ThreeX:
		return 3.0f;
	default:
		return 1.0f;
	}
}

bool UColonySimulationSubsystem::ShouldAutoSpawnBabyNode() const
{
	return BabySpawnAccumulator >= 18.0f && Nodes.Num() < 12;
}
