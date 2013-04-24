#!/usr/bin/python
import cgi
import time
import sqlite3

import cgitb
cgitb.enable ()

conn = sqlite3.connect ('geoServer.db', isolation_level=None)

# cgi.test ()

print "Content-Type: text/html"
print 

for row in conn.execute ('select username, max(timepoint), latitude, longitude from geodata group by username'):
        (username, timepoint, latitude, longitude) = row

	userstring='<a href="https://maps.googleapis.com/maps/api/staticmap?markers=%s,%s&size=1000x1000&sensor=false">%s</a><br>' % (latitude, longitude, username)
        print '\t'.join (map (str, [time.strftime  ("%c",
						    time.localtime (timepoint)),
				    latitude,
				    longitude,
				    userstring]))
