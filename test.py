import reedsolomon
import random
while True:
    tMesg = ""
    for i in xrange(30):
        tMesg+=random.choice('1234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ ')
    tSize = 6
    tdata=[ord(one) for one in tMesg]
    result1= reedsolomon.RSEncode(tdata)
    print result1
    result=result1[:]
    errorcount=random.randint(1,3)
    errseted=set()
    for i in xrange(errorcount):
        while True:
            pos=random.randint(0,len(result)-1)
            if pos not in errseted:
                errseted.add(pos)
                break
        print pos,',',
        result[pos]=0
    print ''
    print result
    result2= reedsolomon.RSDecode(result)
    print result2
    for i in xrange(len(result2)):
        if result2[i]!=result1[i]:
            print 'error'
            exit()