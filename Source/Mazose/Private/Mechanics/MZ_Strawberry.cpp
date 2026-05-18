// José Manuel Rangel Muñoz. Copyright © Todos los derechos reservados (Excepto algunos assets).


#include "Mechanics/MZ_Strawberry.h"

#include "Components/SphereComponent.h"

#include "Player/BallPlayerController.h"

#include "Widgets/MZ_LevelWidget.h"

#include "Kismet/GameplayStatics.h"


AMZ_Strawberry::AMZ_Strawberry()
{
	// Setea para este actor el evento Tick para cada frame.
	PrimaryActorTick.bCanEverTick = false;

	/* Creo la colisión y la establezco como raíz. */
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Strawberry Collision"));
	SetRootComponent(CollisionComponent);
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AMZ_Strawberry::OnOverlapBegin);

	/* Creo la malla estática. */
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Strawberry Body"));
	MeshComponent->SetupAttachment(CollisionComponent);

	/* Inicializo la Timeline. */
	DisappearingTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Strawberry Timeline"));
}

void AMZ_Strawberry::BeginPlay()
{
	Super::BeginPlay();
	
	/* Early return para comprobar la validez de la curva. */
	if (!ScaleCurve)
	{
		return;
	}

	/* Vinculo el progreso de la curva. */
	FOnTimelineFloat ProgressFunction;
	ProgressFunction.BindUFunction(this, FName("TimelineProgress"));
	DisappearingTimeline->AddInterpFloat(ScaleCurve, ProgressFunction);

	/* Vinculo el final de la curva. */
	FOnTimelineEvent FinishedFunction;
	FinishedFunction.BindUFunction(this, FName("TimelineFinished"));
	DisappearingTimeline->SetTimelineFinishedFunc(FinishedFunction);
}

void AMZ_Strawberry::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	/* Si no está la fresa recolectada y el otro actor es el pawn... */
	if (!bWasCollected && (OtherActor == UGameplayStatics::GetPlayerPawn(GetWorld(), 0)))
	{
		/* Notifico que ahora sí esta recolectada. */
		bWasCollected = true;

		/* Casteo al controller personalizado. */
		ABallPlayerController* PC = Cast<ABallPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

		if (PC && PC->CurrentLevelWidget)
		{
			/* Le hacemos el casteo al widget del nivel. */
			UMZ_LevelWidget* W = Cast<UMZ_LevelWidget>(PC->CurrentLevelWidget);

			/* Early return del level widget. */
			if (!W) 
			{
				return;
			}

			/* Si el casteo es válido hacemos el broadcast del event dispatcher. */
			W->AddingStrawberrries();

			/* Ejecutamos la timeline. */
			DisappearingTimeline->PlayFromStart();
		}
	}
}

void AMZ_Strawberry::TimelineProgress(float Value)
{
	/* Nueva escala que va reduciendo la fresa. */
	FVector NewScale = FVector(Value, Value, Value);

	/* Va estableciendo la escala. */
	MeshComponent->SetWorldScale3D(NewScale);
}

void AMZ_Strawberry::TimelineFinished()
{
	/* Se destruye la fresa. */
	Destroy();
}


void AMZ_Strawberry::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

