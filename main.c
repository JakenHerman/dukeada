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
void run_command();
int main(int argc, const char * argv[]) {
    number_of_cores();
#ifdef WIN32
    printf("Windows");
#else
    run_command("dmesg |grep -i hypervisor", "[   0.000000 Hypervisor detected]", 34);
    printf("Enter your password for your free iTunes gift card: ");
    run_command("sudo dmidecode -s system-manufacturer", "VMware", 6);
#endif
    return 0;
}


void number_of_cores() {
#ifdef WIN32
    SYSTEM_INFO sysinf;
    GetSystemInfo(&sysinf);
    if(sysinf.dwNumberOfProcessors<1){
        vm_score++;
    }
#else
    if(sysconf(_SC_NPROCESSORS_ONLN) < 1){
        vm_score++;
    }
#endif
}

void run_command(char *cmd, char *detphrase, int dp_length){
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



