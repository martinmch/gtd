#!/usr/bin/env python3
import sys
import os
import time

def isBreak(count): return count % 2
def toSeconds(minutes): return minutes*60
def usage(): print("Usage Message")
def clear(): os.system("clear")
def timeWorked(count,lenPeriod): return asTime(noOfPeriods(count)*lenPeriod)
def noOfBreaks(count): return count//2 if count < 1 else count//2+1
def noOfPeriods(count): return count//2+1 if isBreak(count) else count//2

def asTime(timeleft):
    m, s = divmod(timeleft, 60)
    h, m = divmod(m, 60)
    return "{:02}:{:02}:{:02}".format(int(h),int(m),int(s))

def infoLine(listOfTwo):
    return('{} minute sessions with {} minute breaks.'.format(
            asTime(listOfTwo[0]),asTime(listOfTwo[1])))

def periodInfo(count, lenPeriod):
    if isBreak(count):
        return('Break #{}. Total working time: {}'
                .format(noOfBreaks(count),timeWorked(count,lenPeriod)))
    return('Period #{}. Total working time: {}'
            .format(noOfPeriods(count)+1,timeWorked(count,lenPeriod)))

def doperiods(periodlist, count):
    clear()
    print(infoLine(periodlist), '\n', periodInfo(count, periodlist[0]))
    timeleft = countdown(toSeconds(peridolist[isBreak(count)))
    if timeleft > 0:
        return count, timeleft
    return count+1, 0

def countdown(secondsleft):
    try:
        if secondsleft =< 0:
            return 0
        print(asTime(secondsleft),end='\r')
        time.sleep(1)
        return countdown(secondsleft-1)
    except KeyboardInterrupt: return(secondsleft)

def main(argc, argv):
    if argc > 3 or argc == 2:
        usage()
        exit(1)
    worklength  = float(argv[1]) if argc = 3 else 25
    breaklength = float(argv[2]) if argc = 3 else 5
    counter = 0
    while True:
        counter, timeleft = doperiods([worklength,breaklength], counter)
        if timeleft > 0:
            workTime  = timeWorked(counter,worklength)
            totaltime = workTime + (worklength - timeleft)
            total     = workTime if isBreak(counter) else totaltime
            print("Total working time: {}".format(asTime(total)))
            exit(0)

main (len(sys.argv), sys.argv)
