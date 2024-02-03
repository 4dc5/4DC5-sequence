# 4DC5-sequence

There are two versions of the 4DC5 sequence generator. One is in Python (and is a bit easier to understand), and the other in C (which is much faster).

The sequence generated is pseudo-random, though its mathematical "randomness" has not been established.

As each bit is generated, the algorithm attempts to balance the *longest* strings first. If the longest length doesn't give an unambiguous answer, then the string that is one bit shorter is tried, and so on until an unambiguous answer can be obtained. If no unambiguous answer can be obtained, then a 0 is used. (In practice, this happens three times in the first few bits, and then hasn't happened again in any run that has been tried so far.)

Example:

Start with 0 (as we have no data to make a choice at this point).

Now we have one 0 and no 1s.

For the next bit, to balance the counts we need to choose a 1.

So the sequence is 01.

We now have one 0 and one 1. We also have no 00s, one 01, no 10s, and no 11s.

Adding a 0 would give us an extra 10, while adding a 1 would give us an extra 11. But as both 10 and 11 counts are at zero right now, this doesn't help. So we look at the one-bit strings.

The 0 and 1 counts are both equal, so that doesn't help either. So we resort to using the default of 0.

So the sequence is now 010.

We have two 0s, one 1.
We have no 00s, one 01, one 10, and no 11s.
We have one 010, and no other three-bit strings.

For the next bit, look at the three-bit counts. That doesn't help, as adding a 0 or 1 doesn't enable us to balance up the counts of this length.

So look at the two-bit counts. Adding a 0 would raise the 00 count to one. Adding a 1 would raise the 01 count to two. So in the interest of balancing the string counts, we choose a 0.

The sequence is now 0100. (This is the 4 at the start of 4D C5.)

The algorithm continues on. For each new bit the longest string counts are examined first.

The first few bytes are: 4D C5 0F 59 18 25 F3 B4 FD E1 6C 72 A4 CD 5D 10 1B 7F 1A 0E A3 E9 4C 4E 05 6B D9 E4 84 5C CB 0C 9E EE 41 79 B2 8A AC 59 84 D2 24 AF CB B0 08 6B 75 C3 8F 11 DF 6D 4E 9B 46 F4 29 68 63 39 F0 61 FF 54 0F 3D 39 37 C5 FE 63 F0 DD B9 C4 A0 8D 42 47 B5 83 E4 53 C0 58 D8 54 4C 0C CF EC 42 EA DB 37 94 81 3E E0 A3 2B 3A D2 E3 15 CB 7B F4 70 83 4C 87 64 B2 F5 2A F1 D0 07 34 5A A9 AE F5 F5 D6...

This algorithm is interesting, but probably not useful as a pseudo-random number generator in practice as it requires ever-increasing amounts of memory for each new bit generated.


Programs:

The code is trivial to run, with no dependencies.

Compile the C code with gcc 4dc5.c.

Run the Python code with python 4dc5.py.

UPDATE 2024-02-03:

It's just been pointed out to me that there is now an entry in OEIS for this sequence:

https://oeis.org/A330731 - by Aresh Pourkavoos
