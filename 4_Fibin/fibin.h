#ifndef FIBIN_H_
#define FIBIN_H_

#include <cstdint>
#include <iostream>
#include <type_traits>
#include <cassert>
#include <string_view>
#define MAXMULT 60466176

namespace details {

template<int N, typename ValueType>
class Integer {
public:
	static constexpr ValueType value = N;
};

template<typename T>
struct IsInteger {
public:
	static constexpr bool value = false;
};

template<int N, typename ValueType>
struct IsInteger<Integer<N, ValueType>> {
public:
	static constexpr bool value = true;
};

template <typename Lambda, typename Env>
class LambdaWrapper {
public:
    using lambda = Lambda;
    using env = Env;
};

template <typename T>
struct IsLambda {
public:
    static constexpr bool value = false;
};

template <typename T>
struct IsLambdaWrapper {
public:
    static constexpr bool value = false;
};

template <typename Lambda, typename Env>
struct IsLambdaWrapper<LambdaWrapper<Lambda, Env>> {
public:
    static constexpr bool value = true;
};

template<typename Fun, typename ValueType, typename Value>
class Apply {};

template <int K, typename V, typename Tail>
class List {
public:
    static constexpr unsigned int key = K;
    using value = V;
    using tail = Tail;
};

class Undefined {};

class EmptyList {};

template <int K, typename Lista>
class Find {
public:
    using value = typename std::conditional<K == Lista::key, typename Lista::value, typename Find<K, typename Lista::tail>::value>::type;
};

template <int K>
class Find<K, EmptyList> {
public:
    using value = Undefined;
};

// Maps character to integer in range (1,36)
constexpr unsigned int mapChar(const char i) {
    unsigned int result = 0;
    if (i >= 'a' && i <= 'z')
        result =  i - 'a' + 11;
    else if (i >= 'A' && i <= 'Z')
        result = i - 'A' + 11;
    else if (i >= '0' && i <= '9')
        result = i - '0' + 1;

    return result;
}

constexpr unsigned int hashVariableName(const char *varName, unsigned int mult) {
    unsigned int result = 0;
    unsigned int charValue = 0;
    unsigned int nameHashed = 0;
    if(mult > MAXMULT)
    	return 0;
    if(varName == nullptr)
    	return 0;
    if (*varName != 0){
    	charValue = mapChar(*varName) * mult;
    	nameHashed = hashVariableName(varName + 1, mult * 36);
    	if(charValue == 0)
    		return 0;
    	if(nameHashed != 0 || *(varName+1) == 0)
    		result = charValue + nameHashed;
    	else
    		result = 0;
    }

    return result;
}

}

constexpr unsigned int Var (const char *id) {
    return details::hashVariableName(id, 1);
}

template<long long N>
class Fib {};

class True {};
class False {};

template<typename Value>
class Lit {};

template<int Var, typename Body>
class Lambda {};

template<typename Left, typename Right>
class Eq {};

template<typename Condition, typename Then, typename Else>
class If {};

template<typename Arg, typename Arg2, typename... Args>
class Sum {};

template<typename Arg>
class Inc1 {};

template<typename Arg>
class Inc10 {};

template<typename Lambda, typename Param>
class Invoke {};

template <int V>
class Ref {};

template <unsigned int Var, typename Body>
struct details::IsLambda<Lambda<Var, Body>> {
public:
    static constexpr bool value = true;
};

template <int V, typename Val, typename Expr>
class Let {};

class MatchError{};

template <typename Expr, typename Env, typename ValueType, typename type = void>
class Evaluate {
public:
    using value = MatchError;
};

template<int N, typename Env, typename ValueType>
class Evaluate<details::Integer<N, ValueType>, Env, ValueType>{
public:
	using value = details::Integer<N, ValueType>;
};

template <unsigned int Var, typename Body, typename Env, typename ValueType>
class Evaluate<Lambda<Var, Body>, Env, ValueType> {
public:
    using value = details::LambdaWrapper<Lambda<Var, Body>, Env>;
};

template <unsigned int N, typename Env, typename ValueType>
class Evaluate<Ref<N>, Env, ValueType> {
public:
	static_assert(N != 0);
    using value = typename details::Find<N, Env>::value;
};

template <int Variable, typename Value, typename Expr, typename Env, typename ValueType>
class Evaluate<Let<Variable, Value, Expr>, Env, ValueType> {
public:
	static_assert(Variable != 0);
    using NewEnv = details::List<Variable,
        typename Evaluate<Value, Env, ValueType>::value, Env>;
    using value = typename Evaluate<Expr, NewEnv, ValueType>::value;
};

