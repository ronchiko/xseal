#pragma once

#include <memory>
#include <vector>

#include "seal/types/traits/template.hpp"

namespace seal {

	namespace detail {

		struct base_early_destroyable
		{
			virtual ~base_early_destroyable() = default;

			virtual void destroy() = 0;
		};

		template<typename T>
		struct early_destroyable : public base_early_destroyable
		{
			early_destroyable(T *value)
				: m_Object(value)
			{}

			virtual void destroy() override
			{
				if constexpr (is_template_instance_v<T, std::vector>) {
					m_Object->clear();
				} else {
					*m_Object = {};		
				}
			}

			T *m_Object;
		};
	}

	/**
	   Utitliy object to destroy static objects that depend on other static object from
	   being destroyed.
	 */
	class early_destroy
	{
	public:
		template<typename T>
		static void queue(T& value)
		{
			auto d = std::make_unique<detail::early_destroyable<T>>(&value);
			g_Queued.push_back(std::move(d));
		}

		static void destroy_all() {
			for (const auto& queued : g_Queued) {
				queued->destroy();
			}

			g_Queued.clear();
		}

	private:
		inline static std::vector<std::unique_ptr<detail::base_early_destroyable>> g_Queued;
	};
}
