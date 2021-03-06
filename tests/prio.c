/*
    Copyright (c) 2013 250bpm s.r.o.  All rights reserved.
    Copyright (c) 2013 GoPivotal, Inc.  All rights reserved.

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom
    the Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included
    in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
    IN THE SOFTWARE.
*/

#include "../src/nn.h"
#include "../src/pipeline.h"

#include "testutil.h"

#define SOCKET_ADDRESS_A "inproc://a"
#define SOCKET_ADDRESS_B "inproc://b"

int main ()
{
    int rc;
    int push;
    int pull1;
    int pull2;
    int sndprio;
    char buf [3];

    pull1 = test_socket (AF_SP, NN_PULL);
    test_bind (pull1, SOCKET_ADDRESS_A);
    pull2 = test_socket (AF_SP, NN_PULL);
    test_bind (pull2, SOCKET_ADDRESS_B);
    push = test_socket (AF_SP, NN_PUSH);
    sndprio = 1;
    rc = nn_setsockopt (push, NN_SOL_SOCKET, NN_SNDPRIO,
        &sndprio, sizeof (sndprio));
    errno_assert (rc == 0);
    test_connect (push, SOCKET_ADDRESS_A);
    sndprio = 2;
    rc = nn_setsockopt (push, NN_SOL_SOCKET, NN_SNDPRIO,
        &sndprio, sizeof (sndprio));
    errno_assert (rc == 0);
    test_connect (push, SOCKET_ADDRESS_B);

    test_send (push, "ABC");
    test_send (push, "DEF");
    test_recv (pull1, "ABC");
    test_recv (pull1, "DEF");

    test_close (pull1);
    test_close (push);
    test_close (pull2);

    return 0;
}

