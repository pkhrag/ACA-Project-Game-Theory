//  Hello World server

#include <zmq.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

int main (void)
{
    //  Socket to talk to clients
    void *context = zmq_ctx_new ();
    void *responder = zmq_socket (context, ZMQ_REP);
    int rc = zmq_bind (responder, "tcp://*:5555");
    assert (rc == 0);
	int i=0,sum=0;
    while (1) {
        char bufferq [6];
        char buffersum[6];
        zmq_recv (responder, bufferq, 6, 0);
        //zmq_recv(responder,buffersum,6,0);
        int q=i*i*i;
        
        printf ("Received sum=%d,q=%d\n",sum,atoi(bufferq));
        sleep (1);      
        sum+=q+atoi(bufferq);    //  Do some 'work'
        //itoa(q,bufferq,10);
        int n=sprintf(bufferq,"%d",q);
        zmq_send (responder, bufferq, 6, 0);
        i++;
    }
    return 0;
}
