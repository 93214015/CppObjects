// TemplateParameterRequirements.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>



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


    std::cout << "Hello World!\n";
}