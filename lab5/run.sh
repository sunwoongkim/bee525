rm -rf inout/result_image.bmp
rm -rf network
mkdir network
#1_client_enc
cd 1_client_enc/build
./client_enc
mv *.bin ../../network/
#2_server
cd ../../2_server/build
./server
mv *.bin ../../network/
#3_client_dec
cd ../../3_client_dec/build
./client_dec
mv *.bmp ../../inout/
