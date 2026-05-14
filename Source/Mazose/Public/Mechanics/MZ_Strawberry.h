// José Manuel Rangel Muñoz. Copyright © Todos los derechos reservados (Excepto algunos assets).

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "Components/SphereComponent.h"
#include "Components/TimelineComponent.h"

#include "MZ_Strawberry.generated.h"

UCLASS()
class MAZOSE_API AMZ_Strawberry : public AActor
{
	GENERATED_BODY()
	
public:	
	/* Constructor */
	AMZ_Strawberry();

protected:
	/* Begin Play: Se llama cuando empieza el juego. */
	virtual void BeginPlay() override;

	/* Variables de la clase. */
	/* Mesh que compondrá la estructura de la fresa. */
	UPROPERTY(EditAnywhere, Category = "Strawberry Structure")
	UStaticMeshComponent* MeshComponent;

	/* Esfera de colisión para la fresa.*/
	UPROPERTY(VisibleAnywhere, Category = "Strawberry Structure")
	USphereComponent* CollisionComponent;

	/* Timeline para la desaparición de la fresa. */
	UPROPERTY()
	UTimelineComponent* DisappearingTimeline;

	/* Curva para controlar la escala del valor del timeline. */
	UPROPERTY(EditAnywhere, Category = "Strawberry Setup")
	UCurveFloat* ScaleCurve;

	/* Funciones de la clase. */
	/* Función que se ejecuta en cada paso de la Timeline. */
	UFUNCTION()
	void TimelineProgress(float Value);

	/* Función que se ejecuta al terminar la Timeline. */
	UFUNCTION()
	void TimelineFinished();

	/* Evento de overlap. */
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	/* Tick: Se llama a cada frame. */
	virtual void Tick(float DeltaTime) override;

private:
	/* Booleano que controla si la fresa está colectada o no. */
	bool bWasCollected = false;

};
