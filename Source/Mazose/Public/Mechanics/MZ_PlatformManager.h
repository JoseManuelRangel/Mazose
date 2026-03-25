// José Manuel Rangel Muñoz. Copyright © Todos los derechos reservados (Excepto algunos assets).

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "Math/TransformNonVectorized.h"

#include "MZ_PlatformManager.generated.h"

UCLASS()
class MAZOSE_API AMZ_PlatformManager : public AActor
{
	GENERATED_BODY()
	
public:
	AMZ_PlatformManager();

	/* Constructor que se ejecuta cada vez que se cambia una propiedad en el editor. */
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	virtual void BeginPlay() override;

protected:
	/* La plataforma que se generará. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Generator")
	TSubclassOf<AActor> PlatformClass;

	/* El número de plataformas que quiero generar. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Generator")
	int32 NumberOfPlatforms = 10;

	/* El espacio entre una plataforma y otra. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Generator")
	float Spacing = 50.0f;

	/* Almaceno los actores generados para que Unreal los limpie correctamente. */
	UPROPERTY()
	TArray<UChildActorComponent*> PlatformComponents;

public:
	virtual void Tick(float DeltaTime) override;

};
