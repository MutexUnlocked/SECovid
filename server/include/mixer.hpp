#pragma once

#include <groth/Utils.h>

#include <groth/CipherTable.h>
#include <groth/Globals.h>
#include <groth/Functions.h>
#include <string.h>
#include <groth/RemoteShuffler.h>
#include <groth/FakeZZ.h>
#include <groth/SchnorrProof.h>

#include <rpc/server.h>
#include <rpc/client.h>
#include <cmath>
#include <stdlib.h>
#include <time.h>
#include <map>
#include <sodium.h>
#include <sodium/crypto_secretbox.h>
#include <stdio.h>
#include <mutex>

vector<long> num(8);
std::vector<std::vector<unsigned char>> msgs;


class Mixer
{
private:
    unsigned char pk[crypto_box_PUBLICKEYBYTES];
    unsigned char sk[crypto_box_SECRETKEYBYTES];
    rpc::server* srv;
    rpc::client* client;
public:
    Mixer();
    std::vector<unsigned char> Encrypt(std::string msg, unsigned char* recipient_pk);
    std::vector<unsigned char> Decrypt(unsigned char* ciphertext);
    void shuffle(bool multi_threaded);
    void get_messages_from_server(std::string ip);
    void run();
};


// An rpc call that returns  messages
std::vector<std::vector<unsigned char>> getmessages(){
    return msgs;
}

// An rpc call the recieves messages
void sendmessages(std::vector<std::vector<unsigned char>> rmsgs){
    for(auto x : rmsgs){
        msgs.push_back(x);
    }
}

// An rpc call the recieves messages
void sendmessage(std::vector<unsigned char> msg){
    for(auto x : msgs){
        msgs.push_back(x);
    }
}