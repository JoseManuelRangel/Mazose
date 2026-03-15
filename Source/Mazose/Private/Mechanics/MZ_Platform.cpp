// José Manuel Rangel Muñoz. Copyright © Todos los derechos reservados (Excepto algunos assets).


#include "Mechanics/MZ_Platform.h"
#include "Player/BallPlayerController.h"


AMZ_Platform::AMZ_Platform()
{
	// Setea para este actor el evento Tick para cada frame.
	PrimaryActorTick.bCanEverTick = true;

	/* Creamos el static mesh base que es la estructura de la plataforma. */
	BaseComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Component"));
	SetRootComponent(BaseComponent);

	/* Creamos el static mesh del elevador de la plataforma. */
	PlatformComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Elevator Component"));
	PlatformComponent->SetupAttachment(BaseComponent);

	/* Creamos el cubo de colisión para que cuando entre la bola la guarde bajandola con el elevador. */
	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision Component"));
	CollisionComponent->SetupAttachment(BaseComponent);

	/* Añadimos la funcionalidad en los eventos de Overlap de la plataforma. */
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AMZ_Platform::OnOverlapBegin);
}

void AMZ_Platform::BeginPlay()
{
	Super::BeginPlay();
	
	/* Si el base component tiene una tag "Start"... */
	if (BaseComponent->ComponentHasTag(FName("Start"))) 
	{
		/* Se setea la localización del elevador de inicio y llamamos al timeline para que eleve el Pawn a la fase. */
		PlatformComponent->SetRelativeLocation(FVector(0.0f, 70.0f, -120.0f));
		SettingTimeline("Start");
	}
	else 
	{
		/* Se setea la localización del elevador para que el Pawn pueda entrar en la plataforma para finalizar la fase. */
		PlatformComponent->SetRelativeLocation(FVector(0.0f, 70.0f, -20.0f));
	}
}

void AMZ_Platform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMZ_Platform::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	/* Si el otro actor es válido y no es este actor... */
	if (OtherActor && (OtherActor != this))
	{
		/* Si el otro actor tiene una tag "Pawn"... */
		if (OtherActor->ActorHasTag(FName(TEXT("Pawn"))))
		{
			/* Si el componente base tiene una tag "End" y el Pawn no está aún en la plataforma. */
			if (BaseComponent->ComponentHasTag(FName(TEXT("End"))) && !bIsAlreadyIn)
			{
				/* Se setea la secuencia de elevación de la plataforma hacia abajo marcando el final de la fase. */
				SettingTimeline("End");
				bIsAlreadyIn = true;

				/* Desactivamos el Input Mapping Context de las fases para el Pawn. */
				DeactivatePawnInput();
			}
		}
	}
}

void AMZ_Platform::SettingTimeline(FString PlatformType)
{
	/* Si la plataforma es la de inicio o la del final seteamos unas localizaciones relativas de inicio y fin para el elevador. */
	if (PlatformType.Equals("Start"))
	{
		StartElevatorLocation = PlatformComponent->GetRelativeLocation();
		EndElevatorLocation = StartElevatorLocation + FVector(0.0f, 0.0f, 100.0f);
	}
	else if (PlatformType.Equals("End"))
	{
		StartElevatorLocation = PlatformComponent->GetRelativeLocation();
		EndElevatorLocation = StartElevatorLocation + FVector(0.0f, 0.0f, -100.0f);
	}

	if (MovementCurve)
	{
		/* Variables para registrar el valor mínimo y el máximo de la curva. */
		float MinTime, MaxTime;

		/* Obtenemos esos valores con GetTimeRange de la curva. */
		MovementCurve->GetTimeRange(MinTime, MaxTime);

		/* Creamos el objeto Timeline. */
		MovementTimelineComponent = NewObject<UTimelineComponent>(this, FName("MovementTimeLine"));

		/* Ajustamos el Timeline con el valor máximo de la curva. */
		MovementTimelineComponent->SetTimelineLength(MaxTime);

		/* 1. Definición de la función que se llama en cada paso. */
		FOnTimelineFloat ProgressFunction;
		ProgressFunction.BindUFunction(this, FName("HandleTimelineProgress"));

		/* 2. Adición de la curva al timeline. */
		MovementTimelineComponent->AddInterpFloat(MovementCurve, ProgressFunction);

		/* 3. Registramos el componente y le damos a que reproduzca la Timeline desde el principio. */
		MovementTimelineComponent->RegisterComponent();
		MovementTimelineComponent->PlayFromStart();
	}
}

void AMZ_Platform::HandleTimelineProgress(float Value)
{
	/* Lerp (Interpolación Lineal) entre inicio y fin según el valor de la curva (de 0 a 3). */
	FVector NewLocation = FMath::Lerp(StartElevatorLocation, EndElevatorLocation, Value);

	/* Aplicación de la posición relativa. */
	PlatformComponent->SetRelativeLocation(NewLocation);
}


void AMZ_Platform::DeactivatePawnInput()
{
	/* Casting de Player Controller a su clase base. */
	if (ABallPlayerController* PC = Cast<ABallPlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		/* Desactiva el Input. */
		if(UEnhancedInputLocalPlayerSubsystem* Subsystem = 
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(PC->GetMappingContext());
		}
	}
}

