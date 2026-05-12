// José Manuel Rangel Muñoz. Copyright © Todos los derechos reservados (Excepto algunos assets).

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "Components/SphereComponent.h"

#include "StagesRespawner.generated.h"

UCLASS()
class MAZOSE_API AStagesRespawner : public AActor
{
	GENERATED_BODY()
	
public:
	/* Constructor. */
	AStagesRespawner();

protected:
	/* Begin Play: Se llama cuando empieza el juego. */
	virtual void BeginPlay() override;

public:
	/* Mesh que compone la base del respawner. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Respawner Structure")
	UStaticMeshComponent* RespawnerBase;

	/* Segundo mesh que compone el mesh de dentro del primero. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Respawner Structure")
	UStaticMeshComponent* RespawnerInsideMesh;

	/* Esfera de colisión que tendrá el respawner. */
	UPROPERTY(EditAnywhere, Category = "Respawner Structure")
	USphereComponent* RespawnerCollision;

	/* Sonido para cuando el jugador consiga el power up. */
	UPROPERTY(EditAnywhere, Category = "Respawner Audio")
	USoundBase* RespawnerSound;

	/* Tag que debe coincidir con las plataformas asociadas al incremento. */
	UPROPERTY(EditAnywhere, Category = "Respawner Structure")
	FName RespawnerTag;


public:
	/* Tick: Se llama a cada frame. */
	virtual void Tick(float DeltaTime) override;

	/* Función para el inicio del overlap del stage con el Pawn. */
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
