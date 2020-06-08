#include <iostream>
#include <vector>
#include "user.hpp"
#include <boost/asio.hpp>
#include <secovid/pkg.hpp>
#include "contact.hpp"
G1 generator;
PKG pkg;
std::string genstr("7520de2c509753d002cccfb5b5da913b90414dba8457665f044c695f5f6e171e113e4bb3917e63cbb132989a8a5e8103");


namespace net = boost::asio;            // from <boost/asio.hpp>

using boost::asio::ip::tcp;


auto main(int argc, char *argv[]) -> int{
    if (argc != 3){
        std::cerr << "Usage: user <host> <port>\n";
    }

    /*auto key_str = get_key_str(argv[1], argv[2]);
    auto pri_key = get_key(key_str);

    // std::cout <<  pri_key.first.d.serializeToHexStr() << "," << 
    // pri_key.first.q.serializeToHexStr() << "," << 
    // pri_key.second.g1.serializeToHexStr() << std::endl;
    std::cout << generator.serializeToHexStr() << std::endl;

    auto fred = encrypt(&pri_key.second, "7639239302", "I like to live in china");
    decrypt(pri_key.first, fred);
    decrypt(pri_key.first, fred);
    decrypt(pri_key.first, fred);
    decrypt(pri_key.first, fred);*/

   // std::cout << "MADE IT HERE" << std::endl;
    //contact_data ata;
    Contact x("ALICE");
    //x.AddContact("AAAB", ata);
	//std::cout << "FIRST GRAPH: ";
	//x.PrintGraph();

    //std::cout << "MADE IT HERE 2" << std::endl;
    Contact b("Bob");
    //b.AddContact("BBBA", ata);
	//std::cout << "SECOND GRAPH: ";
	//b.PrintGraph();

    auto rez = b.Serialize();
    auto data = b.Deserialize(rez);
    //std::cout << "MADE IT HERE 3" << std::endl;


	
    x.AddContact(data.uid, data);
    //std::cout << "FIRST GRAPH: " << std::endl;
    x.PrintGraph();


	auto rez1 = x.Serialize();
	auto data1 = x.Deserialize(rez1);

	Contact c("Ben");
	c.AddContact(data1.uid ,data1);
	c.PrintGraph();

	auto rez2 = c.Serialize();
	auto data2 = c.Deserialize(rez2);

	Contact w("Mayer");
	w.AddContact(data2.uid, data2);
	w.PrintGraph();

	w.AddContact(data.uid, data);
	w.PrintGraph();
	w.PrintAllUIDS();
    return 0;
}
