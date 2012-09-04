#!/usr/bin/python3
import os
import sys

def main():
    print("unit_test.py:compiling unit test...")
    target_name = sys.argv[1].split('.')[0]
    command_list = ["touch {target}.cpp", "make -j4 -f {target}.mk"]    
    is_compile_success = os.system("&&".join(command_list).format(target = target_name))
    if(is_compile_success!=0):
        return 1
    else:
        os.system("chmod +x {target}.out".format(target = target_name))
        print("unit_test.py:result is...")
        return_code = os.system("strace -o {target}.log -F ./{target}.out".format(target = target_name))
        print("unit_test.py:return_code is {code}.".format(code = return_code))
        if(return_code == 11):
            print("unit_test.py:segumentation fault?")
        #os.system("rm unit_test.out")
        
        return 0

if __name__ == "__main__":
    main()
