// José Manuel Rangel Muñoz. Copyright © Todos los derechos reservados (Excepto algunos assets).

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "Components/SphereComponent.h"

class UNiagaraComponent;
class UNiagaraSystem;
class UAudioComponent;
class USoundBase;

#include "MZ_Coin.generated.h"

UCLASS()
class MAZOSE_API AMZ_Coin : public AActor
{
	GENERATED_BODY()
	
public:
	AMZ_Coin();

protected:
	virtual void BeginPlay() override;

protected:
	/* Variables protegidas de la clase. */
	/* Mesh de la moneda. */
	UPROPERTY(EditAnywhere, Category = "Coin Structure")
	UStaticMeshComponent* CoinMesh;

	/* Esfera de colisión de la moneda. */
	UPROPERTY(VisibleAnywhere)
	USphereComponent* CoinCollision;

	/* Componente de partículas de la moneda. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Coin Structure");
	UNiagaraComponent* CoinParticles;

	/* Asset de partículas de la moneda. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Coin Structure");
	UNiagaraSystem* CoinParticlesAsset;

	/* Componente de audio de la moneda. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Coin Structure")
	UAudioComponent* CoinAudio;

	/* Asset de audio de la moneda. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Coin Structure")
	USoundBase* CoinAudioAsset;

private:
	/* Variables privadas de la clase. */
	/* Booleano que controla si la moneda ha sido recogida o no. */
	bool bWasCollected = false;

public:
	/* Funciones públicas de la clase. */
	/* Función que activa el sonido y destruye la moneda. */
	UFUNCTION(BlueprintCallable, Category = "Coin Functions")
	void CollectingCoin();

	/* Función para el inicio del overlap del stage con el Pawn. */
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
