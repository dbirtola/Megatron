// Fill out your copyright notice in the Description page of Project Settings.


#include "MegatronUtilitiesLibrary.h"

#include "Engine/Console.h"
#include "CoreGlobals.h"
#include "Misc/FileHelper.h"
#include "Engine/GameEngine.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/GameViewportClient.h"
#include "Engine/LocalPlayer.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFilemanager.h"
#include "Kismet/KismetStringLibrary.h"
#include "AudioThread.h"
#include "AudioDevice.h"
#include "DSP/VolumeFader.h"

//https://www.youtube.com/watch?v=zuJ2tjZW770
FString UMegatronUtilitiesLibrary::LoadFileToString(FString Filename)
{
	FString directory = FPaths::ProjectContentDir();
	FString result;
	IPlatformFile& file = FPlatformFileManager::Get().GetPlatformFile();

	if (file.CreateDirectory(*directory))
	{
		FString myFile = directory + "/" + Filename; 
		FFileHelper::LoadFileToString(result, *myFile);
	}

	return result;   
}


//https://www.youtube.com/watch?v=zuJ2tjZW770
TArray<FString> UMegatronUtilitiesLibrary::LoadFileToStringArray(FString Filename)
{
	FString directory = FPaths::ProjectContentDir();
	TArray<FString> result;
	IPlatformFile& file = FPlatformFileManager::Get().GetPlatformFile();

	if (file.CreateDirectory(*directory))
	{
		FString myFile = directory + "/" + Filename;  
		FFileHelper::LoadFileToStringArray(result, *myFile);
	}

	return result;
}

void UMegatronUtilitiesLibrary::SaveStringToFile(FString Filename, FString input)
{

	FString directory = FPaths::ProjectContentDir();
	TArray<FString> result;
	IPlatformFile& file = FPlatformFileManager::Get().GetPlatformFile();

	//if (clearFile)
	//	file.DeleteFile(*Filename);

	if (file.CreateDirectory(*directory))
	{
		FString myFile = directory + "/" + Filename;
		FFileHelper::SaveStringToFile(input, *myFile);
	}
}

void UMegatronUtilitiesLibrary::SaveStringArrayToFile(FString Filename, TArray<FString> input)
{
	FString directory = FPaths::ProjectContentDir();
	TArray<FString> result;
	IPlatformFile& file = FPlatformFileManager::Get().GetPlatformFile();

	//if (clearFile)
	//	file.DeleteFile(*Filename);

	if (file.CreateDirectory(*directory))
	{
		FString myFile = directory + "/" + Filename;
		FFileHelper::SaveStringArrayToFile(input, *myFile);
	}
}

void UMegatronUtilitiesLibrary::PrintStringToChannel(UObject * WorldContextObject, const FString & InString, const int channel, bool bPrintToScreen, bool bPrintToLog, FLinearColor TextColor, float Duration)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST) // Do not Print in Shipping or Test

	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
	FString Prefix;
	if (World)
	{
		if (World->WorldType == EWorldType::PIE)
		{
			switch (World->GetNetMode())
			{
			case NM_Client:
				Prefix = FString::Printf(TEXT("Client %d: "), GPlayInEditorID - 1);
				break;
			case NM_DedicatedServer:
			case NM_ListenServer:
				Prefix = FString::Printf(TEXT("Server: "));
				break;
			case NM_Standalone:
				break;
			}
		}
	}

	const FString FinalDisplayString = Prefix + InString;
	FString FinalLogString = FinalDisplayString;

	static const FBoolConfigValueHelper DisplayPrintStringSource(TEXT("Kismet"), TEXT("bLogPrintStringSource"), GEngineIni);
	if (DisplayPrintStringSource)
	{
		const FString SourceObjectPrefix = FString::Printf(TEXT("[%s] "), *GetNameSafe(WorldContextObject));
		FinalLogString = SourceObjectPrefix + FinalLogString;
	}

	if (bPrintToLog)
	{
		UE_LOG(LogBlueprintUserMessages, Log, TEXT("%s"), *FinalLogString);

		APlayerController* PC = (WorldContextObject ? UGameplayStatics::GetPlayerController(WorldContextObject, 0) : NULL);
		ULocalPlayer* LocalPlayer = (PC ? Cast<ULocalPlayer>(PC->Player) : NULL);
		if (LocalPlayer && LocalPlayer->ViewportClient && LocalPlayer->ViewportClient->ViewportConsole)
		{
			LocalPlayer->ViewportClient->ViewportConsole->OutputText(FinalDisplayString);
		}
	}
	else
	{
		UE_LOG(LogBlueprintUserMessages, Verbose, TEXT("%s"), *FinalLogString);
	}

	// Also output to the screen, if possible
	if (bPrintToScreen)
	{
		if (GAreScreenMessagesEnabled)
		{
			if (GConfig && Duration < 0)
			{
				GConfig->GetFloat(TEXT("Kismet"), TEXT("PrintStringDuration"), Duration, GEngineIni);
			}
			GEngine->AddOnScreenDebugMessage((uint64)channel, Duration, TextColor.ToFColor(true), FinalDisplayString);
		}
		else
		{
			UE_LOG(LogBlueprint, VeryVerbose, TEXT("Screen messages disabled (!GAreScreenMessagesEnabled).  Cannot print to screen."));
		}
	}
