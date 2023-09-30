#pragma once
#include <algorithm>

namespace seal::ranges {
	// This is required because emscripten doesn't have these functions

	template<typename Collection, typename Predicate>
	void sort(Collection& collection, const Predicate& predicate)
	{
		std::sort(std::begin(collection), std::end(collection), predicate);
	}

	template<typename Collection, typename Predicate>
	void transform(Collection& collection, const Predicate& predicate)
	{
		std::transform(std::begin(collection), std::end(collection), predicate);
	}

	template<typename Collection, typename Predicate>
	void generate(Collection& collection, const Predicate& predicate)
	{
		std::generate(std::begin(collection), std::end(collection), predicate);
	}

}
