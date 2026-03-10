#include "ColonyNodeVisualActor.h"

#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "UObject/ConstructorHelpers.h"

AColonyNodeVisualActor::AColonyNodeVisualActor()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
	Mesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	Mesh->SetGenerateOverlapEvents(false);
	Mesh->SetWorldScale3D(FVector(0.6f));
	Mesh->SetMobility(EComponentMobility::Movable);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	if (SphereMesh.Succeeded())
	{
		Mesh->SetStaticMesh(SphereMesh.Object);
	}

	Label = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Label"));
	Label->SetupAttachment(Root);
	Label->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
	Label->SetWorldSize(26.0f);
	Label->SetRelativeLocation(FVector(0.0f, 0.0f, 90.0f));

	Bubble = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Bubble"));
	Bubble->SetupAttachment(Root);
	Bubble->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
	Bubble->SetWorldSize(20.0f);
	Bubble->SetRelativeLocation(FVector(0.0f, 0.0f, 130.0f));
	Bubble->SetText(FText::FromString(TEXT("")));
}

void AColonyNodeVisualActor::BeginPlay()
{
	Super::BeginPlay();
	OnClicked.AddDynamic(this, &AColonyNodeVisualActor::HandleClicked);
}

void AColonyNodeVisualActor::ApplyNodeRecord(const FNodeRecord& NodeRecord)
{
	NodeId = NodeRecord.Id;
	NodeName = NodeRecord.DisplayName;

	const FString StateLabel = GetStateLabel(NodeRecord.State);
	Label->SetText(FText::FromString(FString::Printf(TEXT("%s\n[%s]"), *NodeRecord.DisplayName, *StateLabel)));
	const FLinearColor Color = GetStateColor(NodeRecord.State);
	Mesh->SetVectorParameterValueOnMaterials(TEXT("Color"), FVector(Color.R, Color.G, Color.B));
	Mesh->SetRenderCustomDepth(false);

	Bubble->SetText(FText::FromString(NodeRecord.Activity.Left(32)));
}

void AColonyNodeVisualActor::SetConversationText(const FString& ConversationText)
{
	Bubble->SetText(FText::FromString(ConversationText));
}

void AColonyNodeVisualActor::SetSelected(bool bInSelected)
{
	Mesh->SetRenderCustomDepth(bInSelected);
}

void AColonyNodeVisualActor::HandleClicked(AActor* TouchedActor, FKey ButtonPressed)
{
	OnNodeSelected.Broadcast(NodeId);
}

FLinearColor AColonyNodeVisualActor::GetStateColor(ENodeLifecycleState State) const
{
	switch (State)
	{
	case ENodeLifecycleState::Baby:
		return FLinearColor(0.8f, 0.8f, 1.0f);
	case ENodeLifecycleState::Idle:
		return FLinearColor(0.6f, 0.6f, 0.6f);
	case ENodeLifecycleState::Running:
		return FLinearColor(0.2f, 0.8f, 0.2f);
	case ENodeLifecycleState::Complete:
		return FLinearColor(0.1f, 0.5f, 1.0f);
	case ENodeLifecycleState::Error:
		return FLinearColor(1.0f, 0.2f, 0.2f);
	default:
		return FLinearColor::White;
	}
}

FString AColonyNodeVisualActor::GetStateLabel(ENodeLifecycleState State) const
{
	return StaticEnum<ENodeLifecycleState>()->GetNameStringByValue(static_cast<int64>(State));
}
