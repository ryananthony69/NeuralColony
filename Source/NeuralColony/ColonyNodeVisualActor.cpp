#include "ColonyNodeVisualActor.h"

#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "UObject/ConstructorHelpers.h"

AColonyNodeVisualActor::AColonyNodeVisualActor()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	BaseRing = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseRing"));
	BaseRing->SetupAttachment(Root);
	BaseRing->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BaseRing->SetWorldScale3D(FVector(0.9f, 0.9f, 0.06f));
	BaseRing->SetRelativeLocation(FVector(0.0f, 0.0f, -35.0f));

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
	Mesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	Mesh->SetGenerateOverlapEvents(false);
	Mesh->SetWorldScale3D(FVector(0.5f));
	Mesh->SetMobility(EComponentMobility::Movable);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CylinderMesh(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
	if (SphereMesh.Succeeded())
	{
		Mesh->SetStaticMesh(SphereMesh.Object);
	}
	if (CylinderMesh.Succeeded())
	{
		BaseRing->SetStaticMesh(CylinderMesh.Object);
	}

	Label = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Label"));
	Label->SetupAttachment(Root);
	Label->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
	Label->SetWorldSize(24.0f);
	Label->SetRelativeLocation(FVector(0.0f, 0.0f, 80.0f));

	Bubble = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Bubble"));
	Bubble->SetupAttachment(Root);
	Bubble->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
	Bubble->SetWorldSize(20.0f);
	Bubble->SetRelativeLocation(FVector(0.0f, 0.0f, 120.0f));
	Bubble->SetText(FText::FromString(TEXT("")));
}

void AColonyNodeVisualActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	SetActorLocation(FMath::VInterpTo(GetActorLocation(), DesiredLocation, DeltaSeconds, 1.9f));

	if (BubbleTimeRemaining > 0.0f)
	{
		BubbleTimeRemaining -= DeltaSeconds;
		if (BubbleTimeRemaining <= 0.0f)
		{
			Bubble->SetText(FText::FromString(TEXT("")));
		}
	}
}

void AColonyNodeVisualActor::BeginPlay()
{
	Super::BeginPlay();
	OnClicked.AddDynamic(this, &AColonyNodeVisualActor::HandleClicked);
	DesiredLocation = GetActorLocation();
}

void AColonyNodeVisualActor::ApplyNodeRecord(const FNodeRecord& NodeRecord)
{
	NodeId = NodeRecord.Id;
	NodeName = NodeRecord.DisplayName;
	CachedState = NodeRecord.State;

	const FString StateLabel = GetStateLabel(NodeRecord.State);
	Label->SetText(FText::FromString(FString::Printf(TEXT("%s\n[%s]"), *NodeRecord.DisplayName, *StateLabel)));

	const FLinearColor Color = GetStateColor(NodeRecord.State);
	Mesh->SetVectorParameterValueOnMaterials(TEXT("Color"), FVector(Color.R, Color.G, Color.B));
	BaseRing->SetVectorParameterValueOnMaterials(TEXT("Color"), FVector(Color.R * 0.6f, Color.G * 0.6f, Color.B * 0.6f));

	const float Scale = GetStateScale(NodeRecord.State);
	Mesh->SetWorldScale3D(FVector(Scale));
	Label->SetTextRenderColor(Color.ToFColor(true));

	if (BubbleTimeRemaining <= 0.0f)
	{
		Bubble->SetText(FText::FromString(NodeRecord.Activity.Left(34)));
		BubbleTimeRemaining = 1.4f;
	}
}

void AColonyNodeVisualActor::SetConversationText(const FString& ConversationText)
{
	Bubble->SetText(FText::FromString(ConversationText));
	BubbleTimeRemaining = 2.8f;
}

void AColonyNodeVisualActor::SetSelected(bool bInSelected)
{
	Mesh->SetRenderCustomDepth(bInSelected);
	BaseRing->SetRenderCustomDepth(bInSelected);
}

void AColonyNodeVisualActor::SetDesiredLocation(const FVector& InDesiredLocation)
{
	DesiredLocation = InDesiredLocation;
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
		return FLinearColor(0.98f, 0.84f, 0.44f);
	case ENodeLifecycleState::Idle:
		return FLinearColor(0.72f, 0.74f, 0.77f);
	case ENodeLifecycleState::Running:
		return FLinearColor(0.31f, 0.85f, 0.54f);
	case ENodeLifecycleState::Complete:
		return FLinearColor(0.44f, 0.72f, 1.0f);
	case ENodeLifecycleState::Error:
		return FLinearColor(1.0f, 0.27f, 0.23f);
	default:
		return FLinearColor::White;
	}
}

FString AColonyNodeVisualActor::GetStateLabel(ENodeLifecycleState State) const
{
	return StaticEnum<ENodeLifecycleState>()->GetNameStringByValue(static_cast<int64>(State));
}

float AColonyNodeVisualActor::GetStateScale(ENodeLifecycleState State) const
{
	switch (State)
	{
	case ENodeLifecycleState::Baby:
		return 0.33f;
	case ENodeLifecycleState::Running:
		return 0.6f;
	case ENodeLifecycleState::Error:
		return 0.65f;
	default:
		return 0.5f;
	}
}
