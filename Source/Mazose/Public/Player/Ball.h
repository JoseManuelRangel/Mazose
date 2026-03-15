// José Manuel Rangel Muñoz. Copyright © Todos los derechos reservados (Excepto algunos assets).

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"

#include "Components/SphereComponent.h"

#include "Camera/CameraComponent.h"

#include "Ball.generated.h"

UCLASS()
class MAZOSE_API ABall : public APawn
{
	GENERATED_BODY()

public:
	/* Constructor */
	ABall();

protected:
	/* Begin Play: Se llama cuando empieza el juego. */
	virtual void BeginPlay() override;

	/* Tick: Llamado cada frame. */
	virtual void Tick(float DeltaTime) override;
public:
	/* VARIABLES DEL PAWN */
	/* Static Mesh de la bola. (Root) */
	UPROPERTY(VisibleAnywhere, Category = "Pawn Structure")
	UStaticMeshComponent* BodyComponent;

	/* Collision Sphere de la bola. */
	UPROPERTY(VisibleAnywhere, Category = "Pawn Structure")
	USphereComponent* CollisionComponent;

	/* Spring Arm para la cámara de la bola. */
	UPROPERTY(VisibleAnywhere, Category = "Pawn Camera")
	USpringArmComponent* SpringArmComponent;

	/* Cámara para la bola. */
	UPROPERTY(VisibleAnywhere, Category = "Pawn Camera")
	UCameraComponent* CameraComponent;


	/* FUNCIONES DEL PAWN */
	/* Llamado para vincular la funcionalidad de entrada para los controles de usuario. */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
