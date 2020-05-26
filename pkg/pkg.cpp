#include "pkg.hpp"

using namespace std;
std::string genstr("7520de2c509753d002cccfb5b5da913b90414dba8457665f044c695f5f6e171e113e4bb3917e63cbb132989a8a5e8103");


PKG::PKG(){
    initPairing(mcl::BLS12_381);
    generator.deserializeHexStr(genstr);
    G1 pub;
    Fr s; s.setByCSPRNG();
    
    G1::mul(pub,generator, s);

    m_pub_k key{pub};
    m_pri_k sk{s};

    this->m_pub = key;
    this->m_pri = sk;

}

id_pri_key PKG::extract(char* id){
    G2 q, d;
    hashAndMapToG2(q, id, strlen(id));
    G2::mul(d, q, this->m_pri.s);
    id_pri_key k {d,q};
    return k;
}