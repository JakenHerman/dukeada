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
void linux_dmesg();

int main(int argc, const char * argv[]) {
    number_of_cores();
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

void linux_dmesg(){
    #define BUFSIZE 128
    char *cmd = "dmesg |grep -i hypervisor";
    char buf[BUFSIZE];
    FILE *fp;
    
    if((fp = popen(cmd, "r")) == NULL){
        printf("Error");
    }
    
    if(fgets(buf, BUFSIZE, fp) != NULL){
        char *detphrase = "[   0.000000] Hypervisor detected]";
        char detection[35]; //one extra character for null terminator
        strncpy(detection, detphrase, 34);
        detection[34] = '\0'; //place null terminator
        
        if(strcmp(detphrase, detection) == 0){
            vm_score++;
        }
    }
    if(pclose(fp)){
        printf("Command not found or exited with error status \n");
    }
}



