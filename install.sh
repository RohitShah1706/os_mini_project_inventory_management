#!/bin/bash
cc -o server server.c server_handler.c messager.c server_controller.c -lpthread
cc -o client client.c client_handler.c messager.c  client_controller.c tableFormatters/fort.c