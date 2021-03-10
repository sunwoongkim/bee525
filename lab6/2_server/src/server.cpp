/* 
	* @author	Sunwoong Sunny Kim
	* @brief	this file is for the B EE 525 class and cannot be distributed without permission of the author
*/

#include "examples.h"
#include <chrono>

using namespace std;
using namespace seal;
using namespace std::chrono;

int main(void) {
  // prepare to write and read files
  ifstream if_parms_server;
  ifstream if_cipher_server;
  ofstream of_cipher_server;
  if_parms_server.open("../../network/parms.bin");
  if_cipher_server.open("../../network/cipherI.bin");
  of_cipher_server.open("cipherR.bin");
  
  // read the parameters
  EncryptionParameters parms_server;
  parms_server.load(if_parms_server);
   
  // read the 28+1 ciphertexts transferred from the client_enc
  SEALContext context_server(parms_server);
  Ciphertext c0_server [28];
  Ciphertext c1_server;
  Ciphertext cR_server;
  for (size_t i=0; i<28; i++) {
    c0_server[i].load(context_server, if_cipher_server);
  }
  c1_server.load(context_server, if_cipher_server);
  
  // perform homomorphic subtraction
  Evaluator evaluator(context_server);
  auto start = high_resolution_clock::now();
  for (size_t i=0; i<28; i++) { // for 28 ciphertexts
    evaluator.sub(c1_server, c0_server[i], cR_server);
    auto size_cR_server = cR_server.save(of_cipher_server);
  }
  auto end = high_resolution_clock::now();

  duration<double> ex_time_s =  end - start;
  duration<double, std::milli> ex_time_ms =  end - start;
  cout << "time: " << ex_time_s.count() << "s" << endl; 
  cout << "time: " << ex_time_ms.count() << "ms" << endl; 

  // close the ifstream and ofstream
  if_parms_server.close();
  if_cipher_server.close();
  of_cipher_server.close();

  cout << " ==> processing on the server is complete!" << endl;

  return 0;
}
