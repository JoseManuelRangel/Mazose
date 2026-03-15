// José Manuel Rangel Muñoz. Copyright © Todos los derechos reservados (Excepto algunos assets).

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"

#include "InputAction.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"

#include "Ball.h"

#include "BallPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;

/**
 * 
 */
UCLASS()
class MAZOSE_API ABallPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	/* VARIABLES */
	/* Assets de configuración. */
	/* Input Mapping Context. */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* DefaultMappingContext;

	/* Input Action de Movimiento. */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* VerticalMoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* HorizontalMoveAction;

	/* Input Action de Salto. */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* JumpAction;

	/* Input Action de Sprint. */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* SprintAction;

	/* Input Action de Dash. */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* DashAction;

	/* Referencia al jugador. */
	ABall* Player;

	/* FUNCIONES */
	/* Se llama al empezar el juego. */
	virtual void BeginPlay() override;

	/**
	* Este evento y esta función es para si quiero que la cámara sea la del mundo con un tag concreto.
	* virtual void OnPossess(APawn* InPawn) override;
	* void SetupSceneCamera();
	*/
	
	/* Función para vincular las acciones al context. */
	virtual void SetupInputComponent() override;

	/* Función para agregar movimiento vertical a la bola. */
	void VerticalMovementInput(const FInputActionValue& Value);

	/* Función para agregar movimiento horizontal a la bola. */
	void HorizontalMovementInput(const FInputActionValue& Value);

	/* Función que agrega la lógica de inserción de movimiento a la bola según el tipo de movimiento que sea. */
	void ApplyMovementByType(const FInputActionValue& Value, FString MovementType);

	/* Función para agregar movimiento de salto a la bola. */
	void JumpMovementInput();

	/* Función para agregar sprint a la bola si el jugador pulsa la tecla para ello. */
	void SprintMovementInput();

	/* Función para agregar movimiento de dash para la bola. */
	void DashMovementInput();

private:
	/* VARIABLES */
	/* Velocidad inicial de la Bola. */
	float InitialSpeed = 800.0f;

	/* Velocidad de movimiento de la Bola. */
	float Speed;

	/* Impulso de salto de la Bola. */
	float JumpImpulse = 600.0f;

	/* Fuerza del dash de la Bola. */
	float DashStrength = 1000.0f;

	/* Última dirección de movimiento de la Bola. */
	FVector LastMovementDirection;

	/* Resistencia linear de la Bola. */
	float LinearDamping = 2.5f;

	/* Condicionante de salto de la Bola. */
	bool bIsOnGround = true;

	/* Condicionante de sprint de la Bola. */
	bool bIsSprinting = false;

public:
	/* Función getter para obtener el Mapping Context del Controller. */
	UInputMappingContext* GetMappingContext() const { return DefaultMappingContext; };
};
