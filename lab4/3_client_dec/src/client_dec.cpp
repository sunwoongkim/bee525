/* 
	* @author	Sunwoong Sunny Kim
	* @brief	this file is for the B EE 525 class and cannot be distributed without permission of the author
	*         it is mainly based on C syntax
*/

#include "examples.h"

using namespace std;
using namespace seal;

#define DOWNTOPBMP 1
  
unsigned char bmp_header[54] = {66, 77, 102, 9, 0, 0, 0, 0, 0, 0, 
                                54, 0, 0, 0, 40, 0, 0, 0, 28, 0, 
                                0, 0, 28, 0, 0, 0, 1, 0, 24, 0, 
                                0, 0, 0, 0, 48, 9, 0, 0, 18, 23, 
                                0, 0, 18, 23, 0, 0, 0, 0, 0, 0, 
                                0, 0, 0, 0};

void write_img (unsigned char *i_buf) {
  FILE* fw = fopen("result_image.bmp", "wb");
  size_t result;

  // write the 54B bmp_header (image size, format, etc.)
  result = fwrite(bmp_header, sizeof(unsigned char), 54, fw);

  // write R, G, and B pixel values
  for (int r=0; r<28; r++) 
  for (int c=0; c<28; c++) {
    result = fwrite(&i_buf[r*28+(28-c-1)], sizeof(unsigned char), 1, fw);
    result = fwrite(&i_buf[r*28+(28-c-1)], sizeof(unsigned char), 1, fw);
    result = fwrite(&i_buf[r*28+(28-c-1)], sizeof(unsigned char), 1, fw);
  }
  
  fclose(fw);
}

int main(void) {
  // prepare to write bin files
  ifstream if_parms_client2;
  ifstream if_sk_client2;
  ifstream if_cipher_client2;
  if_parms_client2.open("../../network/parms.bin");
  if_sk_client2.open("../../network/sk.bin");
  if_cipher_client2.open("../../network/cipherR.bin");
 
  // read the parameters
  EncryptionParameters parms_client2;
  parms_client2.load(if_parms_client2);
  
  // read the secret key transferred from the client_enc
  SEALContext context_client2(parms_client2);
  SecretKey sk_client2;
  sk_client2.load(context_client2, if_sk_client2);
  
  // read the result ciphertext transferred from the server
  Ciphertext cR_client2;
  cR_client2.load(context_client2, if_cipher_client2);

  // decrypt the result ciphertext
  Plaintext pR;
  Decryptor decryptor(context_client2, sk_client2);
  decryptor.decrypt(cR_client2, pR);
  
  // decode the result plaintext polynomial
  vector<double> result;
  CKKSEncoder encoder_client2(context_client2);
  encoder_client2.decode(pR, result);

  // write the result image
  unsigned char plaintext_buf [28*28];

  reverse(result.begin(), result.end());
  for (int i=0; i<28*28; i++) {
    plaintext_buf[28*28-i-1] = 255*result.back(); // vector::back(): extracts an element from the vector
    result.pop_back(); // vector::pop_back(): removes the element from the vector
  }
  write_img(plaintext_buf);
  
  // close the ifstream
  if_parms_client2.close();
  if_sk_client2.close();
  if_cipher_client2.close();

  cout << " ==> processing on the client_dec is complete!" << endl;

  return 0;
}
