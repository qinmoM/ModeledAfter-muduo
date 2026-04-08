#include "qinmo/base/Timestamp.h"
#include "qinmo/net/detail/Wrapper.h"
#include <iostream>

using qinmo::Timestamp;

int main()
{
    // Timestamp
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

    // sockaddr_cast
    // qinmo::net::detail::sockaddr_cast<int>(1);
    qinmo::net::detail::sockaddr_cast<sockaddr_in, sockaddr>(nullptr);

    return 0;
}