#endif
}

EBuildConfig UMegatronUtilitiesLibrary::GetBuildConfiguration()
{
#if UE_BUILD_SHIPPING
	return EBuildConfig::SHIPPING;
#elif WITH_EDITOR
	return EBuildConfig::EDITOR;
#else
	return EBuildConfig::DEVELOPMENT;
#endif
}

AActor* UMegatronUtilitiesLibrary::SpawnFromWorld(UObject* WorldContextObject, UClass* InClass, const FVector& Position, const FRotator& Rotation)
{
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		return World->SpawnActor(InClass, &Position, &Rotation);
	}
	return nullptr;
}

UWorld* UMegatronUtilitiesLibrary::GetWorldRef(UObject* WorldContextObject)
{
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		return World;
	}
	return nullptr;
}

void UMegatronUtilitiesLibrary::SetComponentIsEditorOnly(UActorComponent * Target, bool bIsEditorOnly)
{
#if WITH_EDITOR
	Target->bIsEditorOnly = bIsEditorOnly;
#endif
}

float UMegatronUtilitiesLibrary::GetAngleNorth360(const float AngleNorth, const float AngleEast)
{
	float Result = 0;
	if(AngleEast < 90)
	{
		Result = AngleNorth;
	}
	else
	{
		Result = 360 - AngleNorth;
	}
	
	return Result;
}

bool UMegatronUtilitiesLibrary::ProjectWorldToScreenBidirectional(APlayerController const* Player, const FVector& WorldPosition, FVector2D& ScreenPosition, bool& bTargetBehindCamera, bool bPlayerViewportRelative)
{
	FVector Projected;
	bool bSuccess = false;
	bTargetBehindCamera = false; 

	ULocalPlayer* const LP = Player ? Player->GetLocalPlayer() : nullptr;
	if (LP && LP->ViewportClient)
	{
		// get the projection data
		FSceneViewProjectionData ProjectionData;
		if (LP->GetProjectionData(LP->ViewportClient->Viewport, eSSP_FULL, /*out*/ ProjectionData))
		{
			const FMatrix ViewProjectionMatrix = ProjectionData.ComputeViewProjectionMatrix();
			const FIntRect ViewRectangle = ProjectionData.GetConstrainedViewRect();

			FPlane Result = ViewProjectionMatrix.TransformFVector4(FVector4(WorldPosition, 1.f));
			if (Result.W < 0.f) { bTargetBehindCamera = true; }
			if (Result.W == 0.f) { Result.W = 1.f; } // Prevent Divide By Zero

			const float RHW = 1.f / FMath::Abs(Result.W);
			Projected = FVector(Result.X, Result.Y, Result.Z) * RHW;

			// Normalize to 0..1 UI Space
			const float NormX = (Projected.X / 2.f) + 0.5f;
			const float NormY = 1.f - (Projected.Y / 2.f) - 0.5f;

			Projected.X = (float)ViewRectangle.Min.X + (NormX * (float)ViewRectangle.Width());
			Projected.Y = (float)ViewRectangle.Min.Y + (NormY * (float)ViewRectangle.Height());

			bSuccess = true;
			ScreenPosition = FVector2D(Projected.X, Projected.Y);

			if (bPlayerViewportRelative)
			{
				ScreenPosition -= FVector2D(ProjectionData.GetConstrainedViewRect().Min);
			}
		}
		else
		{
			ScreenPosition = FVector2D(1234, 5678);
		}
	}

	return bSuccess;
}

