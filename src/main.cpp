#include "qinmo/base/Timestamp.h"
#include "qinmo/net/detail/Wrapper.h"
#include "qinmo/base/StringView.h"
#include "qinmo/net/SocketTCP.h"
#include "qinmo/base/StringConcat.h"
#include "qinmo/base/Logger.h"

using qinmo::Timestamp;

int main()
{
    // // Timestamp
    // std::cout << Timestamp::now().toString()
    //           << "\n"
    //           << Timestamp::now().toStringMicroseconds()
    //           << "\n"
    //           << Timestamp::now().toStringMicroseconds()
    //           << "\n";
    // Timestamp a = Timestamp::now();
    // Timestamp b = Timestamp::now();
    // std::cout << a.getMicroseconds() << ' ' << b.getMicroseconds() << '\n';
    // if (b > a || b == a)
    //     std::cout << "operator overload is successful." << '\n';
    //
    // // sockaddr_cast
    // // qinmo::net::detail::sockaddr_cast<int>(1);
    // qinmo::net::detail::sockaddr_cast<sockaddr_in, sockaddr>(nullptr);
    //
    // // StringView
    // strlen("000");
    // // strlen(nullptr);
    // qinmo::StringView s("0000");
    // std::cout << s[3] << std::endl;
    //
    // // String concat
    // int test = 0;
    // std::cout << qinmo::concat("asd", 9, '0', "asd") << "\n";
    // qinmo::print(test, "hello", "world", 10, '8', 7.9, '\n');
    //
    // // SockTCP
    // qinmo::net::InetAddr addr;
    // addr.setIP("xxx.xxx.xxx.xxx");
    // addr.setPort(0);
    // qinmo::net::SocketTCP sockfd = qinmo::net::SocketTCP::create(addr);
    // sockfd.bind(addr);
    //
    // // SockTCP
    // qinmo::net::SocketTCP sock = qinmo::net::SocketTCP::create(qinmo::net::InetAddr());
    //
    // Logger
    QINMO_TRACE("This is a trial ", 10, " and ", 90);
    QINMO_DEBUG("This is a trial ", 10, " and ", 0);
    QINMO_INFO("This is a trial ", 10, " and ", 9);
    QINMO_WARN("This is a trial ", 1, " and ", 90);
    QINMO_ERROR("This is a trial ", 6, " and ", 0);
    QINMO_FATAL("This is a trial ", 2, " and ", 0);

    return 0;
}

