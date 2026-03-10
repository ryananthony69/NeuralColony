#include "ColonyPresentationManager.h"

#include "ColonyHUDWidget.h"
#include "ColonyNodeVisualActor.h"
#include "ColonyPlayerController.h"
#include "ColonyProjectSiteActor.h"
#include "ColonySimulationSubsystem.h"
#include "ColonyZoneVisualActor.h"
#include "Engine/StaticMeshActor.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

namespace
{
	struct FZoneLayout
	{
		FName Name;
		FString Label;
		FVector Center;
		FVector Size;
		FLinearColor Color;
		int32 Columns;
		float Spacing;
	};
}

AColonyPresentationManager::AColonyPresentationManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AColonyPresentationManager::BeginPlay()
{
	Super::BeginPlay();

	SimulationSubsystem = GetWorld()->GetSubsystem<UColonySimulationSubsystem>();
	if (!SimulationSubsystem)
	{
		return;
	}

	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (PC)
	{
		HUDWidget = CreateWidget<UColonyHUDWidget>(PC, UColonyHUDWidget::StaticClass());
		if (HUDWidget)
		{
			HUDWidget->AddToViewport();
			HUDWidget->InitializeForSimulation(SimulationSubsystem);
			if (AColonyPlayerController* ColonyPC = Cast<AColonyPlayerController>(PC))
			{
				ColonyPC->SetHUDWidget(HUDWidget);
			}
		}
	}

	AStaticMeshActor* Ground = GetWorld()->SpawnActor<AStaticMeshActor>(FVector(0.0f, 0.0f, -6.0f), FRotator::ZeroRotator);
	if (Ground)
	{
		UStaticMeshComponent* GroundMesh = Ground->GetStaticMeshComponent();
		GroundMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		UStaticMesh* PlaneMesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, TEXT("/Engine/BasicShapes/Plane.Plane")));
		if (PlaneMesh)
		{
			GroundMesh->SetStaticMesh(PlaneMesh);
			GroundMesh->SetWorldScale3D(FVector(55.0f, 55.0f, 1.0f));
			GroundMesh->SetVectorParameterValueOnMaterials(TEXT("Color"), FVector(0.08f, 0.10f, 0.11f));
		}
	}

	BuildColonyLayout();
	ProjectActor = GetWorld()->SpawnActor<AColonyProjectSiteActor>(ProjectCenter, FRotator::ZeroRotator);

	EnsureNodeActors();
	SyncNodeActors();
	SyncProjectActor();
}

void AColonyPresentationManager::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (!SimulationSubsystem)
	{
		return;
	}

	EnsureNodeActors();
	SyncNodeActors();
	SyncProjectActor();
	ApplyConversationBehavior();

	if (HUDWidget)
	{
		HUDWidget->SetSelectedNodeId(SelectedNodeId);
	}
}

void AColonyPresentationManager::BuildColonyLayout()
{
	const TArray<FZoneLayout> Zones = {
		{ TEXT("ArchitectCore"), TEXT("Architect Core"), ArchitectCoreCenter, FVector(9.0f, 7.0f, 1.0f), FLinearColor(0.43f, 0.61f, 1.0f), 3, 170.0f },
		{ TEXT("Nursery"), TEXT("Nursery"), NurseryCenter, FVector(9.0f, 7.0f, 1.0f), FLinearColor(0.98f, 0.83f, 0.39f), 3, 170.0f },
		{ TEXT("Workshop"), TEXT("Build Workshop"), WorkshopCenter, FVector(11.0f, 8.0f, 1.0f), FLinearColor(0.32f, 0.86f, 0.55f), 4, 180.0f },
		{ TEXT("Verification"), TEXT("Verification"), VerificationCenter, FVector(10.0f, 7.0f, 1.0f), FLinearColor(0.53f, 0.74f, 1.0f), 3, 170.0f },
		{ TEXT("Repair"), TEXT("Repair/Debug"), RepairCenter, FVector(10.0f, 7.0f, 1.0f), FLinearColor(1.0f, 0.43f, 0.34f), 3, 170.0f }
	};

	for (const FZoneLayout& Zone : Zones)
	{
		AColonyZoneVisualActor* ZoneActor = GetWorld()->SpawnActor<AColonyZoneVisualActor>(Zone.Center, FRotator::ZeroRotator);
		if (ZoneActor)
		{
			ZoneActor->InitializeZone(Zone.Label, FVector(Zone.Size.X * 100.0f, Zone.Size.Y * 100.0f, 0.0f), Zone.Color);
			ZoneActors.Add(ZoneActor);
		}
	}
}

void AColonyPresentationManager::EnsureNodeActors()
{
	const TArray<FNodeRecord>& Nodes = SimulationSubsystem->GetNodes();
	for (const FNodeRecord& Node : Nodes)
	{
		if (!NodeActors.Contains(Node.Id))
		{
			AColonyNodeVisualActor* NodeActor = GetWorld()->SpawnActor<AColonyNodeVisualActor>(NurseryCenter, FRotator::ZeroRotator);
			if (NodeActor)
			{
				NodeActor->OnNodeSelected.AddDynamic(this, &AColonyPresentationManager::HandleNodeSelected);
				NodeActors.Add(Node.Id, NodeActor);
				NodeActor->SetConversationText(TEXT("Spawned in Nursery"));
			}
		}
	}
}

