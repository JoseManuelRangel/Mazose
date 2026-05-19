// José Manuel Rangel Muñoz. Copyright © Todos los derechos reservados (Excepto algunos assets).


#include "Widgets/MZ_CountdownWidget.h"

#include "MZ_GameMode.h"

#include "Kismet/GameplayStatics.h"


void UMZ_CountdownWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!SecondsAnimation)
	{
		return;
	}

	/* Vinculo la función al delegado de finalización. */
	AnimationFinishedDelegate.BindDynamic(this, &UMZ_CountdownWidget::HandleAnimationFinished);
	BindToAnimationFinished(SecondsAnimation, AnimationFinishedDelegate);

	/* Ejecutamos la animación. */
	PlayAnimation(SecondsAnimation, 0.0f, 1, EUMGSequencePlayMode::Forward, 1.0f);
}

void UMZ_CountdownWidget::OnCountdownAnimationFinished()
{
	/* Obtengo el Game Mode. */
	AGameModeBase* GM = UGameplayStatics::GetGameMode(GetWorld());

	/* Early return para comprobar que el Game Mode existe y es válido. */
	if (!GM)
	{
		return;
	}

	/* Le hago un cast al Game Mode creado por mí. */
	AMZ_GameMode* GameMode = Cast<AMZ_GameMode>(GM);

	/* Early return par comprobar que el Game Mode creado existe y es válido. */
	if (!GameMode)
	{
		return;
	}

	/* Llamo a la función del gamemode para destruir la plataforma de respawn. */
	GameMode->DestroyingRespawnPlatform();

	/* Elimino este widget. */
	this->RemoveFromParent();
}

void UMZ_CountdownWidget::HandleAnimationFinished()
{
	/* Llamo a la función que se encarga de destruir la plataforma y crear el widget del nivel. */
	OnCountdownAnimationFinished();
}
