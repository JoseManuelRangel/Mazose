// José Manuel Rangel Muñoz. Copyright © Todos los derechos reservados (Excepto algunos assets).


#include "Player/BallPlayerController.h"
#include "Kismet/GameplayStatics.h"

void ABallPlayerController::BeginPlay()
{
	Super::BeginPlay();

	/* Creo el objeto "Subsystem" que obtendrá el control del Local Player. */
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	/* Si es válido añadimos el Input Mapping Context. */
	if (Subsystem) 
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	/* Obtengo el Pawn. */
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	/* Si el Pawn es válido lo casteo a un objeto de tipo Ball (el tipo del jugador). */
	if (PlayerPawn) 
	{
		Player = Cast<ABall>(PlayerPawn);

		if (Player) 
		{
			/* Simulo las físicas para que el personaje pueda moverse a través de añadirle fuerza. */
			Player->BodyComponent->SetSimulatePhysics(true);
			Speed = InitialSpeed;
		}
	}
}

void ABallPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	/* Aquí casteo el Input Component a Enhanced Input Component. */
	UEnhancedInputComponent* EnhancedInputComponent = Cast <UEnhancedInputComponent>(InputComponent);

	if (EnhancedInputComponent) 
	{
		/* Vinculo la acción de movimiento vertical. */
		EnhancedInputComponent->BindAction(VerticalMoveAction, ETriggerEvent::Triggered, this, &ABallPlayerController::VerticalMovementInput);
		
		/* Vinculo la acción de movimiento horizontal. */
		EnhancedInputComponent->BindAction(HorizontalMoveAction, ETriggerEvent::Triggered, this, &ABallPlayerController::HorizontalMovementInput);

		/* Vinculo la acción de movimiento de salto. */
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ABallPlayerController::JumpMovementInput);

		/* Vinculo la acción de movimiento de sprint. */
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &ABallPlayerController::SprintMovementInput);

		/* Vinculo la acción de movimiento de dash. */
		//EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Started, this, &ABallPlayerController::DashMovementInput);
	}
}

void ABallPlayerController::VerticalMovementInput(const FInputActionValue& Value)
{
	/* Si el Player es válido... */
	if (Player) {
		/* Al ser el movimiento vertical recurrimos a la función creada de aplicar movimiento y le especificamos que es "Vertical". */
		ApplyMovementByType(Value, "Vertical");
	}
}

void ABallPlayerController::HorizontalMovementInput(const FInputActionValue& Value)
{
	/* Si el Player es válido... */
	if (Player) {
		/* Al ser el movimiento horizontal recurrimos a la función creada de aplicar movimiento y le especificamos que es "Horizontal". */
		ApplyMovementByType(Value, "Horizontal");
	}
}

void ABallPlayerController::JumpMovementInput()
{
	/* Si el Player es válido... */
	if (Player) 
	{
		/* Obtenemos la localización del Player. */
		FVector Start = Player->GetActorLocation();

		/* Obtenemos una localización final en la que estaría el Player saltando. */
		FVector End = Start - FVector(0.0f, 0.0f, 30.0f);

		/* Creamos la fuerza de impulso de salto. */
		FVector Jump = FVector(0.0f, 0.0f, 600.0f);

		/* Creamos un objeto del tipo FHitResult para registrar el impacto. */
		FHitResult HitResult;

		/* Para que no se dispare a sí mismo, creamos un FCollisionQueryParams para que se ignore a él mismo. */
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(Player);

		/* Comprobamos si el rayo está golpeando algo. */
		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params);

		/* Si el rayo ha golpeado... */
		if (bHit)
		{
			/* Solo si está golpeando algo, se aplica el impulso. */
			Player->BodyComponent->AddImpulse(Jump, NAME_None, true);
		}
	}
}

