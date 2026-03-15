// José Manuel Rangel Muñoz. Copyright © Todos los derechos reservados (Excepto algunos assets).


#include "Player/Ball.h"


ABall::ABall()
{
	// Setea para este Pawn el evento Tick para cada frame.
	PrimaryActorTick.bCanEverTick = false;

	/* Creamos el static mesh de nuestro pawn y lo establecemos como root. */
	BodyComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body Component"));
	SetRootComponent(BodyComponent);

	/* Creamos la esfera de colisión del pawn y lo jerarquizamos. */
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision Component"));
	CollisionComponent->SetupAttachment(BodyComponent);

	/* Creamos el spring arm que ajustará la posición de la cámara y lo jerarquizamos. */
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm Component"));
	SpringArmComponent->SetupAttachment(BodyComponent);

	/* Creamos la cámara para poder ver el pawn y la jerarquizamos debajo del spring arm. */
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
	CameraComponent->SetupAttachment(SpringArmComponent);
}

void ABall::BeginPlay()
{
	Super::BeginPlay();
}


void ABall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ABall::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

