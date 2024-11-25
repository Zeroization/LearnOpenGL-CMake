#pragma once

namespace GLCore
{
	enum class AnimBlendOption : int
	{
		None = 0,
		SimpleLERP = 1 << 0,
		CrossFading = 1 << 1,
		PartialSkeleton = 1 << 2,
		Additive = 1 << 3
	};

	inline AnimBlendOption operator| (AnimBlendOption lhs, AnimBlendOption rhs)
	{
		return static_cast<AnimBlendOption>(static_cast<int>(lhs) | static_cast<int>(rhs));
	}

	inline AnimBlendOption operator& (AnimBlendOption lhs, AnimBlendOption rhs)
	{
		return static_cast<AnimBlendOption>(static_cast<int>(lhs) & static_cast<int>(rhs));
	}

	inline AnimBlendOption operator^ (AnimBlendOption lhs, AnimBlendOption rhs)
	{
		return static_cast<AnimBlendOption>(static_cast<int>(lhs) ^ static_cast<int>(rhs));
	}

	inline AnimBlendOption operator~ (AnimBlendOption lhs)
	{
		return static_cast<AnimBlendOption>(~static_cast<int>(lhs));
	}

	inline AnimBlendOption& operator|= (AnimBlendOption& lhs, AnimBlendOption rhs)
	{
		lhs = lhs | rhs;
		return lhs;
	}

	inline AnimBlendOption& operator&= (AnimBlendOption& lhs, AnimBlendOption rhs)
	{
		lhs = lhs & rhs;
		return lhs;
	}

	// È¡Ïûrhs
	inline AnimBlendOption& operator-= (AnimBlendOption& lhs, AnimBlendOption rhs)
	{
		lhs &= ~rhs;
		return lhs;
	}
}
