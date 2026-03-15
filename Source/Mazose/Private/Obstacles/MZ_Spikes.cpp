// José Manuel Rangel Muñoz. Copyright © Todos los derechos reservados (Excepto algunos assets).


#include "Obstacles/MZ_Spikes.h"


AMZ_Spikes::AMZ_Spikes()
{
	// Setea para este actor el evento Tick para cada frame.
	PrimaryActorTick.bCanEverTick = true;

	/* Creamos el static mesh base que es la estructura de los pinchos. */
	BaseComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Component"));
	SetRootComponent(BaseComponent);

	/* Creamos el static mesh de los pinchos. */
	SpikesComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Spikes Component"));
	SpikesComponent->SetupAttachment(BaseComponent);

	/* Añadimos la funcionalidad de Overlap para los pinchos con el Pawn. */
	SpikesComponent->OnComponentBeginOverlap.AddDynamic(this, &AMZ_Spikes::OnOverlapBegin);
}

void AMZ_Spikes::BeginPlay()
{
	Super::BeginPlay();

	SettingTimeline();
}

void AMZ_Spikes::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMZ_Spikes::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	/* Si el otro actor es válido y no es este actor... */
	if (OtherActor && (OtherActor != this))
	{
		/* Si el otro actor tiene una tag "Pawn"... */
		if (OtherActor->ActorHasTag(FName(TEXT("Pawn"))))
		{
			/* Destruimos al player. */
			OtherActor->Destroy();
		}
	}
}

void AMZ_Spikes::SettingTimeline()
{
	/* Set de la localización incial y la final que tendrán los pinchos. */
	StartSpikesLocation = SpikesComponent->GetRelativeLocation();
	EndSpikesLocation = StartSpikesLocation + FVector(0.0f, 0.0f, 40.0f);

	if (MovementCurve)
	{
		/* Variables para registrar el valor mínimo y el máximo de la curva. */
		float MinTime, MaxTime;

		/* Obtenemos esos valores con GetTimeRange de la curva. */
		MovementCurve->GetTimeRange(MinTime, MaxTime);

		/* Creamos el objeto Timeline. */
		MovementTimelineComponent = NewObject<UTimelineComponent>(this, FName("SpikesMovementTimeLine"));

		/* Ajustamos el Timeline con el valor máximo de la curva. */
		MovementTimelineComponent->SetTimelineLength(MaxTime);

		/* 1. Definición de la función que se llama en cada paso. */
		FOnTimelineFloat ProgressFunction;
		ProgressFunction.BindUFunction(this, FName("HandleSpikesProgress"));

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

void AMZ_Spikes::HandleSpikesProgress(float Value)
{
	/* Se hace una interpolación entre la localización de inicio y la final de los pinchos. */
	FVector NewLocation = FMath::Lerp(StartSpikesLocation, EndSpikesLocation, Value);

	/* Se setea la localización relativa de los pinchos con la localización interpolada anteriormente creada. */
	SpikesComponent->SetRelativeLocation(NewLocation);

	if (Value > 0.1f && Value < 1.3f) 
	{
		SpikesComponent->SetGenerateOverlapEvents(true);
	}
	else 
	{
		SpikesComponent->SetGenerateOverlapEvents(false);
	}
}

void AMZ_Spikes::OnTimeLineFinished()
{
	/* Si la Timeline llega al final, se hace un Reverse y se reproduce la Timeline al revés. */
	if (MovementTimelineComponent->GetPlaybackPosition() >= MovementTimelineComponent->GetTimelineLength())
	{
		MovementTimelineComponent->PlayFromStart();
	}
	else if (MovementTimelineComponent->GetPlaybackPosition() <= 0.0f)
	{
		/* Si llega al inicio, se reproduce de manera normal. */
		MovementTimelineComponent->PlayFromStart();
	}
}




