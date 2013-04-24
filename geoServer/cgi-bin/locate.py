#!/usr/bin/python
import cgi
import time
import sqlite3

import cgitb
cgitb.enable ()

conn = sqlite3.connect ('geoServer.db', isolation_level=None)

# cgi.test ()

print "Content-Type: text/plain"
print 

for row in conn.execute ('select username, max(timepoint), latitude, longitude from geodata group by username'):
        (username, timepoint, latitude, longitude) = row
        print '\t'.join (map (str, [time.strftime  ("%c",
						    time.localtime (timepoint)),
				    latitude,
				    longitude,
				    username]))
