/*
 * data sign -- We should put a data_sign in the head
 * of a data, so when the server get a data, it can distinct
 * the type of the data, that's very import for modules.
 *
 * author: Lei Liu <liunx163@163.com>
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	unsigned int sign; // the signature
	unsigned int reserve; // for future use
} data_sign_t;

enum {
	TCP_DATA_STREAM = 0x0010,
	UDP_DATA_STREAM = 0x0020,
};

int main()
{
	char buf[64] = {0};
	data_sign_t *data_sign;
	data_sign = (data_sign_t *)malloc(sizeof(data_sign_t));

	data_sign->sign = TCP_DATA_STREAM;
	data_sign->reserve = 0xffff;
	memcpy(buf, data_sign, sizeof(data_sign_t));
	sprintf(buf + sizeof(data_sign_t), "%s", "Hello, Liunx!\n");

	printf("%s", buf + sizeof(data_sign_t));
	// OK, we combinate the data signature and data contents
	// then let's abstract the data signature
	data_sign_t *data = (data_sign_t *)malloc(sizeof(data_sign_t));
	memcpy(data, buf, sizeof(data_sign_t));

	switch (data->sign) {
		case TCP_DATA_STREAM:
			printf("The TCP_DATA_STREAM.\n");
			break;
		case UDP_DATA_STREAM:
			printf("The UDP_DATA_STREAM.\n");
			break;
		default:
			printf("Can recognize the signature!\n");
			break;
	}

	return 0;
}


