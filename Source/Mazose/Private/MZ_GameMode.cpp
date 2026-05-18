// José Manuel Rangel Muñoz. Copyright © Todos los derechos reservados (Excepto algunos assets).


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
	if (IsValid(TargetPlatformInstance))
	{
		TargetPlatformInstance->Destroy();
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
	Controller->PuttingHUD();

	/* Le doy al play a la música de fondo del nivel. */
	PlayingMusic();
}

void AMZ_GameMode::RegisterSpawnPlatform(AActor* Platform)
{
	/* Registro la plataforma en el Game Mode. */
	TargetPlatformInstance = Platform;
}


