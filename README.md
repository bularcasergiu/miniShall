# miniShall
Linux shall using posix

#prereqiesits:
run command "sudo apt-get install libreadline-dev"

#build using: 
gcc -o sebuShell sebuShell.c -lreadline

#inspired from:

[here](https://www.geeksforgeeks.org/making-linux-shell-c/) and [here](https://www.unix.com/programming/268879-c-unix-how-redirect-stdout-file-c-code.html) 

#how works:

run "./sebuShall"
call your function, if you tipe "> yourfileName" the otput will be redirected to that file. 
