// José Manuel Rangel Muñoz. Copyright © Todos los derechos reservados (Excepto algunos assets).

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MZ_Platform.generated.h"

UCLASS()
class MAZOSE_API AMZ_Platform : public AActor
{
	GENERATED_BODY()
	
public:
	AMZ_Platform();

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, Category = "Platform Structure")
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(EditAnywhere, Category = "Platform Structure")
	UStaticMeshComponent* BorderMesh;
};
