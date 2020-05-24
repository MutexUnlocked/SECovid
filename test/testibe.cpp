#include <secovid/pkg.hpp>

auto main() -> int{
    PKG k;
    // Get Friedrich's private key
    auto fred = k.extract("friedrichdoku@gmail.com");
    // Get Ethan's private key
	auto ethan = k.extract("ethandoku@gmail.com");

    // Encrypt a message for Friedrich
    auto ciphertext = encrypt(&k.m_pub, "friedrichdoku@gmail.com", "I like to live in china");
    std::cout << ciphertext.V << std::endl;
    //auto ciphertext2 = encrypt(k.m_pub, "friedrichdoku@gmail.com", "I like to live in china");

    // Open Friedrich's message
    decrypt(fred, ciphertext);


    // Let Ethan try to open Friedrich's message
     decrypt(ethan, ciphertext);
   // printf("decrypted: %s\n", dec2);
}