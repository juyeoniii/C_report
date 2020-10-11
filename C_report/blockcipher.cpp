#include <stdio.h>
#include <string.h>
 
inline unsigned char front(unsigned char uc) //16진수 4비트씩 쪼개는 함수
{
    return ((unsigned char)0xF0 & uc) >> 4;  //앞 4비트
}
 
inline unsigned char back(unsigned char uc) //16진수 4비트씩 쪼개는 함수
{
    return ((unsigned char)0x0F & uc); //뒤 4비트
}

void BlockCipherEncrypt(unsigned int K, unsigned int P)  //블록 암호 함수
{
	unsigned int x,i = 0;
	
	char Xto8bit[8], Kto8bit[8];   //0x12345678 형식을 0x12, 0x34 … 로 쪼갬
	char Xto4bit[8], Kto4bit[8];   //0x12를 4비트씩 받는 변수
	
	int S[16] = {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7}; //S값 초기화
	int Y[16];  //Y 선언
	int Z[8];  //Z 선언
	
	char resultX[8];  //cipher 담을 배열
	
	x = P ^ K;   //평문과 비밀키를 XOR 연산함
	
	memcpy(Xto8bit, &x, 8); //32비트를 8비트씩 쪼개서 0x12, 0x34 ~로 나눔
	for(i = 0; i < 4; i++) //1, 2, 3, 4, 5, 6, 7, 8 로 쪼개질때까지 반복
	{	
	   Xto4bit[2*i] = back(Xto8bit[i]); //0x78로 표현된 수가 8로 나옴 (X -> 4비트)
	   Xto4bit[2*i+1] = front(Xto8bit[i]); //0x78로 표현된 수가 7로 나옴 (X->4비트)
	}
	
	memcpy(Kto8bit, &K, 8);  //32비트를 8비트씩 쪼개서 0x12, 0x34 ~로 나눔
	for(i = 0; i < 4; i++) //1, 2, 3, 4, 5, 6, 7, 8 로 쪼개질때까지 반복
	{
	    Kto4bit[2*i] = back(Kto8bit[i]);  //0x78로 표현된 수가 8로 나옴 (K->4비트)
	    Kto4bit[2*i+1] = front(Kto8bit[i]); //0x78로 표현된 수가 7로 나옴 (K->4비트)
	}
	
	for(i = 0; i < 8; i++)
	{
	  Y[i] = S[Xto4bit[i]];  // 최하위 4비트부터 나온 16진수 값을 차례대로 S->Y값
	}
	
	Z[0] = Y[0] ^ Y[1] ^ Y[3] ^ Y[5] ^ Y[6] ^ Y[7];  //행렬곱 선언
	Z[1] = Y[0] ^ Y[1] ^ Y[3] ^ Y[4] ^ Y[6] ^ Y[7];
	Z[2] = Y[0] ^ Y[1] ^ Y[2] ^ Y[4] ^ Y[5] ^ Y[7];
	Z[3] = Y[1] ^ Y[2] ^ Y[3] ^ Y[4] ^ Y[5] ^ Y[6];
	Z[4] = Y[0] ^ Y[1] ^ Y[5] ^ Y[6] ^ Y[7];
	Z[5] = Y[1] ^ Y[2] ^ Y[4] ^ Y[6] ^ Y[7];
	Z[6] = Y[2] ^ Y[3] ^ Y[4] ^ Y[5] ^ Y[7];
	Z[7] = Y[0] ^ Y[3] ^ Y[4] ^ Y[5] ^ Y[6];
	
	
	for(i = 0; i < 8; i++)
	{	
		resultX[i] = Z[i] ^ Kto4bit[i];  //Z[i] 와 K를 한번 더 XOR 연산해줌
	}
	
	printf("암호문 : 0x");
	for(i = 7; i >= 0; i--)  //최하위비트부터 나타냈던 16진수를 다시 순서대로 표기
	{
		printf("%x", resultX[i]);
	}
	
}


int main(void)
{
	unsigned int P = 0x12345678;  //평문
	unsigned int K = 0xC58FA10B;  //비밀키
	
	BlockCipherEncrypt(K, P);  //암호문 호출
	
	return 0;
}
