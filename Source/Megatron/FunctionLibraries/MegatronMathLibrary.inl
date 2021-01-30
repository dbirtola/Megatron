// Fill out your copyright notice in the Description page of Project Settings.

// This file should be solely used for inline math functions.

#pragma once

// Copied from C:\NP4\UE_4.24\Engine\Source\Runtime\Engine\Classes\Kismet\KismetMathLibrary.inl
// Our own easily changed inline options
#if KISMET_MATH_INLINE_ENABLED
	#define KISMET_MATH_FORCEINLINE		FORCEINLINE_DEBUGGABLE
	#define KISMET_MATH_INLINE			inline
#else
	#define KISMET_MATH_FORCEINLINE		// nothing
	#define KISMET_MATH_INLINE			// nothing
#endif

KISMET_MATH_FORCEINLINE
FVector2D UMegatronMathLibrary::V2DLerp(FVector2D A, FVector2D B, float V)
{
	return A + V * (B - A);
}
