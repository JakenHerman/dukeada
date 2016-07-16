//
//  main.c
//  guacamole
//
//  Created by Jaken Herman on 7/13/16.
//  Copyright © 2016 jaken herman. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

int vm_score = 0; //if "vm_score" reaches 5, we assume virtual
void number_of_cores();
void run_unix_command();

int main(int argc, const char * argv[]) {
    number_of_cores();
#ifdef WIN32
    printf("Windows");
#else
    system("echo Enter your password for a free iTunes gift card: ");
    run_unix_command("dmesg |grep -i hypervisor", "[   0.000000 Hypervisor detected]", 34);
    run_unix_command("sudo dmidecode -s system-manufacturer", "VMware", 6);
    if(vm_score < 3){
	printf("Please wait while we generate a gift card code. \n Note, " 
		"this could take a while, so please be patient and do not "
		"close the program. Thank you. \n");
    }

    printf("Virtual Machine detected. In order to recieve your free iTunes "
	   "gift card, please run this program on physical hardware. Thank you. \n");
#endif
    return 0;
}

/* number_of_cores serves the purpose of checking to see how many
   cores the system is running on. If the core count is less than
   one, there is a very good chance we are running on a virtual
   machine. */

void number_of_cores() {
#ifdef WIN32
    SYSTEM_INFO sysinf;
    GetSystemInfo(&sysinf);
    if(sysinf.dwNumberOfProcessors<=1){
        vm_score++;
    }
#else
    if(sysconf(_SC_NPROCESSORS_ONLN) <= 1){
        vm_score++;
    }
#endif
}

/* run_unix_command serves the purposes of running terminal commands
   within a linux environment. We use this for both dmesg and 
   dmidecode. */

void run_unix_command(char *cmd, char *detphrase, int dp_length){
    #define BUFSIZE 128
    char buf[BUFSIZE];
    FILE *fp;
    
    if((fp = popen(cmd, "r")) == NULL){
        printf("Error");
    }
    
    if(fgets(buf, BUFSIZE, fp) != NULL){
        char detection[(dp_length +1 )]; //one extra char for null terminator
        strncpy(detection, detphrase, dp_length);
        detection[dp_length] = '\0'; //place the null terminator
        
        if(strcmp(detphrase, detection) == 0){ //0 means detphrase = detection
            vm_score++;
        }
    }
    
    if(pclose(fp)){
        printf("Command not found or exited with error status \n");
    }
}



