#include "pkg.hpp"

using namespace std;

PKG::PKG(){
    initPairing(mcl::BLS12_381);
    char* gen = "abc";
    hashAndMapToG1(generator, &gen, strlen(gen));
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