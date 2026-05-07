// José Manuel Rangel Muñoz. Copyright © Todos los derechos reservados (Excepto algunos assets).


#include "Mechanics/MZ_Spikeball.h"

AMZ_Spikeball::AMZ_Spikeball()
{
	// Setea para este actor el evento Tick para cada frame.
	PrimaryActorTick.bCanEverTick = false;

	BallBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Spikeball Base"));
	SetRootComponent(BallBase);

	BallCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Spikeball Collision"));
	BallCollision->SetupAttachment(BallBase);

	PatrolTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Spikeball Timeline"));
}

void AMZ_Spikeball::BeginPlay()
{
	Super::BeginPlay();

	StartTransform = GetActorTransform();

	/* Si la curva ha sido introducida y es válida... */
	if (PatrolCurve && PatrolWaypoints.Num() > 1)
	{
		/* Vinculo la función para el Timeline. */
		FOnTimelineFloat ProgressFunction;
		ProgressFunction.BindUFunction(this, FName("HandleTimelineProgress"));
		PatrolTimeline->AddInterpFloat(PatrolCurve, ProgressFunction);

		/* Creo el evento que disparará una función al acabar el timeline. */
		FOnTimelineEvent FinishedEvent;
		FinishedEvent.BindUFunction(this, FName("OnWaypointReached"));
		PatrolTimeline->SetTimelineFinishedFunc(FinishedEvent);

		/* Primer tramo. */
		CurrentIndex = 0;

		StartLocation = GetActorLocation();

		TargetLocation = PatrolWaypoints[CurrentIndex];

		PatrolTimeline->PlayFromStart();
	}

	if (BallCollision)
	{
		BallCollision->OnComponentBeginOverlap.AddDynamic(this, &AMZ_Spikeball::OnOverlap);
	}
}

void AMZ_Spikeball::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AMZ_Spikeball::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		// 1. Verificamos que sea un Pawn
		APawn* PawnGolpeado = Cast<APawn>(OtherActor);
		if (PawnGolpeado)
		{
			// 2. Obtenemos el componente físico del Pawn (normalmente su RootComponent)
			UPrimitiveComponent* PhysComp = Cast<UPrimitiveComponent>(PawnGolpeado->GetRootComponent());

			if (PhysComp && PhysComp->IsSimulatingPhysics())
			{
				// Calculamos dirección: desde la bola hacia el Pawn
				FVector Direccion = PawnGolpeado->GetActorLocation() - GetActorLocation();
				Direccion.Normalize();

				// Aplicamos el impulso
				// El parámetro 'true' en bVelocityChange hace que ignore la masa (útil para que el empujón sea constante)
				PhysComp->AddImpulse(Direccion * ImpulseStrength + FVector(0.0f, 0.0f, 1000.0f), NAME_None, true);
			}
		}
	}
}

void AMZ_Spikeball::HandleTimelineProgress(float Value)
{
	/* Movimiento entre los puntos A y B. */
	FVector NewLoc = FMath::Lerp(StartLocation, TargetLocation, Value);
	SetActorLocation(NewLoc, true);

	/* Rotación constante (sobre sí misma). */
	FRotator NewRot = GetActorRotation();
	NewRot.Yaw += 4.0f; // Velocidad de giro.
	SetActorRotation(NewRot);
}

void AMZ_Spikeball::OnWaypointReached()
{
	/* Preparo las nuevas coordenadas. */
	StartLocation = GetActorLocation();

	/* Actualizo el índice para el siguiente tramo de la patrulla. */
	CurrentIndex = (CurrentIndex + 1) % PatrolWaypoints.Num();

	TargetLocation = PatrolWaypoints[CurrentIndex];

	/* Hacemos que sea infinito. */
	PatrolTimeline->PlayFromStart();
}

