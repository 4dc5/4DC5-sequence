/*
 * 4dc5 sequence generator
 *
 * 2006
 *
 * 4dc5.com
 *
 */

#include <stdio.h>
#include <stdlib.h>

/*
 * The node type stores a count of the occurences of the string up to this point in the tree.
 */
typedef struct node
  {
  int count;
  void* pbit0;
  void* pbit1;
  } node;

/*
 * pstartnode is the root of the tree.
 */
node* pstartnode;

/*
 * newnode() creates a new node and initialises it.
 */
node* newnode(void)
{
node* p;

  p = (node*)malloc(sizeof(node));
  p->count = 0;
  p->pbit0 = NULL;
  p->pbit1 = NULL;

  return p;
  
}

/*
 * getcounts() gets the number of occurrences of the supplied sequence suffixed with either a 0 or a 1.
 */
void getcounts(int* c0, int* c1, char* seq, int seqlen)
{
node* cur;

  // Assume the counts are zero
  *c0 = 0;
  *c1 = 0;

  cur = pstartnode;

  // Follow the tree down to the node representing the end of the sequence so far.
  int i;
  for (i=0;i<seqlen;i++)
    {
    if (seq[i] == 0 && cur != NULL) cur = cur->pbit0;
    if (seq[i] == 1 && cur != NULL) cur = cur->pbit1;
    }

  // Now have a look at the 0 and 1 branches from this point, and get the counts.
  if (cur != NULL)
    {
    if (cur->pbit0 != NULL) *c0 = ((node*)(cur->pbit0))->count;
    if (cur->pbit1 != NULL) *c1 = ((node*)(cur->pbit1))->count;
    }
  
}

/*
 * getnextbit() returns the next bit of the sequence, given the sequence so far.
 *
 * pos isn't part of the algorithm. It just lets us know how far we had to go in order to get an answer. Useful if
 * we want to know when we had to resort to the default bit.
 */
char getnextbit(char* seq,int seqlen,int* pos)
{
char ret;
int c0;
int c1;

  // Check that we still have some bits to use
  if (seq != NULL && seqlen >= 0)
    {
    // check the sequence counts with a 0 and a 1 suffixed
    getcounts(&c0,&c1,seq,seqlen);

    // If the sequence+0 has occurred more often than sequence+1, then use a 1 as the next bit.
    if (c0 > c1)
      {
      ret = 1;
      *pos = seqlen;
      }
    // And vice-versa.
    if (c1 > c0)
      {
      ret = 0;
      *pos = seqlen;
      }
    // The counts match, so we can't decide. Try again without the leftmost bit.
    if (c0 == c1)
      {
      ret = getnextbit(&(seq[1]), seqlen-1, pos);
      }
    }
  else
    {
    // Can't decide, so we have to return the default bit
    ret = 0;
    *pos = -1;
    }
  return ret;
}

/*
 * updatecounts() updates the tree with the counts of subsequences.
 */
void updatecounts(char* seq, int seqlen)
{
node* cur;

/*
 * Start at the top of the tree, and follow it all the way down for each bit in the sequence, creating new nodes if
 * necessary.
 */
  cur = pstartnode;

  int i;
  for (i=0;i<seqlen;i++)
    {
    if (seq[i] == 0)
      {
      if (cur->pbit0 == NULL)
        {
        cur->pbit0 = newnode();
        }
      cur = cur->pbit0;
      }
    if (seq[i] == 1)
      {
      if (cur->pbit1 == NULL)
        {
        cur->pbit1 = newnode();
        }
      cur = cur->pbit1;
      }
    }

  // Increment the count for this subsequence.
  cur->count++;

/*
 * If this subsequence is more than 1 bit long, update the count of the next subsequence i.e. stripping off the leftmost
 * bit.
 */
  if (seqlen > 1) updatecounts(&(seq[1]), seqlen-1);
}

/*
 * addbit() adds a bit to the end of the sequence, updating the counts too.
 */
void addbit(char nextbit, char* seq, int* seqlen)
{
  seq[*seqlen] = nextbit;
  (*seqlen)++;

  updatecounts(seq,*seqlen);
}

int main(int argc, char* argv[])
{
char* seq;    // The bit sequence
int seqlen;   // The sequence length
char nextbit; // The next bit in the sequence
int maxspace; // Max space allocated to the sequence
int pos;      // How far down the subsequences we had to go to get a decisive answer
char nybble;    // Conversion of binary to hex

  // Allocate space for the sequence
  maxspace = 4096;
  seq = malloc(maxspace);

  seqlen = 0;

  // Instantiate the root node
  pstartnode = newnode();
  
  nybble = 0;

  while (1)
    {
    // Get the next bit in the sequence
    nextbit = getnextbit(seq,seqlen,&pos);

    // Allocate more space for the sequence if necessary
    if (seqlen > maxspace)
      {
      maxspace = maxspace*2;
      seq = realloc(seq,maxspace);
      }
    
    // Add the nextbit to the sequence
    addbit(nextbit,seq,&seqlen);

    // Add the nextbit to the cumulative nybble
    nybble = (nybble << 1) + nextbit;
    
    // Every 4 bits, print the nybble and reset it
    if (seqlen%4 == 0)
      {
      printf("%X",nybble);
      if (seqlen%8 == 0) printf(" "); // Space every 2 nybbles
      if (seqlen%128 == 0) printf("\n"); // Newline every 16 bytes
      fflush(stdout);
      nybble = 0;
      }
    }
}