void ABallPlayerController::SprintMovementInput()
{
	/* Si el Player es válido... */
	if (Player) 
	{
		/* Si el jugador está esprintando, se setea la velocidad normal y su estado pasa a ser sin esprintar. Si no, al contrario. */
		if (bIsSprinting)
		{
			Speed = InitialSpeed;
			bIsSprinting = false;
		}
		else
		{
			Speed = InitialSpeed * 2.50f;
			bIsSprinting = true;
		}
	}
}

void ABallPlayerController::DashMovementInput()
{
	/* Si el player existe y la última dirección de movimiento no es cero... */
	if (Player && !LastMovementDirection.IsNearlyZero())  
	{
		/* Usamos Add Impulse para que sea la aplicación de fuerza sobre el mesh de la Bola instantánea. */
		/* El parámetro "true" ignora la masa del objeto (VelChange). */
		Player->BodyComponent->AddImpulse((LastMovementDirection * DashStrength), NAME_None, true);
	}
}

void ABallPlayerController::ApplyMovementByType(const FInputActionValue& Value, FString MovementType)
{
	/* Conseguimos el valor del float del Input Action con la referencia al valor. */
	float InputValue = Value.Get<float>();

	/* Creamos un vector que representará la fuerza que vamos a darle a la bola. */
	FVector Force;

	/* En función del tipo de movimiento inicializaríamos el vector en un eje o en otro. */
	if (MovementType.Equals("Horizontal")) 
	{
		Force = FVector(0.0f, InputValue * Speed, 0.0f);
	}
	else 
	{
		Force = FVector(InputValue * Speed, 0.0f, 0.0f);
	}

	/* Si la bola se encuentra en movimiento, guardamos la dirección a la que apunta normalizada. */
	if (!Force.IsNearlyZero()) 
	{
		LastMovementDirection = Force.GetSafeNormal();
	}

	/* Variable para detectar si la bola toca el suelo. */
	FHitResult Hit;

	/* Consigo la localización actual de mi player. */
	FVector ActualLocation = Player->GetActorLocation();

	/* Marco la localización estimada de la bola cuando salta teniendo en cuenta su radio. */
	FVector JumpingLocation = ActualLocation - FVector(0.0f, 0.0f, 55.0f);

	/* Parámetros a la hora de ejecutar el LineTrace. */
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Player);

	/* Comprobamos si está en el suelo según un Line Trace. */
	bIsOnGround = GetWorld()->LineTraceSingleByChannel(Hit, ActualLocation, JumpingLocation, ECC_Visibility, Params);

	/* Si la bola no está en el suelo la fuerza la reduzco a un 30%. */
	if (!bIsOnGround)
	{
		float AirControlFactor = 0.5f;
		Force *= AirControlFactor;
	}

	/* Frenado dinámico */
	if (InputValue == 0.0f)
	{
		/* Si no hay input, aumento el damping para frenar en seco. */
		Player->BodyComponent->SetLinearDamping(LinearDamping * 1.5f);
	}
	else {
		/* Si hay input, mantengo el damping original para fluidez. */
		Player->BodyComponent->SetLinearDamping(LinearDamping);
	}

	/* Añadimos la fuerza que hemos creado anteriormente en el eje Y, que la aplique en todo el cuerpo y el cambio de aceleración en true. */
	Player->BodyComponent->AddForce(Force, NAME_None, true);
}

//void ABallPlayerController::OnPossess(APawn* InPawn)
//{
//	Super::OnPossess(InPawn);
//	SetupSceneCamera();
//}

//void ABallPlayerController::SetupSceneCamera()
//{
//	/* Busco el actor de la cámara en la escena. */
//	AActor* TargetCamera = nullptr;
//	TArray<AActor*> FoundActors;
//	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("PawnCamera"), FoundActors);
//
//	/* Si el número de actores con esa tag es más de 1 me quedo con la primera. */
//	if (FoundActors.Num() > 0)
//	{
//		TargetCamera = FoundActors[0];
//	}
//
//	/* Cambio la vista a ese actor. */
//	if (TargetCamera)
//	{
//		this->SetViewTarget(TargetCamera);
//	}
//}