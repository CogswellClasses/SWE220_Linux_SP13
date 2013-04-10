from sys import stdin, stdout, stderr
import re

def findNumbers (line, n):
    return map (int, re.findall (r'\d+', line)[0:n])

def facet (p1, p2, p3):
    (p1x, p1y, p1z) = p1
    (p2x, p2y, p2z) = p2
    (p3x, p3y, p3z) = p3
    stdout.write ("""
facet normal
outer loop
vertex %f %f %f
vertex %f %f %f
vertex %f %f %f
endloop
endfacet
""" % (p1x, p1y, p1z,
       p2x, p2y, p2z,
       p3x, p3y, p3z,
       ))

(width, height) = findNumbers (stdin.readline(), 2)
fw = float (width)
fh = float (height)
zs = [[0 for y in range (height)]
      for x in range (width)]
print >> stderr, width, 'x', height

print >> stderr, 'reading...'
for line in stdin:
    (x, y, z) = findNumbers (line, 3)
    zs[x][height - 1 - y] = z / 256.0

print >> stderr, ('writing...')

stdout.write ('solid name\n')

for x in range (1, width):
    for y in range (1, height):
        (p1, p2, p3, p4) = [((x-1)/fw, (y-1)/fh, zs[x-1][y-1]),
                            (x/fw,     (y-1)/fh, zs[x][y-1]),
                            (x/fw,     y/fh,     zs[x][y]),
                            ((x-1)/fw, y/fh,     zs[x-1][y]),
                            ]
        facet (p1, p2, p3)
        facet (p3, p4, p1)

stdout.write ('endsolid name\n')

