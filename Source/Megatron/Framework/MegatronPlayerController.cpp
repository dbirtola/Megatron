#include "MegatronPlayerController.h"


void AMegatronPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void AMegatronPlayerController::SetTeam(FTeam InTeam)
{
	Team = InTeam;
	Team.OwningController = this;
}

FTeam AMegatronPlayerController::GetTeam()
{
	return Team;
}

