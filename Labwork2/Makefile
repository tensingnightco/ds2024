#!/bin/bash
rpcgen filetrans.x
gcc -c fileclient.c -o fileclient.o
gcc -c filetrans_clnt.c -o filetrans_clnt.o
gcc -c filetrans_xdr.c -o filetrans_xdr.o
gcc -o fileclient fileclient.o filetrans_clnt.o filetrans_xdr.o
gcc -c fileclient.c fileserver.c filetrans_xdr.c
gcc -c fileserver.c -o fileserver.o
gcc -c filetrans_svc.c -o filetrans_svc.o
gcc -o fileserver fileserver.o filetrans_svc.o filetrans_xdr.o
