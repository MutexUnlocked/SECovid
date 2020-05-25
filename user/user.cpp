#include "user.hpp"

User::User(){
    std::cout << "Initialising context object..." << std::endl;
    helib::Context context(this->m, this->p, this->r, this->gens, this->ords);

    // Modify the context, adding primes to the modulus chain.
    std::cout << "Building modulus chain..." << std::endl;
    buildModChain(context, bits, c);

    // Make bootstrappable.
    context.makeBootstrappable(
      helib::convert<NTL::Vec<long>, std::vector<long>>(mvec));

    // Print the context.
    context.zMStar.printout();
    std::cout << std::endl;

    // Print the security level.
    std::cout << "Security: " << context.securityLevel() << std::endl;
    helib::SecKey secret_key(context);

    // Generate the secret key.
    secret_key.GenSecKey();

    // Generate bootstrapping data.
    secret_key.genRecryptData();

    // Public key management.
    // Set the secret key (upcast: SecKey is a subclass of PubKey).
    this->public_key = secret_key;

    // Get the EncryptedArray of the context.
    this->ea = *(context.ea);

    // Build the unpack slot encoding.
    std::vector<helib::zzX> unpackSlotEncoding;
    buildUnpackSlotEncoding(unpackSlotEncoding, ea);

    // Get the number of slot (phi(m)).
    long nslots = ea.size();
    std::cout << "Number of slots: " << nslots << std::endl;
}
void User::GetKeysFromPKG(std::string host, std::string port){
    auto key_str = get_key_str(host, port);
    auto pri_key = get_key(key_str);
    this->keys = std::move(pri_key);
}

ciphertext User::EncryptMessage(char* id, char* msg){
    auto rez = encrypt(&this->keys.second, id, msg);
    return rez;
}

id_pri_key User::GetPrivateKey(){
    return this->keys.first;
}

master_pub_k User::GetMasterKey(){
    return this->keys.second;
}

void User::DecryptMessage(ciphertext c){
    decrypt(this->keys.first, c);
}

void User::CreateEncHomoLocation(long double longitude, long double latitude){
    long bitSize = 16;
    long outSize = 2 * bitSize;
    long a_data = NTL::RandomBits_long(bitSize);
    long b_data = NTL::RandomBits_long(bitSize);
    long c_data = NTL::RandomBits_long(bitSize);

    std::cout << "Pre-encryption data:" << std::endl;
    std::cout << "a = " << a_data << std::endl;
    std::cout << "b = " << b_data << std::endl;
    std::cout << "c = " << c_data << std::endl;

    // Use a scratch ciphertext to populate vectors.
    helib::Ctxt scratch(public_key);
    std::vector<helib::Ctxt> encrypted_a(bitSize, scratch);
    std::vector<helib::Ctxt> encrypted_b(bitSize, scratch);
    std::vector<helib::Ctxt> encrypted_c(bitSize, scratch);
    // Encrypt the data in binary representation.
    for (long i = 0; i < bitSize; ++i)
    {
        std::vector<long> a_vec(this->ea.size());
        std::vector<long> b_vec(this->ea.size());
        std::vector<long> c_vec(this->ea.size());
        // Extract the i'th bit of a,b,c.
        for (auto &slot : a_vec)
            slot = (a_data >> i) & 1;
        for (auto &slot : b_vec)
            slot = (b_data >> i) & 1;
        for (auto &slot : c_vec)
            slot = (c_data >> i) & 1;
        ea.encrypt(encrypted_a[i], public_key, a_vec);
        ea.encrypt(encrypted_b[i], public_key, b_vec);
        ea.encrypt(encrypted_c[i], public_key, c_vec);
    }
}
