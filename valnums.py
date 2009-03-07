"""
Perform verification of numbers
"""

from geonums import *
import time

f = open("coolnums.dat", 'r')
intsDepth = int(f.readline())
intsSum = int(f.readline())
currentSquare = int(f.readline())
currentOddNum = int(f.readline())
squaresSum = int(f.readline())
squaresDepth = int(f.readline())
f.close(); del f

allCorrect = True

#print "Checking triangular number... "
if triangleNum(intsDepth) != intsSum:
#    print "Triangular number is correct!"
#else:
    print "Triangular number test failled."
    print triangleNum(intsDepth), intsSum
    allCorrect = False

#print "Checking square pyramidal number... "
if squarePyrNum(squaresDepth) != squaresSum:
#    print "Square pyramidal number is correct!"
#else:
    print "Square pyramidal number test failled."
    print squarePyrNum(squaresDepth), squaresSum
    allCorrect = False

#print "Checking base of square pyramidal number... "
if squaresDepth**2 != currentSquare:
#    print "Base of square pyramidal number is correct!"
#else:
    print "Base of square pyramidal number text failled."
    print squaresDepth**2, currentSquare
    allCorrect = False

#print "Checking current odd number... "
if getOddNum(currentSquare) + 2 != currentOddNum:
#    print "Current odd number is correct!"
#else:
    print "Current odd number test failled."
    print n(getOddNum(currentSquare)), currentOddNum
    allCorrect = False

if allCorrect == True:
    print "All 4 tests passed."
    time.sleep(1)
else:
    time.sleep(10)
#Clean up
del intsDepth, intsSum, currentSquare, currentOddNum, squaresSum, squaresDepth
