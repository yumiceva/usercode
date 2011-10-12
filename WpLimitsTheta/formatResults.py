#! /usr/bin/env python

#from ROOT import *

import sys,os, math


infile = open(sys.argv[1])

medianExp = {}
OneSigmaUp = {}
OneSigmaDown = {}
TwoSigmaUp = {}
TwoSigmaDown = {}
medianObs = {}

for line in infile:

    if line.find("medianExpected")!=-1:

        list = line.split()
        OP = int(list[1])
        value = list[2]
        medianExp[OP] = value

    if line.find("medianObserved")!=-1:

        list = line.split()
        OP = int(list[1])
        value =list[2]
        medianObs[OP] =value

    if line.find("OneSigmaUp")!=-1:

        list = line.split()
        OP = int(list[1])
        value =list[2]
        OneSigmaUp[OP] =value

    if line.find("OneSigmaDown")!=-1:

        list = line.split()
        OP = int(list[1])
        value =list[2]
        OneSigmaDown[OP] =value

    if line.find("TwoSigmaUp")!=-1:

        list = line.split()
        OP = int(list[1])
        value =list[2]
        TwoSigmaUp[OP] =value

    if line.find("TwoSigmaDown")!=-1:

        list = line.split()
        OP = int(list[1])
        value =list[2]
        TwoSigmaDown[OP] =value


masses = medianExp.keys()
masses.sort()
print masses
#masses.reverse()
#print masses

aline1 = "float y["+str(2*len(masses))+"] = {"
aline2 = "float y2["+str(2*len(masses))+"] = {"
alinex = "float x_expec["+str(2*len(masses))+"] = {"

alineExp = "float y_expec["+str(len(masses))+"] = {"
alineObs = "float y_obs["+str(len(masses))+"] = {"

for m in masses:

    alinex += str(m) + ", "
    aline1 += OneSigmaDown[m] +", "
    aline2 += TwoSigmaDown[m] +", "
    alineExp += medianExp[m] +", "
    alineObs += medianObs[m] +", "

masses.reverse()

for m in masses:

    alinex += str(m) + ", "
    aline1 += OneSigmaUp[m] +", "
    aline2 += TwoSigmaUp[m] +", "

aline1 += "};"
aline2 += "};"


print aline1
print aline2
print alinex

print alineExp
print alineObs

print "done"