template <int N, typename Env, typename ValueType>
class Evaluate<Lit<Fib<N>>, Env, ValueType, typename std::enable_if<(N > 1)>::type> {
public:
	static_assert(N >= 0);
    using value = typename Evaluate<Sum<Lit<Fib<N-1>>, Lit<Fib<N-2>>>, Env, ValueType>::value;
};

template <typename Env, typename ValueType>
class Evaluate<Lit<Fib<1>>, Env, ValueType> {
public:
    using value = details::Integer<1, ValueType>;
};

template <typename Env, typename ValueType>
class Evaluate<Lit<Fib<0>>, Env, ValueType> {
public:
    using value = details::Integer<0, ValueType>;
};

template<typename Left, typename Right, typename Env, typename ValueType>
class Evaluate<Eq<Left, Right>, Env, ValueType>{
public:
	using v1 = typename Evaluate<Left, Env, ValueType>::value;
	using v2 = typename Evaluate<Right, Env, ValueType>::value;
	static_assert(details::IsInteger<v1>::value);
	static_assert(details::IsInteger<v2>::value);

    using value = typename std::conditional<v1::value == v2::value, True, False>::type;
};

template<typename Then, typename Else, typename Env, typename ValueType>
class Evaluate<If<True, Then, Else >, Env, ValueType>{
public:
    using value = typename std::conditional<details::IsLambda<Then>::value,
        Then, typename Evaluate<Then, Env, ValueType>::value>::type;
};

template<typename Then, typename Else, typename Env, typename ValueType>
class Evaluate<If<False, Then, Else >, Env, ValueType>{
public:
    using value = typename std::conditional<details::IsLambda<Else>::value,
        Else, typename Evaluate<Else, Env, ValueType>::value>::type;
};

template<typename Condition, typename Then, typename Else, typename Env, typename ValueType>
class Evaluate<If<Condition, Then, Else >, Env, ValueType>{
public:
    using value = typename Evaluate<If<typename Evaluate<Condition, Env, ValueType>::value, Then, Else>,
        Env, ValueType>::value;
};

template<typename Arg, typename Arg2, typename Env, typename ValueType>
class Evaluate<Sum<Arg, Arg2>, Env, ValueType>{
public:
    using value = details::Integer<Evaluate<Arg, Env, ValueType>::value::value + Evaluate<Arg2, Env, ValueType>::value::value, ValueType>;
};

template<typename Arg, typename Arg2, typename... Args, typename Env, typename ValueType>
class Evaluate<Sum<Arg, Arg2, Args...>, Env, ValueType>{
public:
    using value = details::Integer<Evaluate<Arg, Env, ValueType>::value::value + Evaluate<Sum<Arg2, Args...>, Env, ValueType>::value::value, ValueType >;
};

template<typename Arg, typename Env, typename ValueType>
class Evaluate<Inc1<Arg>, Env, ValueType>{
public:
    using value = typename Evaluate<Sum<Arg, Lit<Fib<1>>>, Env, ValueType>::value;
};

template<typename Arg, typename Env, typename ValueType>
class Evaluate<Inc10<Arg>, Env, ValueType>{
public:
    using value = typename Evaluate<Sum<Arg, Lit<Fib<10>>>, Env, ValueType>::value;
};

template<int Var, typename Body, typename Env, typename Value, typename ValueType>
class details::Apply<details::LambdaWrapper<Lambda<Var, Body>, Env>, ValueType, Value> {
public:
	static_assert(Var != 0);
    using NewEnv = details::List<Var, Value, Env>;
    using value = typename Evaluate<Body, NewEnv, ValueType>::value;
};

template <typename Lambda, typename Param, typename Env, typename ValueType>
class Evaluate<Invoke<Lambda, Param>, Env, ValueType> {
public:
    using value = typename details::Apply<typename Evaluate<Lambda, Env, ValueType>::value, ValueType, typename Evaluate<Param, Env, ValueType>::value>::value;
};

template <typename Env, typename ValueType>
class Evaluate<Lit<True>, Env, ValueType> {
public:
    using value = True;
};

template <typename Env, typename ValueType>
class Evaluate<Lit<False>, Env, ValueType> {
public:
    using value = False;
};

template <typename ValueType>
class Fibin {
public:
    template <typename Expr, typename U = ValueType>
    static constexpr typename std::enable_if<std::is_integral<U>::value, U>::type eval() {
        return Evaluate<Expr, details::EmptyList, ValueType>::value::value;
    }

    template <typename Expr, typename U = ValueType>
    static constexpr typename std::enable_if<!std::is_integral<U>::value, void>::type eval() {
        std::cout << "Fibin doesn't support: " << typeid(ValueType).name() << std::endl;
    }
};

#endif
