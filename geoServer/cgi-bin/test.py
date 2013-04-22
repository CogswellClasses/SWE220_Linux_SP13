#!/usr/bin/python
import cgi
import time
import sqlite3

import cgitb
cgitb.enable ()

conn = sqlite3.connect ('geoServer.db', isolation_level=None)

#cgi.test ()

print "Content-Type: text/html"     # HTML is following
print                               # blank line, end of headers

form = cgi.FieldStorage ()
now = time.time ()

print "As of %s user %s is at latitude %s longitude %s" % (time.ctime (now),
                                                           form["user"].value,
                                                           form["latitude"].value,
                                                           form["longitude"].value
                                                           )
conn.execute ('''insert into geodata
                 (timepoint,
                  latitude,
                  longitude, 
                  username)
                 values (?, ?, ?, ?)''',
              (now,
               form["user"].value,
               form["longitude"].value,
               form["latitude"].value)
              )
[[n]] = conn.execute ('select count(*) from geodata')


print "<p>There are now %d records in the database:<p>" % n

for row in conn.execute ('select * from geodata order by username, timepoint'):
    (timepoint, latitude, longitude, username) = row
    print time.ctime (timepoint), latitude, longitude, username, "<br>"


