// Copyright 2017-2018 Csaba Molnar, Daniel Butum
#pragma once

#include "CoreTypes.h"

#include "DlgCondition.h"
#include "DlgEvent.h"
#include "DlgTextArgument.h"

#include "DlgEdge.generated.h"


class UDlgContextInternal;
class UDlgNode;

/**
 * The representation of a child in a node. Defined by a TargetIndex which points to the index array in the Dialogue.Nodes
 */
USTRUCT(BlueprintType, Blueprintable)
struct DLGSYSTEM_API FDlgEdge
{
	GENERATED_USTRUCT_BODY()

public:
	bool operator==(const FDlgEdge& Other) const
	{
		return TargetIndex == Other.TargetIndex &&
			Text.EqualTo(Other.Text) &&
			Conditions == Other.Conditions;
	}

	bool operator!=(const FDlgEdge& Other) const
	{
		return !(*this == Other);
	}

	// Operator overload for serialization
	friend FArchive& operator<<(FArchive &Ar, FDlgEdge& DlgEdge);

	/** Creates a simple edge without text, without conditions */
	FDlgEdge(int32 InTargetIndex = INDEX_NONE) : TargetIndex(InTargetIndex) {}

	/** Returns with true if every condition attached to the edge and every enter condition of the target node are satisfied */
	bool Evaluate(const UDlgContextInternal* DlgContext, TSet<const UDlgNode*> AlreadyVisitedNodes) const;

	void ConstructTextFromArguments(UDlgContextInternal* DlgContext, FName NodeOwnerName);

	/** Returns if the Edge is valid, has the TargetIndex non negative  */
	bool IsValid() const
	{
		return TargetIndex > INDEX_NONE;
	}

	static const FDlgEdge& GetInvalidEdge();

public:
	/** Index of the node in the Nodes TArray of the dialogue this edge is leading to */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = DlgEdgeData, Meta = (ClampMin = -1))
	int32 TargetIndex = INDEX_NONE;

	/** Required but not sufficient conditions - target node's enter conditions are checked too */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = DlgEdgeData)
	TArray<FDlgCondition> Conditions;

	/** Text associated with the child, can be used for user choices */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = DlgEdgeData, Meta = (MultiLine = true))
	FText Text;

	UPROPERTY(EditAnywhere, EditFixedSize, Category = DlgEdgeData)
	TArray<FDlgTextArgument> TextArguments;

	/** Empty text, or Text formatted with the text arguments if there is any, and the parent node is enterred */
	UPROPERTY(BlueprintReadOnly)
	FText ConstructedText;

	/** player emotion/state attached to this player choice */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = DlgEdgeData)
	FName SpeakerState;

	/** Set this to false in order to skip this edge in the AllChildren array (which lists both satisfied and not satisfied player choices */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = DlgEdgeData)
	bool bIncludeInAllOptionListIfUnsatisfied = true;
};
