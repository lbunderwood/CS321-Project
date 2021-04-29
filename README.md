# CS321-Project
### General Information:
A simple direct-messaging project to learn how to use network sockets in C++ for CS321 Operating Systems at UAF.

This software includes two programs, named Client and Server, which act as their names suggest to provide a service that allows two users running the client program to send each other text messages. Each client is required to provide the ip of the server, an alias to use as a username, and the alias of the person they'd like to correspond with. This enables any users of this service to set up their own local server and to have as many pairs of people correspond using it as they like.

### Using This Service:
Run the elf executable "Server" found in CS321-Project/Server/cmake-build-debug and note the ipv4 address of the machine it is running on. Then, run "Client" found in CS321-Project/Client/cmake-build-debug and enter the server's ip address, your username, and the username of the person you'd like to communicate with. If the other person successfully does the same, a connection will be established by the server, and you will be able to send messages by entering them at the prompt and pressing enter. Note that due to the constraints of the command line interface, you must occasionally hit enter to refresh the program output, as it must first obtain your input before continuing the program.

NOTE: It is best to be careful with the usernames provided to the program, as multiple users attempting to use the same name is untested and may produce unintended results. Additionally, it is important to note that the information sent using this service is not encrypted or encapsulated in any way, so it would be very easy for any untrustworthy people on your network to steal any information sent. Finally, there is a known bug that I have been unable to work out, which requires both users to connect to the server before either one sends their usernames. When one person connects and sends both names before the other user connects, they are inexplicably not placed together in a chat. I cannot, for the life of me, figure out why.

### Design Process:
I have chosen to create a direct-messaging service for a number of reasons. Firstly, I needed something that brought me into close contact with the operating system, which is easily fulfilled by this project due to the number of syscalls made in order to access the network. Secondly, I wanted to learn to use network sockets because I had never witten a program that utilized a network of any kind, and I thought it would be achievable, challenging, and informative. Thirdly, direct-messaging is integral to the history of the internet as well as our everyday lives, and is about as simple as a network application can get, so it seemed a reasonable first step.

I have chosen to structure the project in a traditional client-server model, rather than peer-to-peer, mostly because it seemed far simpler. Also, in the lib directory, you will find that I have created a class Connection, which makes the use of the network far more friendly to the C++ programmer by abstracting away all of the typical C-style function calls and reducing their operation to a simple, object-oriented interface. This class has performed admirably, and I have finally managed to work out the bugs in closing connections. Additionally, I have created a class Chat for the Server program which holds two Connections to represent two clients who have been connected, as well as the usernames that they selected, and manages the interaction between these clients. I wish I had been able to create a simpler public interface for Chat, but I had many small peices of functionality that I had to tack on in order to give Server's main function all the functionality it needs.

### Conclusions:
One of the biggest strengths of the approach that I took was the OOP concept of RAII. Since all of the sockets are owned by the Connection class, I was able to just slap a close() in the destructor and not worry about the lights being left on so to speak. I am also incredibly pleased with the Connection interface in general, even though some of the details are ugly behind the scenes, the class provides a clean, OOP interface to the client code that is incredibly satisfying. One of the primary things I would change if I had this project to do over again would be to use FLTK or some other simple graphics library. This is because a key limitation of the command line (at least as I know how to use it - I know there are ways around this) is that you cannot print output while receiving input, since getline hangs the program. The other thing I ran into was that one of the main loops in my server program ended up being O(n^2) - if you dig in my code and look at the comments you'll see why, which makes it extremely vulnerable to denial of service attacks. Perhaps this is a good reason to spin up a few threads to help handle all of the different things going on. My key takeaway from this project is how to use UNIX network sockets, and how UNIX systems handle them. Between Beej's Guide to Network Programming and reading some of the Linux Man pages, I got a pretty well-rounded view of Unix-style network programming, and I'm confident that I can now build more complicated applications that use the network in future.