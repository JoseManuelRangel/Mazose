// Fill out your copyright notice in the Description page of Project Settings.



#include "Widgets/MZ_LevelWidget.h"

#include "Blueprint/UserWidget.h"

#include "Kismet/GameplayStatics.h"


void UMZ_LevelWidget::NativeConstruct()
{
	Super::NativeConstruct();

	/* Cuando OnAddingStrawberries emita una señal, ejecuta la función AddingStrawberries. */
	OnAddingStrawberries.AddDynamic(this, &UMZ_LevelWidget::AddingStrawberrries);

	/* Cuando OnUpdateBallIndicatorPosition emita una señal, ejecuta la función UpdatingBallIndicatorPosition. */
	OnUpdateBallIndicatorPosition.AddDynamic(this, &UMZ_LevelWidget::UpdatingBallIndicatorPosition);

	/* Cuando OnSettingIsAliveToFalse emita una señal, ejecuta la función SettingAliveToFalse. */
	OnSettingIsAliveToFalse.AddDynamic(this, &UMZ_LevelWidget::SettingAliveToFalse);
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
