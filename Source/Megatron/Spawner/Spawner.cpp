#include "Spawner.h"
#include "../Pawns/Slime.h"


void ASpawner::BeginPlay()
{
	Super::BeginPlay();

	SetTeam(DebugTeam);
	SpawnTeam();
}

void ASpawner::SetTeam(FTeam InTeam)
{
	Team = InTeam;
}

void ASpawner::SpawnTeam()
{
	for (FSlimeInfo& SlimeInfo : Team.SlimeInfos)
	{
		FSpawnLocation* SpawnLocation = GetNextAvailableSpawnLocation();
		if (SpawnLocation)
		{
			ASlime* Slime = SpawnSlime(SlimeInfo, SpawnLocation->Location);
			SpawnLocation->OccupyingActor = Slime;
		}
	}
}

void ASpawner::RespawnTeam()
{
	// TODO: Respawn the team.
	return;
}

ASlime* ASpawner::SpawnSlime(FSlimeInfo& SlimeInfo, const FVector& Location)
{
	ASlime* Slime = nullptr;
	if (ensure(SlimeInfo.SlimeClass->IsChildOf(ASlime::StaticClass())))
	{
		Slime = Cast<ASlime>(GetWorld()->SpawnActor(SlimeInfo.SlimeClass, &Location));
		// Set the slime's abilities
		// Set the slime's passives
		SlimeInfo.Slime = Slime;
	}
	return Slime;
}

FSpawnLocation* ASpawner::GetNextAvailableSpawnLocation()
{
	for (int i = 0; i < SpawnLocations.Num(); i++)
	{
		if (SpawnLocations[i].OccupyingActor == nullptr)
		{
			return &SpawnLocations[i];
		}
	}
	return nullptr;
}