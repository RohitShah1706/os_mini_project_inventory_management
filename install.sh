#!/bin/bash
cc -o server server.c server_handler.c messager.c -lpthread
cc -o client client.c client_handler.c messager.c 