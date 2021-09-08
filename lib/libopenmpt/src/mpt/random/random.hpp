/* SPDX-License-Identifier: BSL-1.0 OR BSD-3-Clause */

#ifndef MPT_RANDOM_RANDOM_HPP
#define MPT_RANDOM_RANDOM_HPP



#include "mpt/base/namespace.hpp"
#include "mpt/random/engine.hpp"

#include <type_traits>


namespace mpt {
inline namespace MPT_INLINE_NS {



template <typename T, typename Trng>
inline T random(Trng & rng) {
	static_assert(std::numeric_limits<T>::is_integer);
	typedef typename std::make_unsigned<T>::type unsigned_T;
	const unsigned int rng_bits = mpt::engine_traits<Trng>::result_bits();
	unsigned_T result = 0;
	for (std::size_t entropy = 0; entropy < (sizeof(T) * 8); entropy += rng_bits) {
		if constexpr (rng_bits < (sizeof(T) * 8)) {
			constexpr unsigned int shift_bits = rng_bits % (sizeof(T) * 8); // silence utterly stupid MSVC and GCC warnings about shifting by too big amount (in which case this branch is not even taken however)
			result = (result << shift_bits) ^ static_cast<unsigned_T>(rng());
		} else {
			result = static_cast<unsigned_T>(rng());
		}
	}
	return static_cast<T>(result);
}

template <typename T, std::size_t required_entropy_bits, typename Trng>
inline T random(Trng & rng) {
	static_assert(std::numeric_limits<T>::is_integer);
	typedef typename std::make_unsigned<T>::type unsigned_T;
	const unsigned int rng_bits = mpt::engine_traits<Trng>::result_bits();
	unsigned_T result = 0;
	for (std::size_t entropy = 0; entropy < std::min(required_entropy_bits, sizeof(T) * 8); entropy += rng_bits) {
		if constexpr (rng_bits < (sizeof(T) * 8)) {
			constexpr unsigned int shift_bits = rng_bits % (sizeof(T) * 8); // silence utterly stupid MSVC and GCC warnings about shifting by too big amount (in which case this branch is not even taken however)
			result = (result << shift_bits) ^ static_cast<unsigned_T>(rng());
		} else {
			result = static_cast<unsigned_T>(rng());
		}
	}
	if constexpr (required_entropy_bits >= (sizeof(T) * 8)) {
		return static_cast<T>(result);
	} else {
		return static_cast<T>(result & ((static_cast<unsigned_T>(1) << required_entropy_bits) - static_cast<unsigned_T>(1)));
	}
}

template <typename T, typename Trng>
inline T random(Trng & rng, std::size_t required_entropy_bits) {
	static_assert(std::numeric_limits<T>::is_integer);
	typedef typename std::make_unsigned<T>::type unsigned_T;
	const unsigned int rng_bits = mpt::engine_traits<Trng>::result_bits();
	unsigned_T result = 0;
	for (std::size_t entropy = 0; entropy < std::min(required_entropy_bits, sizeof(T) * 8); entropy += rng_bits) {
		if constexpr (rng_bits < (sizeof(T) * 8)) {
			constexpr unsigned int shift_bits = rng_bits % (sizeof(T) * 8); // silence utterly stupid MSVC and GCC warnings about shifting by too big amount (in which case this branch is not even taken however)
			result = (result << shift_bits) ^ static_cast<unsigned_T>(rng());
		} else {
			result = static_cast<unsigned_T>(rng());
		}
	}
	if (required_entropy_bits >= (sizeof(T) * 8)) {
		return static_cast<T>(result);
	} else {
		return static_cast<T>(result & ((static_cast<unsigned_T>(1) << required_entropy_bits) - static_cast<unsigned_T>(1)));
	}
}

template <typename T>
struct uniform_real_distribution {
private:
	T a;
	T b;

public:
	inline uniform_real_distribution(T a_, T b_)
		: a(a_)
		, b(b_) {
		return;
	}
	template <typename Trng>
	inline T operator()(Trng & rng) const {
		const int mantissa_bits = std::numeric_limits<T>::digits;
		return ((b - a) * static_cast<T>(mpt::random<uint64, mantissa_bits>(rng)) / static_cast<T>((static_cast<uint64>(1u) << mantissa_bits))) + a;
	}
};


template <typename T, typename Trng>
inline T random(Trng & rng, T min, T max) {
	static_assert(!std::numeric_limits<T>::is_integer);
	typedef mpt::uniform_real_distribution<T> dis_type;
	dis_type dis(min, max);
	return static_cast<T>(dis(rng));
}



} // namespace MPT_INLINE_NS
} // namespace mpt



#endif // MPT_RANDOM_RANDOM_HPP
