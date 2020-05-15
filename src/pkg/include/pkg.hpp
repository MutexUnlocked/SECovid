/*
 *
 * Copyright (c) 2020  Mutex Unlocked
 * Author: Friedrich Doku
 *-----
 *Last Modified:  Thu May 14 2020 12:51:19 PM
 *-----
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 * This program is distributed in the hope that it will be useful
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <tuple>
#include <vector>
#include <sodium.h>
#include <sodium/crypto_secretbox.h>
#include <mcl/bls12_381.hpp>
#include "utils.hpp"
#include <iostream>

using namespace mcl::bn;

extern G1 generator;

std::string genstr("7520de2c509753d002cccfb5b5da913b90414dba8457665f044c695f5f6e171e113e4bb3917e63cbb132989a8a5e8103");
struct master_pub_k {
    G1 g1;
};

struct master_pri_k {
    Fr s;
};

struct id_pri_key {
    G2 d;
    G2 q;
};

struct ciphertext{
    G1 U;
    unsigned char V[crypto_secretbox_MACBYTES+10000];
    unsigned char nonce[crypto_box_NONCEBYTES];
};

typedef struct ciphertext ciphertext;
typedef struct master_pub_k m_pub_k;
typedef struct master_pri_k m_pri_k;
typedef struct id_pri_key id_pri_key;


class PKG
{
private:
    m_pri_k m_pri;
public:
    PKG();
    m_pub_k m_pub;
    id_pri_key extract(char* id);
};


inline auto encrypt(m_pub_k* key, char* id, char* msg){
    G2 q; GT g, er;
    hashAndMapToG2(q, id, strlen(id));
    pairing(g, key->g1, q);
    
    G1 rp;
    Fr r; r.setByCSPRNG();

    // Curve point generator

    G1::mul(rp, generator, r);
    GT::pow(er, g, r);


    int CIPHERTEXT_LEN = crypto_secretbox_MACBYTES + strlen(msg);
    unsigned char nonce[crypto_secretbox_NONCEBYTES];
    unsigned char ciphertexttmp[CIPHERTEXT_LEN];
    auto sk = serialize(q, rp, er);

    //printf("THIS IS COOL\n");
    char buffer[65];
    //std::cout << sk << std::endl;
    sha256(&sk[0], buffer);
    //printf("END\n");


    randombytes_buf(nonce, sizeof(nonce));
    crypto_secretbox_easy(ciphertexttmp, reinterpret_cast<unsigned char*>(msg), strlen(msg),
     nonce, reinterpret_cast<unsigned char*>(buffer));

    ciphertext ctx;
    ctx.U = rp;
    memcpy(ctx.V, ciphertexttmp, sizeof(ciphertexttmp));
    memcpy(ctx.nonce, nonce, sizeof(nonce));
    return ctx;    
}

inline auto decrypt(id_pri_key k, ciphertext c){
    GT e;
    pairing(e, c.U, k.d);

    auto key = serialize(k.q, c.U, e);
    unsigned char decrypted[100000];

    //printf("THIS IS COOL\n");
    char buffer[65];
    //std::cout << key << std::endl;
    sha256(&key[0], buffer);
    //printf("END\n");

    if (crypto_secretbox_open_easy(decrypted, c.V, strlen((char*) c.V),
     c.nonce, reinterpret_cast<unsigned char*>(buffer)) != 0) {
        printf("FAILED TO DECRYPT MESSAGE\n");
    }else{
        std::cout << (char*)decrypted << std::endl;
    }
    
}
