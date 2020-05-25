#pragma once

#include <secovid/pkg.hpp>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <utility>
#include <rpc/client.h>
#include <boost/asio.hpp>
#include <helib/helib.h>
#include <helib/binaryArith.h>
#include <helib/intraSlot.h>


namespace net = boost::asio;            // from <boost/asio.hpp>

using boost::asio::ip::tcp;

class User{
private:
    std::string key_str;
    helib::EncryptedArray &ea;
    helib::PubKey public_key;
    std::pair<id_pri_key, master_pub_k> keys;
    // Hmomorphic Encryption Parameters
    // Plaintext prime modulus.
    long p = 2;
    // Cyclotomic polynomial - defines phi(m).
    long m = 4095;
    // Hensel lifting (default = 1).
    long r = 1;
    // Number of bits of the modulus chain.
    long bits = 500;
    // Number of columns of Key-Switching matrix (typically 2 or 3).
    long c = 2;
    // Factorisation of m required for bootstrapping.
    std::vector<long> mvec = {7, 5, 9, 13};
    // Generating set of Zm* group.
    std::vector<long> gens = {2341, 3277, 911};
    // Orders of the previous generators.
    std::vector<long> ords = {6, 4, 6};
public:
    User();
    void GetKeysFromPKG(std::string host, std::string port);
    ciphertext EncryptMessage(char* id, char* msg);
    void DecryptMessage(ciphertext c);
    void CreateEncHomoLocation(long double longitude, long double latitude);
    id_pri_key GetPrivateKey();
    master_pub_k GetMasterKey();
};

inline auto get_key_str(std::string host, std::string port) -> std::string{
    try
    {
      
        boost::asio::io_context io_context;
        std::string msg;
        std::cout << "ENTER PHONE NUMBER: " << std::endl;
        std::getline(std::cin, msg);
        msg.insert(0, "N");

        tcp::socket s(io_context);
        tcp::resolver resolver(io_context);
        boost::asio::connect(s, resolver.resolve(host, port));

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

            return data;
        }
    }
    catch (std::exception &e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }
    return "";
}

inline auto get_key(std::string key_str) {
    std::vector<std::string> result;
    std::stringstream s_stream(key_str); //create string stream from the string
    while (s_stream.good())
    {
        std::string substr;
        getline(s_stream, substr, ','); //get first string delimited by comma
        result.push_back(substr);

    }

    G2 d, q;
    d.deserializeHexStr(result[0]);
    q.deserializeHexStr(result[1]);
    id_pri_key rez{d,q};

    
    G1 pz;
    pz.deserializeHexStr(result[2]);
    m_pub_k pub{pz};
    
    return std::make_pair(rez,pub);
}
