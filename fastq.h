/** 
 *   \file fastq.h
 *   \author Andrea Sboner (andrea.sboner@yale.edu)
 */


#ifndef DEF_FASTQ_H
#define DEF_FASTQ_H


#include "seq.h"

/**
 * FASTQ structure.
 */
typedef struct {
  Seq* seq;
  char* quality;
} Fastq;


extern void fastq_initFromFile (char *fileName);
extern void fastq_initFromPipe (char *command);
extern void fastq_deInit (void);
extern Fastq* fastq_nextSequence (int truncateName);
extern Array fastq_readAllSequences (int truncateName);
extern char* fastq_printOneSequence (Fastq *currFQ);
extern void fastq_printSequences (Array seqs);


#endif
