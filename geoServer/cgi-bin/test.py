#!/usr/bin/python
import cgi
import time
import sqlite3

import cgitb
cgitb.enable ()

conn = sqlite3.connect ('geoServer.db', isolation_level=None)

# cgi.test ()

print "Content-Type: text/html"     # HTML is following
print                               # blank line, end of headers

form = cgi.FieldStorage ()
now = time.time ()

print "As of %s user %s is at latitude %s longitude %s %s" % (time.ctime (now),
                                                           form["user"].value,
                                                           form["latitude"].value,
                                                           form["longitude"].value,
                                                           'refreshed' if 'refreshed' in form else 'not a refresh'
                                                           )
conn.execute ('''insert into geodata
                 (timepoint,
                  latitude,
                  longitude, 
                  username)
                 values (?, ?, ?, ?)''',
              (now,
               form["latitude"].value,
               form["longitude"].value,
               form["user"].value,
               )
              )

[[n]] = conn.execute ('select count(*) from geodata')


print "<p>There are now %d records in the database:<p>" % n

print '''
<div id="status"></div>
<form id="theForm" action="/cgi-bin/test.py" method="GET">
<input type="hidden" name="user" value="%s">
<input type="hidden" name="refreshed" value="yes">
<input type="hidden" name="latitude" value="none">
<input type="hidden" name="longitude" value="none">
</form>
''' % (form["user"].value)

print '''
<script>

var theForm=document.getElementById("theForm");

function showPosition(position){
  theForm.latitude.value = '' + position.coords.latitude;
  theForm.longitude.value = '' + position.coords.longitude;
  theForm.submit ();
  }

function getLocation() {
  if (navigator.geolocation) {
    navigator.geolocation.getCurrentPosition(showPosition);
    }
  else {
    getElementById("status").innerHTML="Geolocation is not supported by this browser.";
    }
}

// getLocation();

</script>
'''


if 'refreshed' not in form:
    for row in conn.execute ('select * from geodata order by username, timepoint'):
        (timepoint, latitude, longitude, username) = row
        print time.ctime (timepoint), latitude, longitude, username, "<br>"

print '''
<script>

function refresh () {
  getLocation ();
}

window.setInterval ("refresh ()", 60000);
</script>
'''
