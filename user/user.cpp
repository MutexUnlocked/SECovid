#include "user.hpp"

User::User() : context(this->m, this->p, this->r) {
    std::cout << "Initialising context object..." << std::endl;

    // Modify the context, adding primes to the modulus chain.
    std::cout << "Building modulus chain..." << std::endl;
    buildModChain(context, bits, c);

    // Print the context.
    context.zMStar.printout();
    std::cout << std::endl;

    // Print the security level.
    std::cout << "Security: " << context.securityLevel() << std::endl;
    helib::SecKey secret_key(context);

    // Generate the secret key.
    secret_key.GenSecKey();

    // Compute key-switching matrices that we need
    helib::addSome1DMatrices(secret_key);

    // Public key management.
    // Set the secret key (upcast: SecKey is a subclass of PubKey).
    const helib::PubKey& public_key = secret_key;
    hpublic_key = const_cast<helib::PubKey*>(&public_key);
    // Get the EncryptedArray of the context.
    ea = (context.ea);
    // Get the number of slot (phi(m)).
    long nslots = ea->size();
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

std::pair<helib::Ctxt, helib::Ctxt> User::CreateEncHomoLocation(long double longitude, long double latitude){
    helib::Ptxt<helib::BGV> longi(this->context);
    for (int i = 0; i < longi.size(); ++i){
        longi[i] = i;
    }

    helib::Ptxt<helib::BGV> lati(this->context);
    for (int i = 0; i < lati.size(); ++i){
        lati[i] = i;
    }
    longi.addConstant(longitude);
    lati.addConstant(latitude);

    // Create ciphertexts
    helib::Ctxt lat_ctxt(*this->hpublic_key);
    hpublic_key->Encrypt(lat_ctxt, lati);

    helib::Ctxt long_ctxt(*this->hpublic_key);
    hpublic_key->Encrypt(long_ctxt, longi);


    return std::make_pair(lat_ctxt, long_ctxt);
}

void User::ComputeHomoDistance(helib::Ctxt l, helib::Ctxt la, helib::Ctxt l2, helib::Ctxt la2){
    la2 -= la;
    l2 -= l;

    

}