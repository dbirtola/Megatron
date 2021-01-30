#pragma once

#include "GameFramework/PlayerController.h"
#include "../Spawner/Spawner.h"
#include "MegatronPlayerController.generated.h"



UCLASS(Blueprintable, BlueprintType)
class AMegatronPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	void BeginPlay() override;

	void SetTeam(FTeam InTeam);

public:

	// Initial team the player will start with when they start the game.
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FTeam Team;

	FTeam GetTeam();
};