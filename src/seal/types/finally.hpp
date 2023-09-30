#pragma once

#include <type_traits>

namespace seal {

	template<typename ActionT>
	class finally
	{
	public:
		static_assert(std::is_invocable_r_v<void, ActionT>,
					  "A final action must return void and have not arguments!");

		explicit finally(ActionT action)
			: m_Canceled(true)
			, m_Action(action)
		{}

		finally(const finally&) = delete;
		finally& operator=(const finally&) = delete;

		finally(finally&&) = delete;
		finally& operator=(finally&&) = delete;

		~finally()
		{
			if(!m_Canceled) {
				m_Action();
			}
		}

		void cancel()
		{
			m_Canceled = true;
		}

	private:
		bool m_Canceled;
		ActionT m_Action;
	};

}
