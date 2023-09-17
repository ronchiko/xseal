#pragma once

namespace seal {

	template<typename Type, template<typename...> typename Template>
	inline constexpr auto is_template_instance_v = false;

	template<template<typename...> typename Template, typename... TParams>
	inline constexpr auto is_template_instance_v<Template<TParams...>, Template> = true;

	template<typename Type, template<typename...> typename Template>
	concept template_instance_of = is_template_instance_v<Type, Template>;
}
