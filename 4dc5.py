# 4dc5 Sequence generator
# 2004
#
# 4dc5.com

import sys
 
def inc_count(n):
    if n in counts:
        counts[n] += 1
    else:
        counts[n] = 1

# update_counts() runs through the sequence taking the rightmost n bits as n -> 1, and
# updates the count table for each sub-sequence it finds.
def update_counts():
    pos = 0
    while not sequence[pos:] == '':
        inc_count(sequence[pos:])
        pos += 1

# sortfunc() is only used for displaying the counts at the end. It doesn't
# have anything to do with generating the sequence.
def sortfunc(a,b):
    if len(a) < len(b):
        return -1
    elif len(b) < len(a):
        return 1
    else:
        return cmp(a,b)

# counts is a dictionary that uses the sub-sequence as a key, and the occurrence count
# as the value. We are using a dictionary rather than a list because the table is going
# to be extremely sparse.
counts={}
sequence=''
print
x = 0
while x<1024: # Change this value to get more or fewer bits.
    # Run through the sequence, starting with the whole thing, and lopping off the left-most
    # bit each time until we find a winner for the next bit.
    bit = ''
    pos = 0
    # If bit is empty, we haven't found a candidate for the next bit yet.
    while (bit == '') and (pos <= len(sequence)):
        # Check whether we should use a 0 or 1 as the next bit.
        c0 = counts.get(sequence[pos:] + '0',0)
        c1 = counts.get(sequence[pos:] + '1',0)
        if c0 < c1:
            bit = '0'
        elif c0 > c1:
            bit = '1'
        pos += 1
    if bit == '':
        # Couldn't decide based on the counts, so use the default bit.
        bit = '0'
    sequence += bit
    sys.stdout.write(bit)
    sys.stdout.flush()
    update_counts()
    x+=1
 
print

# Display counts of each sub-sequence.
keys=counts.keys()
keys.sort(sortfunc)
for key in keys:
    if counts[key] > 1: # Anything 1 or less is boring.
        print key, counts[key]

