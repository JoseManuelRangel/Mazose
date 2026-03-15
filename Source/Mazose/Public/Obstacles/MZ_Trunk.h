// José Manuel Rangel Muñoz. Copyright © Todos los derechos reservados (Excepto algunos assets).

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "Components/TimeLineComponent.h"
#include "Components/BoxComponent.h"

#include "MZ_Trunk.generated.h"

UCLASS()
class MAZOSE_API AMZ_Trunk : public AActor
{
	GENERATED_BODY()
	
public:
	/* Constructor. */
	AMZ_Trunk();

protected:
	/* Begin Play: Se llama cuando empieza el juego. */
	virtual void BeginPlay() override;

public:
	/* Tick: Se llama a cada frame. */
	virtual void Tick(float DeltaTime) override;

public:
	/* VARIABLES DEL ACTOR */
	/* Static Mesh (Root). */
	UPROPERTY(VisibleAnywhere, Category = "Trunk Structure")
	UStaticMeshComponent* MeshComponent;

	/* Localización inicial del tronco. */
	FVector StartTrunkLocation;

	/* Multiplicador de fuerza para el tronco. */
	UPROPERTY(EditAnywhere, Category = "Trunk Movement")
	float ForceMultiplier = 25000.0f;

	UPROPERTY(VisibleAnywhere, Category = "Trunk Structure")
	USceneComponent* SceneComponent;

	/* Componente de colisión que se situa al frente del tronco. */
	UPROPERTY(VisibleAnywhere, Category = "Trunk Structure")
	UBoxComponent* CollisionComponent;

	/* Curva que define el movimiento (Asignable en el editor). */
	UPROPERTY(EditAnywhere, Category = "Trunk Movement")
	UCurveFloat* MovementCurve;

	/* Componente que gestionará el tiempo. */
	UPROPERTY()
	UTimelineComponent* MovementTimelineComponent;


	/* FUNCIONES DEL ACTOR */
	/* Función que se ejecuta cada frame del Timeline. */
	UFUNCTION()
	void HandleTrunkProgress(float Value);

	/* Función que se ejecuta al finalizar el Timeline. */
	UFUNCTION()
	void OnTimeLineFinished();

	/* Función para el inicio del overlap con el Pawn. */
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/* Función que activa la Timeline. */
	void SettingTimeline();
};
