#!/bin/bash
cc -o server server.c server_handler.c messager.c server_controller.c locker.c log_generator.c tableFormatters/fort.c -lpthread
cc -o client client.c client_handler.c messager.c  client_controller.c log_generator.c tableFormatters/fort.c