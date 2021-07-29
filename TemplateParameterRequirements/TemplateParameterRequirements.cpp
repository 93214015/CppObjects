// TemplateParameterRequirements.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <concepts>





/*
* SFINAE
* 
* use SFINAE (Subtitution is not a failure) technique to check the template parameter types
*/

//this struct will accept just flaoting point types, otherwise the template specialization would fail.
template<class T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
struct TemplatedStructSFINAE
{
    T m_MemberVariable;
};

//or detrmine to include some functions for specific types
template<typename T>
struct NumberStruct
{
    T _NumberValue;

    T Add(T _T0)
    {
        return _NumberValue + _T0;
    }

    template<typename = std::enable_if_t<std::is_floating_point_v<T>>>
    T FractionPart()
    {
        float _iPart;
        return modf(_NumberValue, &_iPart);
    }
};

//For free functions we could also employ SFINAE in this way
template<typename T>
std::enable_if_t<std::is_integral_v<T>, T>
TemplatedFunctionSFINAE(T _arg0, T _arg1)
{
    return _arg0 + _arg1;
}






/*
* Tag Dispatch
* 
* this technique is used to call different version of a function
*/

template<class T>
T FractionPart_Impl(T _Val, std::false_type)
{
    return 0;
}

template<class T>
T FractionPart_Impl(T _Val, std::true_type)
{
    T _iPart;
    return modf(_Val, &_iPart);
}


template<class T>
struct NumberStructTag
{
    T _Number;

    T FractionPart()
    {
        return FractionPart_Impl(_Number, std::is_floating_point<T>{});
    }

};




/*
* constexpr if
*/

template<class T>
struct NumberStructConstexpr
{
    T _Number;

    T FractionPart()
    {
        if constexpr (std::is_floating_point_v<T>)
        {
            T _iPart;
            return modf(_Number, &_iPart);
        }
        else
        {
            return 0;
        }
    }

};



/*
* C++ 20 Concepts
*/

template<typename T>
concept ConceptIsArithmetic = std::is_arithmetic_v<T>;

template<ConceptIsArithmetic T>
struct NumberStructConcept
{
    T _Number;

    T FractionPart() requires std::is_floating_point_v<T>
    {
        T _iPart;
        return modf(_Number, &_iPart);
    }
};




int main()
{
    TemplatedStructSFINAE<float> _Struct;// passed
    //TemplatedStructSFINAE<int> _Struct;// error


    auto _Result = TemplatedFunctionSFINAE(1, 2);// passed
    //auto _Result = TemplatedFunctionSFINAE(1.1, 2.2); //error


    NumberStruct<int> _Number1{ 5 };
    NumberStruct<float> _Number2{ 5.5 };

    _Number2.FractionPart(); //passes
    //_Number1.FractionPart(); error

    NumberStructTag<double> _NumberTag{1.256};
    NumberStructTag<int> _NumberTag2{ 2 };
    
    NumberStructConstexpr _NumberConstexpr1{ 2.56 }; // used C++ 17 Template parameter deduction
    NumberStructConstexpr _NumberConstexpr2{ 2 };


    NumberStructConcept _NumberConcept1{ 1 };
    NumberStructConcept _NumberConcept2{ 2.23565 };
    //NumberStructConcept<std::string> _NumberConcept3{ "ABC" }; // error - string is not arithmetic

    //_NumberConcept1.FractionPart(); // error  - The int type doesn't have fraction() method
    auto _Fraction = _NumberConcept2.FractionPart();

    std::cout << _NumberConstexpr1.FractionPart() << "\n";
    std::cout << _NumberConstexpr2.FractionPart() << "\n";


    std::cout << "Hello World!\n";
}