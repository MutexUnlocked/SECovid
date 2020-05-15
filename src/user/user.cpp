#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>
#include "user.hpp"
#include <boost/asio.hpp>
#include <secovid/pkg.hpp>

G1 generator;

PKG pkg;

namespace net = boost::asio;            // from <boost/asio.hpp>

using boost::asio::ip::tcp;


auto main(int argc, char *argv[]) -> int{
    if (argc != 3){
        std::cerr << "Usage: user <host> <port>\n";
    }

    auto key_str = get_key_str(argv[1], argv[2]);
    auto pri_key = get_key(key_str);

    // std::cout <<  pri_key.first.d.serializeToHexStr() << "," << 
    // pri_key.first.q.serializeToHexStr() << "," << 
    // pri_key.second.g1.serializeToHexStr() << std::endl;

    auto fred = encrypt(&pri_key.second, "7639239302", "I like to live in china");
    decrypt(pri_key.first, fred);
    decrypt(pri_key.first, fred);
    decrypt(pri_key.first, fred);
    decrypt(pri_key.first, fred);
    return 0;
}