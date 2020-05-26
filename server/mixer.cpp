#include "mixer.hpp"


Mixer::Mixer(){
    init();
    srv = new rpc::server(8080);
    srv->bind("getmessages", &getmessages);
    crypto_box_keypair(this->pk, this->sk);
}

void Mixer::run(){
    srv->async_run();
}

// Send this mixers messages to another mixer
void Mixer::get_messages_from_server(std::string ip){
    client = new rpc::client(ip,8080);
    client->call("sendmessages", msgs);
}

//TODO: NAME CONFLICT FIX LATER
std::vector<unsigned char> Mixer::Encrypt(std::string msg, unsigned char* recipient_pk){
    int CIPHERTEXT_LEN = msg.length() + crypto_box_MACBYTES;
    unsigned char ciphertext[CIPHERTEXT_LEN];
    unsigned char message[msg.length()];

    strncpy((char*)(message), &msg[0], sizeof(message));

    crypto_box_seal(ciphertext, message, msg.length(), recipient_pk);
    auto n = sizeof(ciphertext)/sizeof(ciphertext[0]);
    std::vector<unsigned char> rez(ciphertext, ciphertext + n);
    return rez;
}

std::vector<unsigned char> Mixer::Decrypt(unsigned char* ciphertext){
    unsigned char decrypted[10000];
    auto len = sizeof(ciphertext)/sizeof(ciphertext[0]);
    crypto_box_seal_open(decrypted, ciphertext, len,this->pk, this->sk);

    auto n = sizeof(decrypted)/sizeof(decrypted[0]);
    std::vector<unsigned char> rez(decrypted, decrypted + n);
    return rez;
}

void Mixer::shuffle(bool multi_threaded){
    parallel = multi_threaded;
	const int SECRET_SIZE = 100;

	srand ((unsigned int) time (NULL));
	
	long m = num[1];
	long n = msgs.size();
	cout << "shuffling " << n * m << " messages" <<endl;
	unsigned char** secrets = new unsigned char* [m * n];
	
	for (int i = 0; i < n; i++) {
        for(int j = 0; j < msgs[i].size(); j++){
            secrets[i][j] = msgs[i][j];
        }
	}
	
	time_t begin = time(NULL);
    cout << "GOING TO ENCRYPT" << endl;
	CipherTable* ciphers = (CipherTable*) encrypt((void**) secrets, SECRET_SIZE, m * n, 1);
	time_t enc_time = time(NULL);
	cout << "ecryption time: " << enc_time - begin << endl; 

	time_t parse_start = time(NULL);
	cout << "parsing input" <<endl;
	string shuffle_input(ciphers->encode_all_ciphers());
	cout << "done parsing. " << time(NULL) - parse_start << endl;

	char* shuffled_ciphers;
	int shuffled_ciphers_len;
	char* proof;
	int proof_len;
	int* permutation;
	int permutation_len;
	char* public_randoms;
	int public_randoms_len;
	
	cout << "shuffle begins!" <<endl;
	ElGammal* elgammal = (ElGammal*)create_pub_key(1);
	char* input = (char*)shuffle_input.c_str();
	time_t shuffle_time = time(NULL);
	void *cached_shuffle = shuffle_internal(elgammal, input, shuffle_input.size(), m*n, &shuffled_ciphers, &shuffled_ciphers_len, &permutation, &permutation_len);
	cout << "shuffle is done! In " << time(NULL) - shuffle_time << endl;
        time_t prove_time = time(NULL);
	prove(cached_shuffle, &proof, &proof_len, &public_randoms, &public_randoms_len);
	cout << "proof is done! In " << time(NULL) - prove_time << endl;

	time_t verify_time = time(NULL);
	int ret = verify(1, proof, proof_len, input, shuffle_input.size(), shuffled_ciphers, shuffled_ciphers_len, public_randoms, public_randoms_len);
	cout << "verification is done! In " << time(NULL) - verify_time << endl;
	cout << "Shuffle + prove + verify = " << time(NULL) - shuffle_time << endl;
	delete ciphers;

	for (int i = 0; i < m * n; i++) {
		delete [] secrets[i];
	}
	delete [] secrets;


	delete [] shuffled_ciphers;
	delete [] proof;
	delete [] permutation;

	if (ret) {
		cout << "everything passed!" <<endl;
	} else {
		cout << "shuffle failed!" <<endl;
	}
}