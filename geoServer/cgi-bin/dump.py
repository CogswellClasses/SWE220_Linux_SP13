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

for row in conn.execute ('select * from geodata order by username, timepoint'):
        (timepoint, latitude, longitude, username) = row
        print '\t'.join (map (str, [timepoint, latitude, longitude, username]))
