# EvilNamedPipe
## First Commit
1.If you choose to download the source code, you need to compile the program yourself

2.hold Service.exe And Server.exe Put it in the same level directory, and then run with administrator rights Server.exe . right will be raised later

3.It has been tested under win7 and can be elevated

4.Technical details:Create a named pipe with the administrator's permission, and then connect the service to the named pipe by creating the system privilege service. Then we obtain the token of system permission by simulating the client, and then copy it. Then we create a new process (such as CMD) with the copied token. The new process permission is system permission.
5.![Image text](http://www.const27.com/wp-content/uploads/2020/11/image-44-1024x398.png)
