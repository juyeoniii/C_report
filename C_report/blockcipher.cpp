#include <stdio.h>
#include <string.h>
 
inline unsigned char front(unsigned char uc) //16���� 4��Ʈ�� �ɰ��� �Լ�
{
    return ((unsigned char)0xF0 & uc) >> 4;  //�� 4��Ʈ
}
 
inline unsigned char back(unsigned char uc) //16���� 4��Ʈ�� �ɰ��� �Լ�
{
    return ((unsigned char)0x0F & uc); //�� 4��Ʈ
}

void BlockCipherEncrypt(unsigned int K, unsigned int P)  //��� ��ȣ �Լ�
{
	unsigned int x,i = 0;
	
	char Xto8bit[8], Kto8bit[8];   //0x12345678 ������ 0x12, 0x34 �� �� �ɰ�
	char Xto4bit[8], Kto4bit[8];   //0x12�� 4��Ʈ�� �޴� ����
	
	int S[16] = {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7}; //S�� �ʱ�ȭ
	int Y[16];  //Y ����
	int Z[8];  //Z ����
	
	char resultX[8];  //cipher ���� �迭
	
	x = P ^ K;   //�򹮰� ���Ű�� XOR ������
	
	memcpy(Xto8bit, &x, 8); //32��Ʈ�� 8��Ʈ�� �ɰ��� 0x12, 0x34 ~�� ����
	for(i = 0; i < 4; i++) //1, 2, 3, 4, 5, 6, 7, 8 �� �ɰ��������� �ݺ�
	{	
	   Xto4bit[2*i] = back(Xto8bit[i]); //0x78�� ǥ���� ���� 8�� ���� (X -> 4��Ʈ)
	   Xto4bit[2*i+1] = front(Xto8bit[i]); //0x78�� ǥ���� ���� 7�� ���� (X->4��Ʈ)
	}
	
	memcpy(Kto8bit, &K, 8);  //32��Ʈ�� 8��Ʈ�� �ɰ��� 0x12, 0x34 ~�� ����
	for(i = 0; i < 4; i++) //1, 2, 3, 4, 5, 6, 7, 8 �� �ɰ��������� �ݺ�
	{
	    Kto4bit[2*i] = back(Kto8bit[i]);  //0x78�� ǥ���� ���� 8�� ���� (K->4��Ʈ)
	    Kto4bit[2*i+1] = front(Kto8bit[i]); //0x78�� ǥ���� ���� 7�� ���� (K->4��Ʈ)
	}
	
	for(i = 0; i < 8; i++)
	{
	  Y[i] = S[Xto4bit[i]];  // ������ 4��Ʈ���� ���� 16���� ���� ���ʴ�� S->Y��
	}
	
	Z[0] = Y[0] ^ Y[1] ^ Y[3] ^ Y[5] ^ Y[6] ^ Y[7];  //��İ� ����
	Z[1] = Y[0] ^ Y[1] ^ Y[3] ^ Y[4] ^ Y[6] ^ Y[7];
	Z[2] = Y[0] ^ Y[1] ^ Y[2] ^ Y[4] ^ Y[5] ^ Y[7];
	Z[3] = Y[1] ^ Y[2] ^ Y[3] ^ Y[4] ^ Y[5] ^ Y[6];
	Z[4] = Y[0] ^ Y[1] ^ Y[5] ^ Y[6] ^ Y[7];
	Z[5] = Y[1] ^ Y[2] ^ Y[4] ^ Y[6] ^ Y[7];
	Z[6] = Y[2] ^ Y[3] ^ Y[4] ^ Y[5] ^ Y[7];
	Z[7] = Y[0] ^ Y[3] ^ Y[4] ^ Y[5] ^ Y[6];
	
	
	for(i = 0; i < 8; i++)
	{	
		resultX[i] = Z[i] ^ Kto4bit[i];  //Z[i] �� K�� �ѹ� �� XOR ��������
	}
	
	printf("��ȣ�� : 0x");
	for(i = 7; i >= 0; i--)  //��������Ʈ���� ��Ÿ�´� 16������ �ٽ� ������� ǥ��
	{
		printf("%x", resultX[i]);
	}
	
}


int main(void)
{
	unsigned int P = 0x12345678;  //��
	unsigned int K = 0xC58FA10B;  //���Ű
	
	BlockCipherEncrypt(K, P);  //��ȣ�� ȣ��
	
	return 0;
}
