#pragma once

#include <rpc/server.h>
#include <vector>

class Coordinator
{
private:
    /* data */
public:
    Coordinator(/* args */);
    ~Coordinator();
};


void send_data(){

}

// An rpc call the recieves a users location
void sendlocation(std::vector<unsigned char> msg){
    for(auto x : rmsgs){
        msgs.push_back(x);
    }
}