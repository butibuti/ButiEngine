#include"stdafx.h"
#include"Test.h"
int ButiEngine:: TestFunc() {

    std::cout << ("Hello World") << std::endl;

    return 0;
}

ButiEngine::TestClass::TestClass(int argValue)
{
    value = argValue;
}

void ButiEngine::TestClass::DoSomeThing()
{
    std::cout <<value << std::endl;
}
