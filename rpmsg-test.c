/*
 * rpmsg-test.c
 * This file is part of <rpmsg-test>
 *
 * Copyright (C) 2017 - Pierre-Jean Texier <texier.pj2@gmail.com>
 *
 * <rpmsg-test> is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * <rpmsg-test> is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with <rpmsg-test>. If not, see <http://www.gnu.org/licenses/>.
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <termios.h>
#include <getopt.h>

#define TTY_RPMSG	"/dev/ttyRPMSG"

int rpmsgfd;

static void display_help(const char * name)
{
    	fprintf(stderr, "usage: %s options...\n", name);
	fprintf(stderr, " options:\n");
	fprintf(stderr, " -r --RL1\n");
	fprintf(stderr, " -l --RL2 \n");
	fprintf(stderr, " -h --help Prints this help\n\n");
	fprintf(stderr, "Example: %s --RL1=1\n\n", name);
}

int rpmsg_init()
{
	int fd;
	struct termios cfg_tty;

	// Open tty
	fd = open(TTY_RPMSG, O_RDWR | O_NOCTTY );
	if (fd < 0){
		perror(TTY_RPMSG);
		return EXIT_FAILURE;
	}

	// init cfg_tty
	memset(&cfg_tty, 0, sizeof(cfg_tty));
	
	fcntl(fd, F_SETFL, 0);
	tcgetattr(fd, &cfg_tty);
	
	cfg_tty.c_lflag = ICANON;
	cfg_tty.c_iflag &= ~IGNBRK;
    	cfg_tty.c_lflag = 0;                
	cfg_tty.c_oflag = 0;                
	if (tcsetattr(fd,TCSANOW, &cfg_tty) != 0)
		return EXIT_FAILURE;
	
	return fd;
}

int main(int argc, char *argv[])
{
	// Variables
	int len = 0;
	int ret = 0;
	int opt = 0;
	int long_index = 0;
	char tx_buffer[512] = {0};
	char rx_buffer[512] = {0};
	
	// Init tty
	rpmsgfd = rpmsg_init();
	if (rpmsgfd == EXIT_FAILURE) {
		printf("Please insert the imx_rpmsg_tty module\n");
		exit(EXIT_FAILURE);
	}
	
	// Check argc
	if (argc < 2){
		display_help(argv[0]);
		exit(EXIT_FAILURE);
	}
	
	// Options
	static struct option option[] = 
	{
		{"RL1", 	required_argument, 		NULL, 'r' },		
		{"RL2", 	required_argument, 		NULL, 'l' },
		{"help", 	no_argument, 			NULL, 'h' },
		{0, 		0, 				0, 	0 }
	};
			
	while ((opt = getopt_long(argc, argv, "r:l:h", option, &long_index)) >= 0) {
		switch(opt) {
			case 'r':
				len = snprintf(tx_buffer, sizeof(tx_buffer), "!out_RL1:%s", optarg);	
				break;
			case 'l':
				len = snprintf(tx_buffer, sizeof(tx_buffer), "!out_RL2:%s", optarg);	
				break;
			case 'h':
				display_help(argv[0]);
				exit(EXIT_SUCCESS);	
			default:
				fprintf(stderr,"[ERROR] %s: Bad option. -h for help\n", argv[0]);
				exit(EXIT_FAILURE);
		}				
	}
		
	// Write to rpmsg tty
	ret = write(rpmsgfd, tx_buffer, len);
	if(ret <= 0)
		perror("write");
		
	// Read from rpmsg tty
	ret = read(rpmsgfd, rx_buffer, sizeof(rx_buffer));
	if(ret > 0)
		printf("received from cortex M4 = %s", rx_buffer);
	else
		return EXIT_FAILURE;
		
	close(rpmsgfd);
	
	return EXIT_SUCCESS;
}



