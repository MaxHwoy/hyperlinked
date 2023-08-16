#pragma once

namespace hyper
{
	using action = void(*)();

	template <typename T> using action1 = void(*)(T obj);

	template <typename T1, typename T2> using action2 = void(*)(T1 arg1, T2 arg2);

	template <typename T1, typename T2, typename T3> using action3 = void(*)(T1 arg1, T2 arg2, T3 arg3);

	template <typename T1, typename T2, typename T3, typename T4> using action4 = void(*)(T1 arg1, T2 arg2, T3 arg3, T4 arg4);

	template <typename TResult> using function = TResult(*)();

	template <typename T, typename TResult> using function1 = TResult(*)(T arg);

	template <typename T1, typename T2, typename TResult> using function2 = TResult(*)(T1 arg1, T2 arg2);

	template <typename T1, typename T2, typename T3, typename TResult> using function3 = TResult(*)(T1 arg1, T2 arg2, T3 arg3);

	template <typename T1, typename T2, typename T3, typename T4, typename TResult> using function4 = TResult(*)(T1 arg1, T2 arg2, T3 arg3, T4 arg4);
}
