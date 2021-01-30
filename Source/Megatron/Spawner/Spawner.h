#pragma once

#include "GameFramework/Actor.h"
#include "Spawner.generated.h"

class ASlime;

// Structure to contain all the data we should ever need to recreate a slime
USTRUCT(BlueprintType)
struct FSlimeInfo
{
	GENERATED_BODY()

	AController* OwningController;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UClass* SlimeClass;
	// The current slime spawned from this info, if one exists.
	ASlime* Slime;
	// TArray<SlimeAbilities>
	// TArray<SlimePassives>
};

USTRUCT(BlueprintType)
struct FTeam
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	AController* OwningController;

	// Spawn info's used to spawn or respawn this team at the start of combat.
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TArray<FSlimeInfo> SlimeInfos;
};

USTRUCT(BlueprintType)
struct FSpawnLocation
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FVector Location = FVector::ZeroVector;
	UPROPERTY(BlueprintReadOnly)
	AActor* OccupyingActor;

	bool IsValid() { return Location != FVector::ZeroVector; }
};

UCLASS(Blueprintable, BlueprintType)
class ASpawner : public AActor
{
	GENERATED_BODY()

	FTeam Team;

public:

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FTeam DebugTeam;

	void BeginPlay() override;

	// Assigns a team to this spawner.
	void SetTeam(FTeam InTeam);

	// Spawns the currently assigned team. Be sure you use SetTeam to assign a team before calling this.
	void SpawnTeam();
	void RespawnTeam();
	ASlime* SpawnSlime(FSlimeInfo& SlimeInfo, const FVector& Location);
	FSpawnLocation* GetNextAvailableSpawnLocation();

public:
	// Locations, in order, in which the team will be spawned. TODO: STRETCH: Any team members that do not fit should be queued for when a team member dies.
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FSpawnLocation> SpawnLocations;
};