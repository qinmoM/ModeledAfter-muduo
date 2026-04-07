#include "qinmo/base/Timestamp.h"
#include <iostream>

using qinmo::Timestamp;

int main()
{
    std::cout << Timestamp::now().toString()
              << "\n"
              << Timestamp::now().toStringMicroseconds()
              << "\n"
              << Timestamp::now().toStringMicroseconds()
              << "\n";

    Timestamp a = Timestamp::now();
    Timestamp b = Timestamp::now();
    std::cout << a.getMicroseconds() << ' ' << b.getMicroseconds() << '\n';
    if (b > a || b == a)
        std::cout << "operator overload is successful." << '\n';

    return 0;
}

