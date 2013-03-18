all:	programMain simpleServer simpleClient

programMain: programMain.c
	grep '<p>' Edition\ 4\ \(1996\).html | grep -v helvetica | grep -v href | grep -v English>"Read me"
	sed -i 's/<p>/"/g' "Read me"
	sed -i 's!</p>!",!g' "Read me"

	gcc -o programMain programMain.c	

simpleServer:	simple-server.c
	gcc -o simpleServer simple-server.c

simpleClient:	simple-client.c
	gcc -o simpleClient simple-client.c
