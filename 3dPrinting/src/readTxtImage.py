from sys import stdin, stdout, stderr
import re

# find all the numbers in a line of text and return the first N
def findNumbers (line, n):
    return map (int, re.findall (r'\d+', line)[0:n])

# write out an STL facet for a triangle
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

# read in an image in TXT format

# get the width and height from the header line
(width, height) = findNumbers (stdin.readline(), 2)
fw = float (width)
fh = float (height)

# make a matrix of Z values, initially 0
zs = [[0 for y in range (height)]
      for x in range (width)]

print >> stderr, width, 'x', height
print >> stderr, 'reading...'

# read each pixel coordinate and value
for line in stdin:
    (x, y, z) = findNumbers (line, 3)
    # store the scaled Z value in the matrix
    zs[x][height - 1 - y] = z / 256.0

print >> stderr, ('writing...')

# write the STL file
stdout.write ('solid name\n')

# interpret the pixel coordinates as grid points
# and write out facets representing quads
# scale the pixel coordinates to [0,1]
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

