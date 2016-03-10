//  Hello World client
#include <zmq.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main (void)
{
    printf ("Connecting to hello world server...\n");
    void *context = zmq_ctx_new ();
    void *requester = zmq_socket (context, ZMQ_REQ);
    zmq_connect (requester, "tcp://localhost:5555");
    int sum=0,i=0;
    int request_nbr;
    for (request_nbr = 0; request_nbr != 10; request_nbr++) {
        char buffer [10];
        int q=request_nbr;
        char a[7];
        //itoa(q,a,10);
        int n=sprintf(a,"%d",q);
		char b[7];
		//itoa(sum,b,10);
        n=sprintf(b,"%d",sum);
        printf ("Sending sum=%d,q=%d\n", sum,q);
        zmq_send (requester, a, 7, 0);
        //zmq_recv (requester, buffer, 10, 0);
        //zmq_send(requester,b,7,0);
        zmq_recv (requester, buffer, 10, 0);
        sum+=q;
        printf ("Received  sum=%d, a=%s\n", sum,buffer);
        sum+=atoi(buffer);
    }
    zmq_close (requester);
    zmq_ctx_destroy (context);
    return 0;
}
