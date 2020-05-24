package mixer

import (
	crypto_rand "crypto/rand"
	"fmt"
	"io"
	"time"

	"golang.org/x/crypto/nacl/box"
)

type server struct {
	sk       *[32]byte
	pk       *[32]byte
	messages *[32][32]byte
}

func (m *server) init() {
	publicKey, privateKey, err := box.GenerateKey(crypto_rand.Reader)
	if err != nil {
		panic(err)
	}
	m.sk = privateKey
	m.pk = publicKey
	//fmt.Println(*m.sk)
	//fmt.Println(*m.pk)
}

func (m *server) encrypt(msg []byte, recvPublicKey [32]byte) []byte {
	// The first 24 bytes of the ciphertext will contain the nonce
	var nonce [24]byte
	if _, err := io.ReadFull(crypto_rand.Reader, nonce[:]); err != nil {
		panic(err)
	}
	enc := box.Seal(nonce[:], msg, &nonce, &recvPublicKey, m.sk)
	return enc
}

func (m *server) decrypt(enc []byte, senderPublicKey [32]byte) []byte {
	var decNonce [24]byte
	copy(decNonce[:], enc[:24])
	dec, ok := box.Open(nil, enc[24:], &decNonce, &senderPublicKey, m.sk)
	if !ok {
		panic("Failed to decrypt message")
	}
	return dec
}

func (m *server) shuffle() {
	start := time.Now()

	var secrets []byte
	secrets = make([]byte, 100000, 100000)
	var i int = 0
	for i < len(secrets) {
		secrets[i] = byte(i)
		i += 1
	}

	g := Groth{}
	ciphers, ciphers_len, groupelts, element_len := g.Encrypt(secrets, 5, 1)
	fmt.Println("Going to shuffle", len(groupelts)/element_len, "elements")
	dec_groupelts, dec_element_len := g.Decrypt(ciphers, ciphers_len, 1)

	if dec_element_len != element_len {
		fmt.Println("Error: lengths mismatch ", dec_element_len, element_len)
	}

	fmt.Println("starting shuffle.")
	shuffleStart := time.Now()
	ciphers_outStr, _, handle := g.Shuffle(ciphers, ciphers_len, 1)
	shuffleTime := time.Since(shuffleStart)
	fmt.Println("Shuffle time: ", shuffleTime)

	proof, pub_randoms := g.Prove(handle)
	proofTime := time.Since(shuffleStart)
	fmt.Println("Proof time: ", proofTime)

	ret := g.Verify(proof, ciphers, ciphers_outStr, 1, pub_randoms)

	var j int = 0
	for j < len(groupelts) {
		if groupelts[j] != dec_groupelts[j] {
			fmt.Println("Error:")
			fmt.Println("---", groupelts[j])
			fmt.Println("---", dec_groupelts[j])
		}
		j += 1
	}
	elapsed := time.Since(start)
	fmt.Println("Test complete:", ret, ". Total time: ", elapsed)
}
