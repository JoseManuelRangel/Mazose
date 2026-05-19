// José Manuel Rangel Muñoz. Copyright © Todos los derechos reservados (Excepto algunos assets).



#include "Widgets/MZ_LevelWidget.h"

#include "Blueprint/UserWidget.h"

#include "Player/BallPlayerController.h"

#include "Kismet/GameplayStatics.h"


void UMZ_LevelWidget::NativeConstruct()
{
	Super::NativeConstruct();

	/* Le hacemos un casteo al Player Controller base. */
	Controller = Cast<ABallPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	/* Inicializo el número de fresas. */
	Txt_Strawberries->SetText(FText::FromString(FString::Printf(TEXT("0"))));

	/* Inicializo el número de monedas. */
	Txt_Coins->SetText(FText::FromString(FString::Printf(TEXT("0"))));

	/* Ejecución de la animación. */
	PlayAnimation(Strawberries, 0.0f, 0, EUMGSequencePlayMode::Forward, 0.5f);

	/* Ejecución de la animación. */
	PlayAnimation(Coins, 0.0f, 0, EUMGSequencePlayMode::Forward, 0.5f);
}

void UMZ_LevelWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	/* Obtener el controlador si no lo tengo. */
	if(!Controller)
	{
		return;
	}

	/* Si es válido... */
	if (Controller)
	{
		/* Si no está pausado el controlador... */
		if (!(Controller->bIsPaused))
		{
			/* Si el jugador está vivo... */
			if (bPlayerIsAlive)
			{
				ActiveTimer();
			}
			else 
			{
				/* Evitamos que esto se ejecute en bucle cada frame del Tick. */
				if (!GetWorld()->GetTimerManager().IsTimerActive(ScoreDelayTimerHandle))
				{
					/* Seteamos el record del jugador. */
					FTimespan Timespan = FTimespan::FromSeconds(Time);
					PlayerRecord = FText::FromString(FString::Printf(TEXT("%02d:%02d"),
						Timespan.GetMinutes(), Timespan.GetSeconds()));

					/* Metemos el record en el textbox. */
					if (Txt_Record_Time)
					{
						Txt_Record_Time->SetText(PlayerRecord);
					}

					/* Tras un delay de 2.0s creamos el widget. */
					GetWorld()->GetTimerManager().SetTimer(ScoreDelayTimerHandle, this, &UMZ_LevelWidget::ShowScoreHUD, 2.0f, false);
				}
			}
		}
	}
}

void UMZ_LevelWidget::AddingStrawberrries()
{
	/* Incremento el contador de fresas 1 más. */
	StrawberriesCount++;

	/* Early return para comprobar validez del textbox. */
	if (!Txt_Strawberries)
	{
		return;
	}

	/* Seteo la cantidad en el textbox. */
	Txt_Strawberries->SetText(FText::AsNumber(StrawberriesCount));
}

void UMZ_LevelWidget::AddingCoins()
{
	/* Incremento el contador de monedas 1 más. */
	CoinsCount++;

	/* Early return para comprobar validez del textbox. */
	if (!Txt_Coins)
	{
		return;
	}

	/* Seteo la cantidad en el textbox. */
	Txt_Coins->SetText(FText::AsNumber(CoinsCount));
}

void UMZ_LevelWidget::UpdatingBallIndicatorPosition()
{
	/* Si el jugador está por debajo de la primera altura se ejecuta la segunda parte de la animación.*/
	if (bPlayerIsLower)
	{
		/* Ejecución de animación (2º parte). */
		PlayAnimation(BallIndicatorLevel, 1.0f, 1, EUMGSequencePlayMode::Forward, 1.0f);
	}
	else 
	{
		/* Ejecución de animación (1º parte). */
		PlayAnimationTimeRange(BallIndicatorLevel, 0.0f, 1.0f, 1, EUMGSequencePlayMode::Forward, 1.0f);

		/* Notifico que el jugador está debajo del primer nivel. */
		bPlayerIsLower = true;
	}

	/* Early return para comprobar la validez de la animación. */
	if (!BallArrowIndicator)
	{
		return;
	}

	/* Ejecución de la animación. */
	PlayAnimation(BallArrowIndicator, 0.0f, 1, EUMGSequencePlayMode::Forward, 1.0f);
}

void UMZ_LevelWidget::SettingAliveToFalse()
{
	/* Early return en caso de que no exista esa clase. */
	if (!ScoreWidgetClass)
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

	/* Creo el widget de puntuación. */
	CurrentScoreWidget = CreateWidget<UUserWidget>(PC, ScoreWidgetClass);

	/* Si es válido.. */
	if (CurrentScoreWidget)
	{
		/* Lo añado al viewport. */
		CurrentScoreWidget->AddToViewport();
	}

	/* Pauso el juego. */
	UGameplayStatics::SetGamePaused(GetWorld(), true);

	/* Quito la interfaz del nivel. */
	this->RemoveFromParent();
}

void UMZ_LevelWidget::ActiveTimer()
{
	/* Calculo el tiempo sumando la cantidad actual + los delta seconds para independizar al tick de los FPS. */
	Time += UGameplayStatics::GetWorldDeltaSeconds(GetWorld());

	/* Obtenemos los segundos del tiempo. */
	int32 Seconds = FMath::FloorToInt(Time);

	/* Early return del textblock del */
	if (!Txt_Time)
	{
		return;
	}

	/* Transformo el tiempo de segundos a formato 00:00:00 y lo pego al textblock usando FTimespan para el formato de tiempo. */
	FTimespan Timespan = FTimespan::FromSeconds(Time);

	/* Conseguimos la parte decimal. */
	float ParteDecimal = Time - Seconds;

	/* Obtenemos los MilliSeconds. */
	int32 MilliSeconds = FMath::FloorToInt(ParteDecimal * 100.0f);

	/* Formateo como MM:SS o similar. */
	FString TimeString = FString::Printf(TEXT("%02d:%02d:%02d"),
		Timespan.GetMinutes(),
		Timespan.GetSeconds(),
		MilliSeconds);

	/* Seteo el texto en el textbox. */
	Txt_Time->SetText(FText::FromString(TimeString));
}

void UMZ_LevelWidget::ShowScoreHUD()
{
	/* Early return para comprobar la clase. */
	if (!ScoreWidgetClass)
	{
		return;
	}

	/* Creación del widget de puntuación. */
	UUserWidget* ScoreWidget = CreateWidget<UUserWidget>(GetOwningPlayer(), ScoreWidgetClass);

	/* Early return para comprobar el widget. */
	if (!ScoreWidget)
	{
		return;
	}

	/* Lo añado al viewport. */
	ScoreWidget->AddToViewport();
}
