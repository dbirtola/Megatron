// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Framework/MegatronTypes.h"
#include "Components/ActorComponent.h"
#include "Components/AudioComponent.h"
#include "Components/SplineComponent.h"
#include "MegatronUtilitiesLibrary.generated.h"

/**
 * 
 */
UCLASS()
class MEGATRON_API UMegatronUtilitiesLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "File I/0")
	static FString LoadFileToString(FString Filename);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "File I/0")
	static TArray<FString> LoadFileToStringArray(FString Filename);
   
	UFUNCTION(BlueprintCallable, Category = "File I/0")
	static void SaveStringToFile(FString Filename, FString input);

	UFUNCTION(BlueprintCallable, Category = "File I/0")
	static void SaveStringArrayToFile(FString Filename, TArray<FString> input);

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", CallableWithoutWorldContext, Keywords = "log print to channel", AdvancedDisplay = "3", DevelopmentOnly), Category = "Utilities|String")
	static void PrintStringToChannel(UObject* WorldContextObject, const FString& InString = FString(TEXT("Hello")), const int channel = 7777, bool bPrintToScreen = true, bool bPrintToLog = true, FLinearColor TextColor = FLinearColor(0.0, 0.33, 1.0), float Duration = 2.f);

	UFUNCTION(BlueprintPure, Category = "Utility")
	static BuildConfig GetBuildConfiguration();

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
    static AActor* SpawnFromWorld(UObject* WorldContextObject, UClass* InClass, const FVector& Position, const FRotator& Rotation);

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (WorldContext = "WorldContextObject"))
	static UWorld* GetWorldRef(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "Component", meta = (DevelopmentOnly))
	static void SetComponentIsEditorOnly(UActorComponent* Target, bool bIsEditorOnly);

	//static void AdjustPitchInternal(UAudioComponent* target, float AdjustVolumeDuration, float AdjustVolumeLevel, bool bIsFadeOut, const EAudioFaderCurve FadeCurve)

	UFUNCTION(BlueprintCallable, BlueprintPure)
    static float GetAngleNorth360(const float AngleNorth, const float AngleEast);

	UFUNCTION(BlueprintPure, Category = "Utilities")
	static bool ProjectWorldToScreenBidirectional(APlayerController const* Player, const FVector& WorldPosition, FVector2D& ScreenPosition, bool& bTargetBehindCamera, bool bPlayerViewportRelative = false);
};
