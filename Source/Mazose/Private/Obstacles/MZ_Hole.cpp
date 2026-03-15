// José Manuel Rangel Muñoz. Copyright © Todos los derechos reservados (Excepto algunos assets).


#include "Obstacles/MZ_Hole.h"
#include "Player/BallPlayerController.h"
#include "Kismet/KismetSystemLibrary.h"


AMZ_Hole::AMZ_Hole()
{
	// Setea para este actor el evento Tick para cada frame.
	PrimaryActorTick.bCanEverTick = true;

	/* Creamos el static mesh base que es la estructura del agujero. */
	BaseComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Component"));
	SetRootComponent(BaseComponent);

	/* Creamos el static mesh de la primera trampilla del agujero. */
	FirstHatchPartComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("First Hatch Part Component"));
	FirstHatchPartComponent->SetupAttachment(BaseComponent);

	/* Creamos el static mesh de la segunda trampilla del agujero. */
	SecondHatchPartComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Second Hatch Part Component"));
	SecondHatchPartComponent->SetupAttachment(BaseComponent);

	/* Creamos la esfera de colisión para que cuando caiga la bola dentro del agujero haga algo. */
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision Component"));
	CollisionComponent->SetupAttachment(BaseComponent);

	/* Añadimos la funcionalidad en los eventos de Overlap del agujero. */
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AMZ_Hole::OnOverlapBegin);
}

void AMZ_Hole::BeginPlay()
{
	Super::BeginPlay();

	/* Activamos la Timeline al inciar el juego. */
	SettingTimeline();
}

void AMZ_Hole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMZ_Hole::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	/* Si el otro actor es válido y no es este actor... */
	if (OtherActor && (OtherActor != this))
	{
		/* Si el otro actor tiene una tag "Pawn"... */
		if (OtherActor->ActorHasTag(FName(TEXT("Pawn"))))
		{
			/* Desactivamos los controles del jugador. */
			DeactivatePawnInput();
		}
	}
}

void AMZ_Hole::SettingTimeline()
{
	if (FirstHatchPartComponent && SecondHatchPartComponent)
	{
		/* Set de la localización inicial y la final que tendrá la primera trampilla. */
		StartFirstHatchRotation = FirstHatchPartComponent->GetRelativeRotation();
		EndFirstHatchRotation = StartFirstHatchRotation - FRotator(90.0f, 0.0f, 0.0f);

		/* Set de la localización inicial y la final que tendrá la segunda trampilla. */
		StartSecondHatchRotation = SecondHatchPartComponent->GetRelativeRotation();
		EndSecondHatchRotation = StartSecondHatchRotation - FRotator(90.0f, 0.0f, 0.0f);
	}
	
	if (MovementCurve)
	{
		/* Variables para registrar el valor mínimo y el máximo de la curva. */
		float MinTime, MaxTime;

		/* Obtenemos esos valores con GetTimeRange de la curva. */
		MovementCurve->GetTimeRange(MinTime, MaxTime);

		/* Creamos el objeto Timeline. */
		MovementTimelineComponent = NewObject<UTimelineComponent>(this, FName("HatchMovementTimeLine"));

		/* Ajustamos el Timeline con el valor máximo de la curva. */
		MovementTimelineComponent->SetTimelineLength(MaxTime);

		/* 1. Definición de la función que se llama en cada paso. */
		FOnTimelineFloat ProgressFunction;
		ProgressFunction.BindUFunction(this, FName("HandleHoleProgress"));

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

void AMZ_Hole::HandleHoleProgress(float Value)
{
	/* Se hace una interpolación entre la localización de inicio y la final de la trampilla. */
	FRotator NewRotation = FMath::Lerp(StartFirstHatchRotation, EndFirstHatchRotation, Value);
	FRotator NewSecondRotation = FMath::Lerp(StartSecondHatchRotation, EndSecondHatchRotation, Value);

	/* Se setea la localización relativa de la trampilla con la localización interpolada anteriormente creada. */
	FirstHatchPartComponent->SetRelativeRotation(NewRotation);
	SecondHatchPartComponent->SetRelativeRotation(NewSecondRotation);
}

void AMZ_Hole::OnTimeLineFinished()
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

void AMZ_Hole::DeactivatePawnInput()
{
	/* Casting de Player Controller a su clase base. */
	if (ABallPlayerController* PC = Cast<ABallPlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		/* Desactivación del input. */
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(PC->GetMappingContext());
		}
	}
}
