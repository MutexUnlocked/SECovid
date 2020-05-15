#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>
#include <boost/asio.hpp>

namespace net = boost::asio;            // from <boost/asio.hpp>

using boost::asio::ip::tcp;

auto main(int argc, char *argv[]) -> int
{
    try
    {
        if (argc != 3)
        {
            std::cerr << "Usage: user <host> <port>\n";
            return 1;
        }

        boost::asio::io_context io_context;
        std::string msg;
        std::cout << "ENTER PHONE NUMBER: " << std::endl;
        std::getline(std::cin, msg);
        msg.insert(0, "N");

        tcp::socket s(io_context);
        tcp::resolver resolver(io_context);
        boost::asio::connect(s, resolver.resolve(argv[1], argv[2]));

        boost::asio::write(s, boost::asio::buffer(msg, msg.size()));
        if (recv)
        {
            std::vector<char> buf(1024);
            size_t len = s.read_some(boost::asio::buffer(buf));
            std::string data(buf.begin(), buf.end());
            data.resize(len);

            std::cout << data << std::endl;
        }

        std::cout << "ENTER CODE: " << std::endl;
        std::getline(std::cin, msg);
        msg.insert(0, "C");
        boost::asio::write(s, boost::asio::buffer(msg, msg.size()));
        if (recv)
        {
            std::vector<char> buf(1024);
            size_t len = s.read_some(boost::asio::buffer(buf));
            std::string data(buf.begin(), buf.end());
            data.resize(len);

            std::cout << data << std::endl;
        }
    }
    catch (std::exception &e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }
    return 0;
}