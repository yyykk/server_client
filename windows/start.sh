gcc client.c -o client -pthread
gcc server_thread.c -o server_thread -pthread
gnome-terminal -x bash -c "./server_thread 1234"
gnome-terminal -x bash -c "./client 127.0.0.1 1234"
