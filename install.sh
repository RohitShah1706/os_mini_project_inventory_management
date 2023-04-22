#!/bin/bash
cc -o init init.c 
cc -o server server.c handler/server_handler.c controller/server_controller.c router/server_router.c messager/messager.c locker/locker.c logger/log_generator.c tableFormatters/fort.c -lpthread
cc -o client client.c handler/client_handler.c controller/client_controller.c router/client_router.c messager/messager.c logger/log_generator.c tableFormatters/fort.c