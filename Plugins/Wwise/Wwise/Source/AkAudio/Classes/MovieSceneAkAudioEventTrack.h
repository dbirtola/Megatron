// Copyright (c) 2006-2016 Audiokinetic Inc. / All Rights Reserved

#pragma once

#include "MovieSceneAkTrack.h"
#include "IMovieScenePlayer.h"
#include "AkInclude.h"
#include "AkAudioEvent.h"
#if UE_4_26_OR_LATER
#include "Compilation/IMovieSceneTrackTemplateProducer.h"
#else
#include "MovieSceneBackwardsCompatibility.h"
#endif
#include "MovieSceneAkAudioEventTrack.generated.h"

class UMovieSceneAkAudioEventSection;

UCLASS(MinimalAPI)
class UMovieSceneAkAudioEventTrack
	: public UMovieSceneAkTrack
	, public IMovieSceneTrackTemplateProducer
{
	GENERATED_BODY()

public:
	UMovieSceneAkAudioEventTrack()
	{
#if WITH_EDITORONLY_DATA
		SetColorTint(FColor(0, 156, 255, 65));
#endif
	}

	AKAUDIO_API virtual UMovieSceneSection* CreateNewSection() override;
	virtual bool SupportsMultipleRows() const override { return true; }
	virtual bool SupportsType(TSubclassOf<UMovieSceneSection> SectionClass) const override;

	AKAUDIO_API virtual FName GetTrackName() const override;

#if WITH_EDITORONLY_DATA
	AKAUDIO_API virtual FText GetDisplayName() const override;
#endif

	AKAUDIO_API bool AddNewEvent(FFrameNumber Time, UAkAudioEvent* Event, const FString& EventName = FString());

    void WorkUnitChangesDetectedFromSection(UMovieSceneAkAudioEventSection* in_pSection);

protected:
	AKAUDIO_API virtual FMovieSceneEvalTemplatePtr CreateTemplateForSection(const UMovieSceneSection& InSection) const override;
};
