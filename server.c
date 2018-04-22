#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>

#include<stdlib.h>

// nc localhost 5555

static void* thread_main(void* arg);

int main() {
    int sock, count;
    //int tcp_in;
    struct sockaddr_in tcp_addr;
    pthread_t my_thread;

    // 1
    sock = socket(AF_INET, SOCK_STREAM, 0);

    // 2
    tcp_addr.sin_family = AF_INET;
    tcp_addr.sin_port = htons(5555); // Порт. Не выбирать < 1024, они там привелегированные
    tcp_addr.sin_addr.s_addr = htonl(INADDR_ANY); // 0.0.0.0

    int check;
    while (1) {  // пока не забиндимся
        check = bind(sock, (struct sockaddr*) &tcp_addr, sizeof(tcp_addr));
        if (check == 0)  {
            break;
        } else {
            printf("Error.\n");
            sleep(5);
        }
    }
    printf("Server up and ready.\n");

    // 3
    listen(sock, 5);
    while (1) {
        // 4
        int* tcp_in = (int*)malloc(sizeof(tcp_in));
        *tcp_in = accept(sock, NULL, 0);

        pthread_create(&my_thread, NULL, thread_main, tcp_in);
    }
    return 0;
}


static void* thread_main(void* arg) { 
    int count;                        
    char buf[256];
    int* tcp_in = (int*) arg;

    // 5
    sprintf(buf, "Hello client %i\n", *tcp_in);
    sleep(2);
    send(*tcp_in, buf, strnlen(buf, sizeof(buf)), 0);

    // 6
    shutdown(*tcp_in, SHUT_RDWR);
    free(tcp_in);
}
