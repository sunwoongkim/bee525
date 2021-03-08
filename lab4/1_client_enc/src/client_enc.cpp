/* 
	* @author	Sunwoong Sunny Kim
	* @brief	this file is for the B EE 525 class and cannot be distributed without permission of the author
	*         it is mainly based on C syntax
*/

#include "examples.h"

using namespace std;
using namespace seal;

#define DOWNTOPBMP 1
  
unsigned char bmp_header[54];

void read_img (unsigned char *o_buf) {
  FILE* fr = fopen("../../inout/test_image0.bmp", "rb");
  unsigned char i_buf[3*28*28];
  int tmpI;
  size_t result;

  // skip the 54B bmp_header
  result = fread(bmp_header, sizeof(unsigned char), 54, fr);

  // read R, G, and B pixel values
  for (int i=0; i<3; i++) 
  for (int r=0; r<28; r++) 
  for (int c=0; c<28; c++) {
    result = fread(i_buf, sizeof(unsigned char), 3*28*28, fr);
  }
  
  // crop and convert the image format from RGB to Y (monochrome)
  for (int r=0; r<28; r++) {
    for (int c=0; c<28; c++) {
      tmpI = ((int)i_buf[3*(r*28+c)] + 2*(int)i_buf[3*(r*28+c)+1] + (int)i_buf[3*(r*28+c)+2])/4;
      if (DOWNTOPBMP == 1) { // because the captured image is upside down
        o_buf[(28-r-1)*28+c] = (unsigned char)tmpI;
      } else {
        o_buf[r*28+c] = (unsigned char)tmpI;
      }
    }
  }

  fclose(fr);
}

int main(void) {
  // prepare to write files
  ofstream of_parms;
  ofstream of_sk;
  ofstream of_cipher_client;
  of_parms.open("parms.bin");
  of_sk.open("sk.bin");
  of_cipher_client.open("cipherI.bin");

  // read an image
  unsigned char plaintext_buf [28*28];
  read_img(plaintext_buf);
  
  // set parameters
  EncryptionParameters parms_client1(scheme_type::ckks); // the CKKS homomorphic encryption scheme is used
  size_t poly_modulus_degree = 8192; // 4K slots are available
  parms_client1.set_poly_modulus_degree(poly_modulus_degree);
  parms_client1.set_coeff_modulus(CoeffModulus::Create(poly_modulus_degree, { 60, 40, 40, 60 })); // less than 4 multiplications
  auto parms_size = parms_client1.save(of_parms); // save parameters
   
  // generate keys
  SEALContext context_client1(parms_client1);
  KeyGenerator keygen(context_client1);
  auto sk_client1 = keygen.secret_key(); // secret key generation 
  PublicKey pk_client1; 
  keygen.create_public_key(pk_client1); // public key generation
  sk_client1.save(of_sk); // save the secret key for decryption
 
  // encode data0 (input image)
  double scale = pow(2.0, 40);
  CKKSEncoder encoder_client1(context_client1);
  size_t slot_count = encoder_client1.slot_count();
  vector<double> input0;
  input0.reserve(slot_count);
  for (size_t i=0; i<slot_count; i++) {
    double norm_pxl = 0.0; 
    if (i < 28*28) { // use 28x28 slots out of 4K slots
      norm_pxl = (double)plaintext_buf[i] / 255; 
    }
    input0.push_back(norm_pxl); // put 28x28 pixels into the input0 vector
  }
  Plaintext p0;
  encoder_client1.encode(input0, scale, p0); // encoding

  // encode data1 (constant 255)
  vector<double> input1;
  input1.reserve(slot_count);
  for (size_t i=0; i<slot_count; i++) {
    input1.push_back(1.0); // 1.0 = 255 / maximum_pixel_value
  }
  Plaintext p1;
  encoder_client1.encode(input1, scale, p1); // encoding

  // encrypt the two plaintext polynomials
  Encryptor encryptor(context_client1, pk_client1);
  auto size_c0 = encryptor.encrypt(p0).save(of_cipher_client);
  auto size_c1 = encryptor.encrypt(p1).save(of_cipher_client);
        
  // close the ofstream
  of_parms.close();
  of_sk.close();
  of_cipher_client.close();

  cout << " ==> processing on the client_enc is complete!" << endl;

  return 0;
}
