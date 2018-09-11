/*
 * pipe.c
 *
 *      Author: Nine
 */
#include <stdio.h>
#include <mqueue.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>

#define kQUEUE_NAME                     "/message_queue"
#define kMQ_MESSAGE_DEFAULT_VALUE       8192
#define kMESSAGE_TO_SEND                "hello"


static mqd_t message_queue;


int main()
{
    char buffer[1024];

    message_queue = mq_open(kQUEUE_NAME, O_RDWR | O_CREAT, 0666, NULL);
    if (message_queue < 0) {
        printf("open message queue failed: %d\n", errno);
        exit(1);
    }

    printf("sned message\n");
    mq_send(message_queue, kMESSAGE_TO_SEND, strlen(kMESSAGE_TO_SEND), 0);

    printf("receiving message\n");
    mq_receive(message_queue, buffer, kMQ_MESSAGE_DEFAULT_VALUE, NULL);

    printf("receivce data: %s\n", buffer);
}