/*
void UMegatronUtilitiesLibrary::AdjustPitchInternal(UAudioComponent* target, float AdjustVolumeDuration, float AdjustVolumeLevel, bool bIsFadeOut, const EAudioFaderCurve FadeCurve)
{
	if (!target->IsActive())
	{
		return;
	}

	FAudioDevice* AudioDevice = target->GetAudioDevice();
	if (!AudioDevice)
	{
		return;
	}

	AdjustVolumeDuration = FMath::Max(0.0f, AdjustVolumeDuration);
	AdjustVolumeLevel = FMath::Max(0.0f, AdjustVolumeLevel);
	if (FMath::IsNearlyZero(AdjustVolumeDuration) && FMath::IsNearlyZero(AdjustVolumeLevel))
	{
		target->Stop();
		return;
	}

	target->bIsFadingOut = bIsFadeOut || FMath::IsNearlyZero(AdjustVolumeLevel);

	const uint64 InAudioComponentID = target->GetAudioComponentID();
	DECLARE_CYCLE_STAT(TEXT("FAudioThreadTask.AdjustVolume"), STAT_AudioAdjustVolume, STATGROUP_AudioThreadCommands);
	FAudioThread::RunCommandOnAudioThread([AudioDevice, InAudioComponentID, AdjustVolumeDuration, AdjustVolumeLevel, bIsFadeOut, FadeCurve]()
	{
		FActiveSound* ActiveSound = AudioDevice->FindActiveSound(InAudioComponentID);
		if (!ActiveSound)
		{
			return;
		}

		Audio::FVolumeFader& Fader = ActiveSound->ComponentVolumeFader;
		const float InitialTargetVolume = Fader.GetTargetVolume();

		// Ignore fade out request if requested volume is higher than current target.
		if (bIsFadeOut && AdjustVolumeLevel >= InitialTargetVolume)
		{
			return;
		}

		const bool ToZeroVolume = FMath::IsNearlyZero(AdjustVolumeLevel);
		if (ActiveSound->FadeOut == FActiveSound::EFadeOut::Concurrency)
		{
			// Ignore adjust volume request if non-zero and currently voice stealing.
			if (!FMath::IsNearlyZero(AdjustVolumeLevel))
			{
				return;
			}

			// Ignore request of longer fade out than active target if active is concurrency (voice stealing) fade.
			if (AdjustVolumeDuration > Fader.GetFadeDuration())
			{
				return;
			}
		}
		else
		{
			ActiveSound->FadeOut = bIsFadeOut || ToZeroVolume ? FActiveSound::EFadeOut::User : FActiveSound::EFadeOut::None;
		}

		if (bIsFadeOut || ToZeroVolume)
		{
			// If negative, active indefinitely, so always make sure set to minimum positive value for active fade.
			const float OldActiveDuration = Fader.GetActiveDuration();
			const float NewActiveDuration = OldActiveDuration < 0.0f
				? AdjustVolumeDuration
				: FMath::Min(OldActiveDuration, AdjustVolumeDuration);
			Fader.SetActiveDuration(NewActiveDuration);
		}

		Fader.StartFade(AdjustVolumeLevel, AdjustVolumeDuration, static_cast<Audio::EFaderCurve>(FadeCurve));
	}, GET_STATID(STAT_AudioAdjustVolume));
}

void UAudioComponent::Stop()
{
	if (!IsActive())
	{
		return;
	}

	FAudioDevice* AudioDevice = GetAudioDevice();
	if (!AudioDevice)
	{
		return;
	}

	// Set this to immediately be inactive
	SetActiveFlag(false);

	UE_LOG(LogAudio, Verbose, TEXT("%g: Stopping AudioComponent : '%s' with Sound: '%s'"),
		GetWorld() ? GetWorld()->GetAudioTimeSeconds() : 0.0f, *GetFullName(),
		Sound ? *Sound->GetName() : TEXT("nullptr"));

	AudioDevice->StopActiveSound(AudioComponentID);
}
*/
