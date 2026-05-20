// #include "qinmo/base/Timestamp.h"
// #include "qinmo/net/detail/Wrapper.h"
// #include "qinmo/base/StringView.h"
// #include "qinmo/net/SocketTCP.h"
// #include "qinmo/base/StringConcat.h"
// #include "qinmo/base/Logger.h"
// #include "qinmo/net/TcpConnect.h"
// #include "qinmo/net/TcpListen.h"
// #include "qinmo/net/PacketBuffer.h"
#include <qinmo/net/ReactorTcpServer.h>
#include <iostream>

// using qinmo::Timestamp;

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
    // qinmo::net::SocketTCP sock = qinmo::net::SocketTCP::createNonBlockOrDie(qinmo::net::InetAddr());
    //
    // Logger
    // QINMO_TRACE("This is a trial ", 10, " and ", 90);
    // QINMO_DEBUG("This is a trial ", 10, " and ", 0);
    // QINMO_INFO("This is a trial ", 10, " and ", 9);
    // QINMO_WARN("This is a trial ", 1, " and ", 90);
    // QINMO_ERROR("This is a trial ", 6, " and ", 0);
    // QINMO_FATAL("This is a trial ", 2, " and ", 0);

    // using namespace qinmo::net;
    // InetAddr server;
    // server.setIP("192.168.87.212");
    // server.setPort(7129);
    //
    // #define TEMP_EOE(isTrue, describe) if (!isTrue) { std::cout << describe << std::endl; return -1; }
    //
    // TEMP_EOE(server.isValid(), "server address error.");
    // TcpConnect connect = TcpConnect::connectRaw(server);
    // TEMP_EOE(connect.isValid(), "connect invalid.");
    // char buf[1024] = "";
    // TEMP_EOE(-1 != connect.recv(buf, sizeof(buf)),"connect.recv");
    // std::cout << buf << std::endl;
    // connect.send("123456789", 9);

    // using namespace qinmo::net;
    // InetAddr serAddr;
    // serAddr.setIP("192.168.87.90");
    // serAddr.setPort(7129);
    // TcpListen lsock = TcpListen::createRaw(serAddr);
    //
    // #define TEMP_EOE(isTrue, describe) if (!isTrue) { perror(describe); return -1; }
    //
    // TEMP_EOE(lsock.isValid(), "isValid");
    // TEMP_EOE(lsock.setReuseAddr(true), "reuse address");
    // TEMP_EOE(lsock.setReusePort(true), "reuse port");
    // TEMP_EOE(lsock.bind(serAddr), "bind");
    // TEMP_EOE(lsock.listen(), "listen");
    // InetAddr cliAddr;
    // TcpConnect cli = lsock.accept(cliAddr);
    // TEMP_EOE(0 >= cli.send("ok\n", 3), "send");
    // char buf[1024] = "";
    // TEMP_EOE(0 >= cli.recv(buf, sizeof(buf)), "recv");
    // buf[1023] = '\0';
    // std::cout << buf << std::endl;
    // InetAddr local = lsock.getLocalAddr();
    // std::cout << local.getIP() << std::endl;
    // std::cout << cli.getPeerAddr().getIP() << std::endl;

    // using namespace qinmo::net;
    // #define TEMP_EOE(isTrue, describe) if (!isTrue) { std::cout << describe << std::endl; return -1; }
    //
    // InetAddr serAddr;
    // serAddr.setIP("192.168.87.212");
    // serAddr.setPort(7129);
    // TEMP_EOE(serAddr.isValid(), "server address error.");
    // TcpConnect connect = TcpConnect::connectRaw(serAddr);
    // TEMP_EOE(connect.isValid(), "connect invalid.");
    //
    // PacketBuffer bufOut(1);
    // std::string s = "Hello, it's a connect to test PacketBuffer class. Reply something, please.\n";
    // bufOut.appendString(s);
    // TEMP_EOE(bufOut.headSet8(0, s.size()), "protocol header error");
    // ssize_t len = 0;
    // int savederror = 0;
    // len = bufOut.writeFd(connect.getfd(), savederror);
    // int8_t headerNumbet = 0;
    // std::cout << "len:" << len
    //         << ", header:" << bufOut.headGet8(0, headerNumbet)
    //         << ", size:" << (int)headerNumbet
    //         << ", error code:" << savederror
    //         << std::endl;
    //
    // // PacketBuffer bufIn(0);
    // // len = bufIn.readFd(connect.getfd(), savederror, 12);
    // // bufIn.retrieveAll(s);
    // // std::cout << s << std::endl;
    //
    // PacketBuffer bufIn(1);
    // len = bufIn.readFd(connect.getfd(), savederror, 12);
    // TEMP_EOE(bufIn.retrieve8(headerNumbet), "retrieve8");
    // TEMP_EOE(bufIn.headSet8(0, headerNumbet), "headSet8");
    // TEMP_EOE(bufIn.retrieveAll(s), "retrieveAll");
    // std::cout << "len:" << len << ", size:" << headerNumbet << ", text:" << s << std::endl;

    using namespace qinmo::net;
    using namespace qinmo;

    EventLoop loop;

    InetAddr addr;
    addr.setIP("192.168.233.90");
    addr.setPort(7129);
    ReactorTcpServer server(&loop, addr, 2);

    server.setConnectFunc(
        [](const RTcpConnPtr& conn) -> void
        {
            std::cout << conn->getfd() << std::endl;
            QINMO_DEBUG("main.connect func : fd=", conn->getfd());
        }
    );
    server.setMessageFunc(
        [](const RTcpConnPtr& conn, PacketBuffer& buf, Timestamp time) -> void
        {
            std::string s;
            buf.retrieveString(1, s);
            QINMO_DEBUG(s);
            conn->shutdown();
        }
    );
    server.start();
    loop.loop();

    return 0;
}

