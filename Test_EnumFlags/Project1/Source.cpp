#include <iostream>
#include <stdio.h>
#include <conio.h>

struct PsoOptions // to configure pso creation
{
    enum Value
    {
        None = 0,
        UseDepthTest = 1 << 0,
        UseBlend = 1 << 1,
        UseScissor = 1 << 2
    };
};


int main()
{
    uint32_t PsoOptionsCheck = PsoOptions::UseScissor;

    if ((PsoOptions::UseBlend & PsoOptionsCheck) && (PsoOptions::UseDepthTest & PsoOptionsCheck))
    {
        std::cout << "Using Blend and Depth" << std::endl;
    }
    else
    {
        std::cout << "Not Using Blend and Depth" << std::endl;
    }

    PsoOptionsCheck & PsoOptions::UseScissor ? std::cout << "Using Scissor" << std::endl: std::cout << "Not Using Scissor" << std::endl;
    /*if (PsoOptionsCheck & PsoOptions::UseScissor)
    {
        std::cout << "Using Scissor" << std::endl;
    }
    else
    {
        std::cout << "Not Using Scissor" << std::endl;
    }*/

    _getch();
    return 1;
}
