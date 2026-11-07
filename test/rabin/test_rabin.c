#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#define RABIN_POLY UINT64_C(0x3DA3358B4DC173)
#define RABIN_WINDOW_SIZE 48


bool poly_divides(uint64_t a){
  uint64_t ours = RABIN_POLY;
  for (int i = 0; i < 64;i ++){
    int max_bits  = 64 -  __builtin_clzll(a);
    printf("max bits = %d\n", max_bits);
    if( max_bits >=  54){
      a ^= ours << (max_bits - 54);
      printf("xor a = %lu\n", a);
      if(a == 0){
	return true;
      }else if(64 - __builtin_clzll(a) <  54){
	printf("the remainder %lu is less than our poly, quitting with false \n", a);
	break;
      }else{
	printf("trying one more time\n");
      }
    }else {
      break;;
    }
  }
  return false;
}


uint64_t poly_division_remainder(uint64_t a){
  if(a == 0){
    return  0;
  }
  uint64_t ours = RABIN_POLY;
  for (int i = 0; i < 64;i ++){
    int max_bits  = 64 -  __builtin_clzll(a);
    printf("max bits = %d\n", max_bits);
    if( max_bits >=  54){
      a ^= ours << (max_bits - 54);
      printf("xor a = %lu\n", a);
      if(a == 0){
	return 0;
      }else if(64 - __builtin_clzll(a) <  54){
	printf("the remainder %lu is less than our poly, quitting with false \n", a);
	break;
      }else{
	printf("trying one more time\n");
      }
    }else {
      break;;
    }
  }
  return a;
}




int main(void){
  printf("Working with Rabin\n");

  const uint8_t test_window[RABIN_WINDOW_SIZE] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
    0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
    0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
    0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F
  };

  assert(sizeof(test_window) == RABIN_WINDOW_SIZE);

  uint64_t a = UINT64_C(0x7B466B169B82E6);
  uint64_t b = UINT64_C(0x7B466B169B82E7);
      
  assert(poly_divides(a));
  assert(!poly_divides(b));
  uint64_t remainder = 0;
  uint64_t temp = 0;
  for (int y = 0;y <8; y++ ){
    temp |= (uint64_t)test_window[y] << ((7 - y)*8);
    }
    remainder = poly_division_remainder(temp);

  for (int i =8; i < RABIN_WINDOW_SIZE; i++ ){
    uint64_t n_r  =  remainder << 8;
    uint64_t c_b =   (uint64_t) test_window[i];
    n_r |= c_b;
    remainder = poly_division_remainder(n_r);
    printf("new remainde  now is %lu\n", remainder);
  }
    
}
