/** 
 *   \file fastq.c Module to handle FASTQ sequences
 *   \author Andrea Sboner (andrea.sboner@yale.edu)
 */


#include "log.h"
#include "format.h"
#include "linestream.h"
#include "stringUtil.h"
#include "common.h"
#include "fastq.h"



#define NUM_CHARACTRS_PER_LINE 60


static LineStream lsFastq = NULL;



/**
 * Initialize the FASTQ module using a file name.
 * @note Use "-" to denote stdin.
 * @post fastq_nextSequence(), fastq_readAllSequences() can be called.
 */
void fastq_initFromFile (char* fileName) 
{
  lsFastq = ls_createFromFile (fileName);
  ls_bufferSet (lsFastq,1);
}



/**
 * Deinitialize the FASTQ module. Frees module internal memory.
 */
void fastq_deInit (void) 
{
  ls_destroy (lsFastq);
}



/**
 * Initialize the FASTQ module using a pipe.
 * @post fastq_nextSequence(), fastq_readAllSequences() can be called.
 */
void fastq_initFromPipe (char* command)
{
  lsFastq = ls_createFromPipe (command);
  ls_bufferSet (lsFastq,1);
}



static void fastq_freeFastq (Fastq* currFQ)
{
  if (currFQ == NULL) {
    return;
  }
  Seq* currSeq = currFQ->seq;
  hlr_free (currSeq->name);
  hlr_free (currSeq->sequence);
  freeMem (currSeq);
  currSeq = NULL;
  hlr_free (currFQ->quality);
  freeMem ( currFQ );
}



static Fastq* fastq_processNextSequence (int freeMemory, int truncateName)
{
  char *line;
  static Fastq* currFQ = NULL;
  int count;
  Seq* currSeq = NULL;

  if (ls_isEof (lsFastq)) {
    if (freeMemory) {
      fastq_freeFastq (currFQ);
    }
    return NULL;
  }
  count = 0;
  while ( (line=ls_nextLine (lsFastq)) && (count<4) ) {
    if (line[0] == '\0') {
      continue;
    }
    if (line[0] == '@') {      
      if (freeMemory) {
	fastq_freeFastq (currFQ);
      }
      count++;
      AllocVar (currFQ);
      AllocVar (currFQ->seq);
      currSeq = currFQ->seq;
      currSeq->name = hlr_strdup (line + 1);
      if (truncateName) {
	currSeq->name = firstWordInLine (skipLeadingSpaces (currSeq->name));
      }
      line = ls_nextLine (lsFastq); // reading sequence
      currSeq->sequence = hlr_strdup ( line );
      currSeq->size = strlen (currSeq->sequence);
      count++;
      line = ls_nextLine (lsFastq); // reading quality ID
      if( line[0] != '+' )
	die("Expected quality ID: '+' or '+%s'", currSeq->name );
      count++;
      line = ls_nextLine (lsFastq); // reading quality
      currFQ->quality = hlr_strdup( line );
      count++;
    } 
  }   
  ls_back (lsFastq,1);
  return currFQ;
}



/**
 * Returns a pointer to the next FASTQ sequence.
 * @param[in] truncateName If truncateName > 0, leading spaces of the name are skipped. Furthermore, the name is truncated after the first white space. If truncateName == 0, the name is stored as is.
 * @note The memory belongs to this routine.
 */
Fastq* fastq_nextSequence (int truncateName) 
{
  return fastq_processNextSequence (1,truncateName);
}



/**
 * Returns an Array of FASTQ sequences.
 * @param[in] truncateName If truncateName > 0, leading spaces of the name are skipped. Furthermore, the name is truncated after the first white space. If truncateName == 0, the name is stored as is.
 * @note The memory belongs to this routine.
 */
Array fastq_readAllSequences (int truncateName)
{
  Array seqs;
  Fastq *currFQ;
  seqs = arrayCreate (100000,Fastq);
  while (currFQ = fastq_processNextSequence (0,truncateName)) {
    array (seqs,arrayMax (seqs),Fastq) = *currFQ;
  }
  return seqs;
}



/**
 * Prints currSeq to char*.
 */
char* fastq_printOneSequence (Fastq* currFQ) 
{
  static Stringa buffer=NULL;
  stringCreateClear( buffer, 100 );
  stringPrintf( buffer, "@%s\n%s\n+\n%s", currFQ->seq->name, currFQ->seq->sequence, currFQ->quality );
  return string( buffer );
}



/**
 * Prints seqs to stdout.
 */
void fastq_printSequences (Array seqs)
{
  int i;
  Fastq *currFQ;
  
  for (i = 0; i < arrayMax (seqs); i++) {
    currFQ = arrp (seqs,i,Fastq);
    fastq_printOneSequence (currFQ); 
  }
}
