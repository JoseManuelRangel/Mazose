// Fill out your copyright notice in the Description page of Project Settings.


#include "MZ_GameMode.h"

#include "Components/AudioComponent.h"

#include "Blueprint/UserWidget.h"

#include "Player/BallPlayerController.h"

#include "Kismet/GameplayStatics.h"

AMZ_GameMode::AMZ_GameMode()
{
	/* Inicializo el componente de audio y desactivo la opción de auto activarse. */
	LevelMusic = CreateDefaultSubobject<UAudioComponent>(TEXT("MusicComponent"));
	LevelMusic->bAutoActivate = false;
}

void AMZ_GameMode::BeginPlay()
{
	Super::BeginPlay();

	/* Cuando OnPlaySound emita una señal, ejecuta la función PlayingMusic. */
	OnPlaySound.AddDynamic(this, &AMZ_GameMode::PlayingMusic);

	/* Cuando OnStopSound emita una señal, ejecuta la función QuittingMusic. */
	OnStopSound.AddDynamic(this, &AMZ_GameMode::QuittingMusic);

	/* Llamo al evento que dispara la cuenta atrás con el user widget específico. */
	SettingCountdownOnPlayableLevels();
}

void AMZ_GameMode::PlayingMusic()
{
	/* Si el componente de audio no es válido anulo la función. */
	if (!LevelMusic)
	{
		return;
	}

	/* En caso de ser válido la ejecuto. */
	LevelMusic->Play();
}

void AMZ_GameMode::QuittingMusic()
{
	/* Si el componente de audio no es válido anulo la función. */
	if (!LevelMusic)
	{
		return;
	}

	/* En caso de ser válido la detengo. */
	LevelMusic->Stop();
}

void AMZ_GameMode::SettingCountdownOnPlayableLevels()
{
	/* Obtengo el nombre del nivel. */
	FString LevelName = GetWorld()->GetMapName();

	/* Limpio la cadena porque la función GetMapName viene con prefijos. */
	LevelName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

	/* Si empieza con "L_". */
	if (LevelName.StartsWith(TEXT("L_")))
	{
		CountdownSet();
	}
}

void AMZ_GameMode::CountdownSet()
{
	/* Early return en caso de que no exista esa clase. */
	if (!CountdownWidgetClass)
	{
		return;
	}

	/* Obtengo el player controller del personaje. */
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	/* Early return en caso de que no exista el player controller. */
	if (!PC)
	{
		return;
	}

	/* Creo el widget de la cuenta atrás. */
	CurrentCountdownWidget = CreateWidget<UUserWidget>(PC, CountdownWidgetClass);

	/* Si es válido... */
	if (CurrentCountdownWidget)
	{
		/* Lo añado al viewport. */
		CurrentCountdownWidget->AddToViewport();
	}
}

void AMZ_GameMode::DestroyingRespawnPlatform()
{
	/* Early return para comprobar que se ha asignado la clase correctamente. */
	if (!RespawnPlatformClass)
	{
		return;
	}

	/* Cojo todos los actores de esa clase y los vuelco en el array. */
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), RespawnPlatformClass, FoundActors);

	/* Si la longitud de ese array es de 1... */
	if (FoundActors.Num() == 1)
	{
		/* Cojo el primer y único actor del array. */
		AActor* TargetPlatform = FoundActors[0];

		/* Compruebo su validez... */
		if (IsValid(TargetPlatform))
		{
			TargetPlatform->Destroy();
		}
	}

	/* Obtener el player controller. */
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	/* Le hago un casting a mi player controller para poder llamar al event dispatcher. */
	ABallPlayerController* Controller = Cast<ABallPlayerController>(PC);

	/* Early return para comprobar su validez. */
	if (!Controller)
	{
		return;
	}

	/* Me suscribo al evento y lo llamo para ejecutarlo. */
	Controller->OnPuttingHUD.Broadcast();
}


