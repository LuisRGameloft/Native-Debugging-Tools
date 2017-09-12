/*
 * MIT License
 * 
 * Copyright (c) 2017 David Landeros
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "InAppRemoteShell.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <android/log.h>
#define REMOTE_SHELL_LOG(...) __android_log_print(ANDROID_LOG_INFO, "InAppRemoteShell", __VA_ARGS__)

int  InAppRemoteShell::s_iPort = 3435;
char InAppRemoteShell::s_sPid[10] = "";

/* THIS METHOD MUST BE CALLED FROM JNI_OnLoad(...)
 *  - Reads and execute "/sdcard/commands.txt" if exists
 *  - Creates the posix thread to start listening for remote commands
 */
void InAppRemoteShell::Init()
{
    int pid = getpid();
    sprintf(s_sPid, "%i", pid);
    FILE *pFile = fopen("/sdcard/commands.txt", "r");
    if(pFile)
    {
        char c_str_cmd[500];
        int r = 1;
        while(r != 0)
        {
            r = fscanf(pFile, "%99[^\n]", c_str_cmd);
            if(r != 0)
            {
                std::string cmd(c_str_cmd);
                Exec(PreprocessCommand(cmd).c_str());
            }
        }
        fclose(pFile);
    }
    pthread_t thread;
    pthread_create(&thread, NULL, StartService, NULL);
}

void *InAppRemoteShell::StartService(void*args)
{
    int client_fd = 0;
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in listener;
    listener.sin_family = AF_INET;
    listener.sin_addr.s_addr = INADDR_ANY;
    listener.sin_port = htons(s_iPort);
    bind(server_fd, (sockaddr*)&listener, sizeof listener);
    listen(server_fd, SOMAXCONN);
    while(true)
    {
        fd_set readset;
        FD_ZERO(&readset);
        FD_SET(server_fd, &readset);
        if(client_fd != 0)
        {
            FD_SET(client_fd, &readset);
        }
        int no_events = select(FD_SETSIZE, &readset, NULL, NULL, NULL);
        if(FD_ISSET(server_fd, &readset))
        {
            REMOTE_SHELL_LOG("New client connected\n" );
            client_fd = accept(server_fd, NULL, NULL);
            send(client_fd, "welcome to in-app-remote-shell\r\n", 32, 0);
        }
        else if(FD_ISSET(client_fd, &readset))
        {
            char buffer[1024];
            memset(buffer, 0, 1024);
            int len = recv(client_fd, buffer, sizeof buffer-1, 0);
            if(len < 1) {
                REMOTE_SHELL_LOG("Client disconnected\n" );
                client_fd = 0;
                continue;
            }
            buffer[len] = 0x00;
            std::string cmd(buffer);
            Exec(PreprocessCommand(cmd).c_str());
            send(client_fd, "done\r\n", 6, 0);
        }
    }
}

std::string& InAppRemoteShell::PreprocessCommand(std::string& cmd)
{
    size_t pos = cmd.find("{PID}");
    if(pos != std::string::npos)
    {
        cmd.replace(pos, 5, s_sPid);
    }
    REMOTE_SHELL_LOG(">>> %s",cmd.c_str());
    return cmd.append(" 2>&1");
}

void InAppRemoteShell::Exec(const char *cmd)
{
    if (fork() == 0)
    {
        FILE *fp;
        char path[1035];
        fp = popen(cmd, "r");
        if (fp == NULL) {
            REMOTE_SHELL_LOG("Failed to run command\n" );
        }
        while (fgets(path, sizeof(path)-1, fp) != NULL) {
            REMOTE_SHELL_LOG("%s", path);
        }
        pclose(fp);
        exit(0); // exit child process
    }
}