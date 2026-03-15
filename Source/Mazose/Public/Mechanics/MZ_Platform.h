// José Manuel Rangel Muñoz. Copyright © Todos los derechos reservados (Excepto algunos assets).

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "Components/BoxComponent.h"
#include "Components/TimeLineComponent.h"

#include "MZ_Platform.generated.h"

UCLASS()
class MAZOSE_API AMZ_Platform : public AActor
{
	GENERATED_BODY()
	
public:	
	/* Constructor */
	AMZ_Platform();

protected:
	/* Begin Play: Se llama cuando empieza el juego. */
	virtual void BeginPlay() override;

public:
	/* Tick: Se llama a cada frame. */
	virtual void Tick(float DeltaTime) override;

public:	
	/* VARIABLES DEL ACTOR */
	/* Static Mesh Base. (Root) */
	UPROPERTY(VisibleAnywhere, Category = "Platform Structure")
	UStaticMeshComponent* BaseComponent;

	/* Static Mesh de la Plataforma. */
	UPROPERTY(VisibleAnywhere, Category = "Platform Structure")
	UStaticMeshComponent* PlatformComponent;

	/* Componente de colisión en caja del actor. */
	UPROPERTY(VisibleAnywhere, Category = "Platform Structure")
	UBoxComponent* CollisionComponent;

	/* Componente que gestionará el tiempo. */
	UPROPERTY()
	UTimelineComponent* MovementTimelineComponent;

	/* Curva que define el movimiento (Asignable en el Editor). */
	UPROPERTY(EditAnywhere, Category = "Platform Movement")
	UCurveFloat* MovementCurve;

	/* Posición inicial del elevador. */
	FVector StartElevatorLocation;

	/* Posición final del elevador. */
	FVector EndElevatorLocation;

	/* Comprobante de colisión del jugador. (Variable temporal puesto que cuando el pawn entre se desactivará su input. */
	bool bIsAlreadyIn = false;


	/* FUNCIONES DEL ACTOR */
	/* Ésta función se ejecuta en cada frame del Timeline. */
	UFUNCTION()
	void HandleTimelineProgress(float Value);

	/* Función para el inicio del overlap con el Pawn. */
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/* Ésta función configura el Timeline y lo inicializa de una forma u otra dependiendo de la circunstancia. */
	void SettingTimeline(FString PlatformType);

	/* Ésta función desactiva los controles del Pawn. */
	void DeactivatePawnInput();
};
