import platform
import commands
import os
import subprocess

score = 0

'''
Determine the operating system the program is running on
'''
def determine_os():
    if platform.system() ==  "Linux":
        #we know the platform is linux, test linux commands
        linux_dmesg()
        linux_dmidecode()
    elif platform.system() == "Windows":
        #we know the platform is win, test win commands
        windows_sysinfo()
    else:               
        #the platform is something else, abort program
        print "OS not supported. Sorry"
        quit()

'''
Run the Ubuntu dmesg command to check for hypervisor
'''
def linux_dmesg():
    dmesg = commands.getstatusoutput("sudo dmesg |grep -i hypervisor")
    if(dmesg[0]==0):
        if(dmesg[1][:34] == "[    0.000000] Hypervisor detected"):
            score_increment() #add to score, as hypervisor was detected
        else:
            pass #no hypervisor detected, may not be vm
    else:
        pass

'''
Run the Linux dmidecode to check system-manufacturer or product name
'''
def linux_dmidecode():
    dmidecode_man = commands.getstatusoutput("sudo dmidecode -s system-manufacturer")
    if(dmidecode_man[0]==0):
        if(dmidecode_man[1]=="VMware" or dmidecode_man[1]=="Xen"):
            score_increment()
        else:
            pass
    else:
        pass

    dmidecode_prod = commands.getstatusoutput("dmidecode -s system-product-name")
    if(dmidecode_prod[0]==0):
        if(dmidecode_prod[1]=="KVM" or dmidecode_prod[1]=="VirtualBox"):
            score_increment()
        else:
            pass
    else:
        pass
    
'''
Run the Windows sysinfo command to check hardware naming
'''
def windows_sysinfo():
    sysinfo = subprocess.Popen(["systeminfo"])
    print sysinfo
    os.system("pause")

def score_increment():
    global score
    score += 1

determine_os()
print score
