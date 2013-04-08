all:	programMain simpleServer simpleClient listen talk chatServer

programMain: programMain.c
	grep '<p>' Edition\ 4\ \(1996\).html | grep -v helvetica | grep -v href | grep -v English>"Read me"
	sed -i 's/<p>/"/g' "Read me"
	sed -i 's!</p>!",!g' "Read me"

	gcc -o programMain programMain.c	

simpleServer:	simple-server.c saying.c saying.h saying.c chatPorts.h
	gcc -o simpleServer simple-server.c saying.c

simpleClient:	simple-client.c chatPorts.h
	gcc -o simpleClient simple-client.c

talk:	talk.c chatPorts.h
	gcc -o talk talk.c

listen:	listen.c chatPorts.h
	gcc -o listen listen.c

chatServer:	chatServer.c chatPorts.h
	gcc -o chatServer chatServer.c

clean:
	rm simpleClient simpleServer programMain talk listen chatServer


