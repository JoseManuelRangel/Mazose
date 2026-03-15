// José Manuel Rangel Muñoz. Copyright © Todos los derechos reservados (Excepto algunos assets).

#include "Obstacles/MZ_Trunk.h"
#include "Player/BallPlayerController.h"
#include "Kismet/KismetSystemLibrary.h"

AMZ_Trunk::AMZ_Trunk()
{
	// Setea para este actor el evento Tick para cada frame.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));
	SetRootComponent(SceneComponent);

	/* Creamos el static mesh base que es la forma del tronco. */
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	MeshComponent->SetupAttachment(SceneComponent);

	/* Habilitamos las físicas para el tronco. */
	MeshComponent->SetSimulatePhysics(true);

	/* Creamos la caja de colisión par que cuando choque con el Pawn sea destruída. */
	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision Component"));
	CollisionComponent->SetupAttachment(MeshComponent);

	/* Añadimos la funcionalidad en los eventos de Overlap del tronco. */
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AMZ_Trunk::OnOverlapBegin);
}

void AMZ_Trunk::BeginPlay()
{
	Super::BeginPlay();

	SettingTimeline();
}

void AMZ_Trunk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMZ_Trunk::SettingTimeline()
{
	if (MovementCurve)
	{
		/* Variables para registrar el valor mínimo y el máximo de la curva. */
		float MinTime, MaxTime;

		/* Obtenemos esos valores con GetTimeRange de la curva. */
		MovementCurve->GetTimeRange(MinTime, MaxTime);

		/* Creamos el objeto Timeline. */
		MovementTimelineComponent = NewObject<UTimelineComponent>(this, FName("TrunkMovementTimeLine"));

		/* Ajustamos el Timeline con el valor máximo de la curva. */
		MovementTimelineComponent->SetTimelineLength(MaxTime);

		/* 1. Definición de la función que se llama en cada paso. */
		FOnTimelineFloat ProgressFunction;
		ProgressFunction.BindUFunction(this, FName("HandleTrunkProgress"));

		FOnTimelineEvent FinishedFunction;
		FinishedFunction.BindUFunction(this, FName("OnTimeLineFinished"));

		/* 2. Adición de la curva al timeline. */
		MovementTimelineComponent->AddInterpFloat(MovementCurve, ProgressFunction);
		MovementTimelineComponent->SetTimelineFinishedFunc(FinishedFunction);

		/* 3. Registramos el componente y le damos a que reproduzca la Timeline desde el principio. */
		MovementTimelineComponent->RegisterComponent();
		MovementTimelineComponent->PlayFromStart();
	}
}

void AMZ_Trunk::HandleTrunkProgress(float Value)
{
	/* Comprobación de validez del MeshComponent. */
	if (!MeshComponent)
	{
		return;
	}

	/* Cogemos el Deltatime. */
	float DeltaTime = GetWorld()->GetDeltaSeconds();

	/* Calculo de la dirección. */
	FVector ForceDirection = MeshComponent->GetForwardVector();

	/* Aplicación de la fuerza escalada por el valor de la curva. */
	MeshComponent->AddForce(ForceDirection * Value * ForceMultiplier * DeltaTime, NAME_None, true);
}

void AMZ_Trunk::OnTimeLineFinished()
{
	Destroy();
}

void AMZ_Trunk::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	/* Si el otro actor es válido y no es este actor... */
	if (OtherActor && (OtherActor != this)) 
	{
		/* Si el otro actor tiene una tag "Pawn"... */
		if (OtherActor->ActorHasTag(FName(TEXT("Pawn")))) 
		{
			OtherActor->Destroy();
		}
	}
}



