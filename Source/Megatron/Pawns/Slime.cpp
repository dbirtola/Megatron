#include "Slime.h"
#include "../Components/HealthComponent.h"

ASlime::ASlime(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	HealthComponent = CreateDefaultSubobject<UHealthComponent>("Health");
}


void ASlime::BeginPlay()
{
	Super::BeginPlay();
}
