/* 
	* @author	Sunwoong Sunny Kim
	* @brief	this file is for the B EE 525 class and cannot be distributed without permission of the author
	*         it is mainly based on C syntax
*/

#include "examples.h"

using namespace std;
using namespace seal;

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
   
  // read the two ciphertexts transferred from the client_enc
  SEALContext context_server(parms_server);
  Ciphertext c0_server, c1_server;
  Ciphertext cR_server;
  c0_server.load(context_server, if_cipher_server);
  c1_server.load(context_server, if_cipher_server);
  
  // perform homomorphic subtraction
  Evaluator evaluator(context_server);
  evaluator.sub(c1_server, c0_server, cR_server);
  auto size_cR_server = cR_server.save(of_cipher_server);

  // close the ifstream and ofstream
  if_parms_server.close();
  if_cipher_server.close();
  of_cipher_server.close();

  cout << " ==> processing on the server is complete!" << endl;

  return 0;
}
