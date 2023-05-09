#pragma once

#include <memory>

namespace seal::ecs::manager::detail {
	/**
	   Class representing a system without knowing its internal type.
	 */
	class system_interface
	{
	public:
		/**
		   Creates a new system interface with a set of parameters.
		 */
		template<typename SystemT, typename... ConstructionArgs>
		static system_interface create(ConstructionArgs&&...args)
		{
			SystemT instance(args...);
			return system_interface{
				std::make_unique<concrete_system<SystemT>>(std::move(instance))
			};
		}

		void *instance()
		{
			return m_System->instance();
		}

	private:
		class abstract_system
		{
		public:
			virtual ~abstract_system() = default;

			/**
			   Gets the instance of the system.
			 */
			virtual void *instance() = 0;
		};

		template<typename T>
		class concrete_system : public abstract_system
		{
		public:
			concrete_system(T&& instance)
				: m_Instance(std::move(instance))
			{}

			virtual void *instance() override
			{
				return &m_Instance;
			}

		private:
			T m_Instance;
		};

		explicit system_interface(std::unique_ptr<abstract_system> system)
			: m_System(std::move(system))
		{}

		std::unique_ptr<abstract_system> m_System;
	};
}
