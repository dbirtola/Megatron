// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "MegatronMathLibrary.generated.h"

/**
 * Megatron MATH LIBRARY
 * Library for any general Math Functions not provided already by the Kismet Math Library. 
 * The .inl file should store implementations for inherently inlined functions.
 * The .cpp file should store implementations for all other functions.
 */

UCLASS(meta = (BlueprintThreadSafe, ScriptName = "MathLibrary"))
class MEGATRON_API UMegatronMathLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	//
	// Vector (2D) functions.
	//

	/** Linearly interpolates between A and B based on Alpha (100% of A when Alpha=0 and 100% of B when Alpha=1) */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Lerp (Vector 2D)", ScriptMethod = "LerpTo"), Category = "Math|Vector 2D")
	static FVector2D V2DLerp(FVector2D A, FVector2D B, float Alpha);
};

// Conditionally inlined
#if KISMET_MATH_INLINE_ENABLED
#include "MegatronMathLibrary.inl"
#endif
