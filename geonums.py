"""
Contains a list of functions which assist in
my current number theory works.
"""

from math import sqrt

def triangleNum(base):
    """
    This function gets the triangular number with the given base.
    """
    n = base
    return (n**2 + n)/2
def triNumBase(sum):
    """
    This function gets the base of the given triangular number.
    """
    return sqrt(2*sum + 1/4) - 1/2
def squarePyrNum(base):
    """
    This function gets the square pyramidal number with the given base.
    """
    n = base
    return (2*n**3 + 3*n**2 + n)/6
def squarePyrBase(sum):
    """
    This function gets the base of the given square pyramidal number.
    """
    s = sum
    return (sqrt(3888*s**2 - 1)/(24*sqrt(3)) + 3*s/2)**(1/3) + 1/(12*(sqrt(3888*s**2 - 1)/(24*sqrt(3)) + 3*s/2)**(1/3)) - 1/2
def getOddNum(square):
    """
    Try to find the last odd number added to create
    the square number
    """
    return sqrt(square)*2-1
