import platform

if platform.system() ==  "Linux":
    print "Ubuntu"
elif platform.system() == "Windows":
    print "Windows"
else:
    #abort                                                                      
    print "Mac"


