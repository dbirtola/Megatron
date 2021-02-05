#include "Spawner.h"
#include "../Pawns/Slime.h"
#include "../Components/HealthComponent.h"
#include "../MegatronGameModeBase.h"
#include "../FunctionLibraries/MegatronFunctionLibrary.h"


void ASpawner::BeginPlay()
{
	Super::BeginPlay();

/*
	SetTeam(DebugTeam);
	SpawnTeam();*/
}

void ASpawner::SetTeam(FTeam InTeam)
{
	Team = InTeam;
}

void ASpawner::RandomizeTeam()
{
	if (AMegatronGameModeBase* MegatronGameMode = UMegatronFunctionLibrary::GetMegatronGameMode(this))
	{
		int32 Round = MegatronGameMode->CurrentLevel;
		
		TArray<FTeam> FilteredTeams;
		for (FTeam CandidateTeam : PossibleTeams)
		{
			if (CandidateTeam.MinimumPossibleRound < Round && CandidateTeam.MaximumPossibleRound > Round)
			{
				FilteredTeams.Add(CandidateTeam);
			}
		}

		int32 Index = FMath::RandRange(0, FilteredTeams.Num() - 1);
		SetTeam(FilteredTeams[Index]);
	}
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

TArray<ASlime*> ASpawner::GetSpawnedSlimeActors(bool bAliveOnly)
{
	TArray<ASlime*> SpawnedSlimes;
	for (FSlimeInfo SlimeInfo : Team.SlimeInfos)
	{
		if (SlimeInfo.Slime)
		{
			if (bAliveOnly)
			{
				if (SlimeInfo.Slime->HealthComponent->CurrentHealth <= 0)
				{
					continue;
				}
			}
			SpawnedSlimes.Add(SlimeInfo.Slime);
		}
	}
	return SpawnedSlimes;
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

PRAGMA_DISABLE_OPTIMIZATION

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

void ASpawner::CleanupDeadSlimes()
{
	for (int i = Team.SlimeInfos.Num() - 1; i >= 0; i--)
	{
		FSlimeInfo SlimeInfo = Team.SlimeInfos[i];
		if (SlimeInfo.Slime->HealthComponent->CurrentHealth <= 0)
		{
			GetWorld()->DestroyActor(SlimeInfo.Slime);
			SlimeInfo.Slime = nullptr;
		}
	}
	// Clean up locations
	for (int i = SpawnLocations.Num() - 1; i >= 0; i--)
	{
		if (!IsValid(SpawnLocations[i].OccupyingActor))
		{
			SpawnLocations[i].OccupyingActor = nullptr;
		}
	}
	UE_LOG(LogTemp, Log, TEXT("Finished cleaning up slimes"));
}

PRAGMA_ENABLE_OPTIMIZATION