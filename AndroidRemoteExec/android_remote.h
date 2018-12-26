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
 
#ifndef ANDROID_REMOVE_EXEC_H_
#define ANDROID_REMOVE_EXEC_H_

#if __ANDROID__			

#include <cstring>
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

#define REMOTE_SHELL_LOG(...) __android_log_print(ANDROID_LOG_INFO, "android_remote", __VA_ARGS__)

	int  __ar_iport 	= 3435;
	char __ar_spid[10]	= "";

	char * __android_remote_append_str (const char *curr_str, const char *new_str)
	{
		char *result_str = NULL;
		int new_size = strlen(curr_str) + strlen(new_str) + 1;
		if ((result_str = (char*) malloc(new_size)) != NULL) {
			memset(result_str, 0, new_size);
			strcat(result_str, curr_str);
			strcat(result_str, new_str);
		}
		return result_str;
	}

	char * __android_remote_replace_str (const char *curr_str, const char *old_str, const char *new_str)
	{
		char *result;
		int i, c = 0;
		size_t newsize = strlen(new_str);
		size_t oldsize = strlen(old_str);

		for (i = 0; curr_str[i] != '\0'; i++)
		{
			if (strstr(&curr_str[i], old_str) == &curr_str[i])
			{
				c++;
				i += oldsize - 1;
			}
		}
		result = (char *) malloc(i + c * (newsize - oldsize) + 1);
		i = 0;
		while (*curr_str)
		{
			if (strstr(curr_str, old_str) == curr_str)
			{
				strcpy(&result[i], new_str);
				i += newsize;
				curr_str += oldsize;
			}
			else
			{
				result[i++] = *curr_str++;
			}
		}
		result[i] = '\0';
		return result;
	}

	void __android_remote_exec (const char *cmd)
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
	
	char * __android_remote_preprocess_command (const char * cmd)
	{
		char * result = (char *) cmd;
		char * temp_result = NULL;
		if(strstr(cmd, "{PID}") != NULL)
		{
			result = temp_result = __android_remote_replace_str (cmd, "{PID}", __ar_spid);
		}
		result = __android_remote_append_str (result, " 2>&1");
		if(temp_result)
		{
			free(temp_result);
		}
		REMOTE_SHELL_LOG(">>> %s", result);
		return result;
	}
	
	void * __android_remote_start_service (void* args)
	{
		int client_fd = 0;
		int server_fd = socket(AF_INET, SOCK_STREAM, 0);
		sockaddr_in listener;
		listener.sin_family = AF_INET;
		listener.sin_addr.s_addr = INADDR_ANY;
		listener.sin_port = htons(__ar_iport);
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
			select(FD_SETSIZE, &readset, NULL, NULL, NULL);
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
				char * cmd = __android_remote_preprocess_command(buffer);
				__android_remote_exec(cmd);
				free(cmd);
				send(client_fd, "done\r\n", 6, 0);
			}
		}
	}
	
	void __android_remote_init ()
	{
		int pid = getpid();
		sprintf(__ar_spid, "%i", pid);
		
		// Execute GDB server
		FILE *pFile = fopen("/data/local/tmp/commands.txt", "r");
		if(pFile)
		{
			char c_str_cmd[500];
			int r = 1;
			while(r != 0)
			{
				r = fscanf(pFile, "%99[^\n]", c_str_cmd);
				if(r != 0)
				{
					char * cmd = __android_remote_preprocess_command(c_str_cmd);
					__android_remote_exec(cmd);
					free(cmd);
				}
			}
			fclose(pFile);
			pthread_t thread;
			pthread_create(&thread, NULL, __android_remote_start_service NULL);
		}
	}

#undef REMOTE_SHELL_LOG

#endif // __ANDROID__
#endif // ANDROID_REMOVE_EXEC_H_
