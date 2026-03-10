#include "ColonyPresentationManager.h"

#include "ColonyHUDWidget.h"
#include "ColonyNodeVisualActor.h"
#include "ColonyPlayerController.h"
#include "ColonyProjectSiteActor.h"
#include "ColonySimulationSubsystem.h"
#include "Engine/StaticMeshActor.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

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

	AStaticMeshActor* Ground = GetWorld()->SpawnActor<AStaticMeshActor>(FVector::ZeroVector, FRotator::ZeroRotator);
	if (Ground)
	{
		UStaticMeshComponent* GroundMesh = Ground->GetStaticMeshComponent();
		static ConstructorHelpers::FObjectFinder<UStaticMesh> PlaneMesh(TEXT("/Engine/BasicShapes/Plane.Plane"));
		if (PlaneMesh.Succeeded())
		{
			GroundMesh->SetStaticMesh(PlaneMesh.Object);
			GroundMesh->SetWorldScale3D(FVector(30.0f, 30.0f, 1.0f));
		}
	}

	ProjectActor = GetWorld()->SpawnActor<AColonyProjectSiteActor>(FVector(0.0f, 700.0f, 40.0f), FRotator::ZeroRotator);

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
	ApplyConversationBubbles();

	if (HUDWidget)
	{
		HUDWidget->SetSelectedNodeId(SelectedNodeId);
	}
}

void AColonyPresentationManager::EnsureNodeActors()
{
	const TArray<FNodeRecord>& Nodes = SimulationSubsystem->GetNodes();
	for (int32 Index = 0; Index < Nodes.Num(); ++Index)
	{
		const FNodeRecord& Node = Nodes[Index];
		if (!NodeActors.Contains(Node.Id))
		{
			const FVector SpawnLocation = FVector((Index % 6) * 300.0f - 750.0f, (Index / 6) * 320.0f - 200.0f, 80.0f);
			AColonyNodeVisualActor* NodeActor = GetWorld()->SpawnActor<AColonyNodeVisualActor>(SpawnLocation, FRotator::ZeroRotator);
			if (NodeActor)
			{
				NodeActor->OnNodeSelected.AddDynamic(this, &AColonyPresentationManager::HandleNodeSelected);
				NodeActors.Add(Node.Id, NodeActor);
			}
		}
	}
}

void AColonyPresentationManager::SyncNodeActors()
{
	for (const FNodeRecord& Node : SimulationSubsystem->GetNodes())
	{
		if (AColonyNodeVisualActor** NodeActorPtr = NodeActors.Find(Node.Id))
		{
			(*NodeActorPtr)->ApplyNodeRecord(Node);
			(*NodeActorPtr)->SetSelected(Node.Id == SelectedNodeId);
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

void AColonyPresentationManager::ApplyConversationBubbles()
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
	FString Left;
	FString Right;
	if (LatestAlert.Split(TEXT("->"), &Left, &Right))
	{
		Left = Left.TrimStartAndEnd();
		FString Listener = Right;
		int32 ColonIndex = INDEX_NONE;
		if (Listener.FindChar(TEXT(':'), ColonIndex))
		{
			Listener = Listener.Left(ColonIndex);
		}
		Listener = Listener.TrimStartAndEnd();

		for (const TPair<FGuid, AColonyNodeVisualActor*>& Pair : NodeActors)
		{
			if (Pair.Value->GetNodeName() == Left)
			{
				Pair.Value->SetConversationText(TEXT("Sending context"));
			}
			if (Pair.Value->GetNodeName() == Listener)
			{
				Pair.Value->SetConversationText(TEXT("Received"));
			}
		}
	}
}

void AColonyPresentationManager::HandleNodeSelected(const FGuid& NodeId)
{
	SelectedNodeId = NodeId;
	if (AColonyPlayerController* PC = Cast<AColonyPlayerController>(UGameplayStatics::GetPlayerController(this, 0)))
	{
		PC->SetSelectedNodeId(NodeId);
	}
}
