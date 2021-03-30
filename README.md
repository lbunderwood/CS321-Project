# CS321-Project
A simple direct-messaging project to learn how to use network sockets in C++ for CS321 Operating Systems at UAF.

This software includes two programs, named Client and Server, which act as their names suggest to provide a service that allows two users running the client program to send each other text messages. Each client is required to provide the ip of the server, an alias to use as a username, and the alias of the person they'd like to correspond with. This enables any users of this service to set up their own local server and to have as many pairs of people correspond using it as they like.

Using this service:
Run the elf executable "Server" found in CS321-Project/Server/cmake-build-debug and note the ipv4 address of the machine it is running on. Then, run "Client" found in CS321-Project/Client/cmake-build-debug and enter the server's ip address, your username, and the username of the person you'd like to communicate with. If the other person successfully does the same, a connection will be established by the server, and you will be able to send messages by entering them at the prompt and pressing enter. Note that due to the constraints of the command line interface, you must occasionally hit enter to refresh the program output, as it must first obtain your input before continuing the program.

NOTE: It is best to be careful with the usernames provided to the program, as multiple users attempting to use the same name is untested and may produce unintended results. Additionally, it is important to note that the information sent using this service is not encrypted or encapsulated in any way, so it would be very easy for any untrustworthy people on your network to steal any information sent.