void AColonyPresentationManager::SyncNodeActors()
{
	const TArray<FNodeRecord>& Nodes = SimulationSubsystem->GetNodes();
	for (int32 NodeIndex = 0; NodeIndex < Nodes.Num(); ++NodeIndex)
	{
		const FNodeRecord& Node = Nodes[NodeIndex];
		if (AColonyNodeVisualActor** NodeActorPtr = NodeActors.Find(Node.Id))
		{
			AColonyNodeVisualActor* NodeActor = *NodeActorPtr;
			NodeActor->ApplyNodeRecord(Node);
			NodeActor->SetSelected(Node.Id == SelectedNodeId);
			NodeActor->SetDesiredLocation(ResolveTargetLocationForNode(Node) + FVector((NodeIndex % 2) * 30.0f, (NodeIndex % 3) * 28.0f, 0.0f));
		}
	}
}

void AColonyPresentationManager::SyncProjectActor()
{
	if (!ProjectActor)
	{
		return;
	}

	const TArray<FProjectRecord>& Projects = SimulationSubsystem->GetProjects();
	if (!Projects.IsEmpty())
	{
		ProjectActor->ApplyProjectRecord(Projects[0]);
	}
}

void AColonyPresentationManager::ApplyConversationBehavior()
{
	const TArray<FAlertRecord>& Alerts = SimulationSubsystem->GetAlerts();
	if (Alerts.IsEmpty())
	{
		return;
	}

	const FString& LatestAlert = Alerts[0].Message;
	if (LatestAlert == LastConversationAlert || !LatestAlert.Contains(TEXT("->")))
	{
		return;
	}

	LastConversationAlert = LatestAlert;
	FString SpeakerName;
	FString Right;
	if (!LatestAlert.Split(TEXT("->"), &SpeakerName, &Right))
	{
		return;
	}

	SpeakerName = SpeakerName.TrimStartAndEnd();
	int32 ColonIndex = INDEX_NONE;
	FString ListenerName = Right;
	if (ListenerName.FindChar(TEXT(':'), ColonIndex))
	{
		ListenerName = ListenerName.Left(ColonIndex);
	}
	ListenerName = ListenerName.TrimStartAndEnd();

	for (const TPair<FGuid, AColonyNodeVisualActor*>& Pair : NodeActors)
	{
		AColonyNodeVisualActor* NodeActor = Pair.Value;
		if (NodeActor->GetNodeName() == SpeakerName)
		{
			NodeActor->SetConversationText(TEXT("Passing instruction"));
		}
		if (NodeActor->GetNodeName() == ListenerName)
		{
			NodeActor->SetConversationText(TEXT("Instruction received"));
		}
	}
}

FVector AColonyPresentationManager::ResolveTargetLocationForNode(const FNodeRecord& Node) const
{
	if (Node.State == ENodeLifecycleState::Baby)
	{
		return GetSlotPosition(TEXT("Nursery"), Node.Generation);
	}

	if (Node.State == ENodeLifecycleState::Running)
	{
		if (Node.Role == ENodeRole::Builder || Node.Role == ENodeRole::Planner)
		{
			return FMath::Lerp(WorkshopCenter, ProjectCenter, 0.45f);
		}
		if (Node.Role == ENodeRole::Verifier)
		{
			return VerificationCenter;
		}
		if (Node.Role == ENodeRole::Repairer)
		{
			return RepairCenter;
		}
		if (Node.Role == ENodeRole::Architect)
		{
			return ArchitectCoreCenter;
		}
	}

	if (Node.State == ENodeLifecycleState::Complete)
	{
		return VerificationCenter + FVector(-180.0f, -180.0f, 0.0f);
	}

	if (Node.State == ENodeLifecycleState::Error)
	{
		return RepairCenter + FVector(-160.0f, 110.0f, 0.0f);
	}

	if (Node.Role == ENodeRole::Architect)
	{
		return GetSlotPosition(TEXT("ArchitectCore"), Node.Generation + 1);
	}
	if (Node.Role == ENodeRole::Verifier)
	{
		return GetSlotPosition(TEXT("Verification"), Node.Generation + 1);
	}
	if (Node.Role == ENodeRole::Repairer)
	{
		return GetSlotPosition(TEXT("Repair"), Node.Generation + 1);
	}

	return GetSlotPosition(TEXT("Workshop"), Node.Generation + 1);
}

FVector AColonyPresentationManager::GetSlotPosition(const FName& ZoneName, int32 SlotIndex) const
{
	const int32 SafeSlot = FMath::Max(0, SlotIndex);
	int32 Columns = 3;
	float Spacing = 170.0f;
	FVector Center = WorkshopCenter;

	if (ZoneName == TEXT("Nursery"))
	{
		Center = NurseryCenter;
	}
	else if (ZoneName == TEXT("ArchitectCore"))
	{
		Center = ArchitectCoreCenter;
	}
	else if (ZoneName == TEXT("Verification"))
	{
		Center = VerificationCenter;
	}
	else if (ZoneName == TEXT("Repair"))
	{
		Center = RepairCenter;
	}
	else if (ZoneName == TEXT("Workshop"))
	{
		Center = WorkshopCenter;
		Columns = 4;
		Spacing = 180.0f;
	}

	const int32 Col = SafeSlot % Columns;
	const int32 Row = SafeSlot / Columns;
	const float HalfW = (Columns - 1) * Spacing * 0.5f;
	return Center + FVector((Col * Spacing) - HalfW, (Row * Spacing) - 130.0f, 80.0f);
}

void AColonyPresentationManager::HandleNodeSelected(const FGuid& NodeId)
{
	SelectedNodeId = NodeId;
	if (AColonyPlayerController* PC = Cast<AColonyPlayerController>(UGameplayStatics::GetPlayerController(this, 0)))
	{
		PC->SetSelectedNodeId(NodeId);
	}
}
