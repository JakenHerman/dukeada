//
//  main.c
//  guacamole
//
//  Created by Jaken Herman on 7/13/16.
//  Copyright © 2016 jaken herman. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

int vm_score = 0; //if "vm_score" reaches 5, we assume virtual
void number_of_cores();

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



