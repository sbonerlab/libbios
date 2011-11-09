#ifndef DEF_BED_PARSER_H
#define DEF_BED_PARSER_H

/**
 *    \file bedParser.h
 *    \author Andrea Sboner (ans2077@med.cornell.edu)
 */


/**
 * BED.
 */

typedef struct {
  char*        chromosome;
  unsigned int start;
  unsigned int end;
  unsigned int extended; // 0 - false; 1 - true
  // Optional
  char*        name;
  unsigned int score;  // [0,1000]
  char         strand; // '+' or '-'
  unsigned int thickStart; 
  unsigned int thickEnd;
  char*        itemRGB; // ex. 255,0,0
  unsigned int blockCount;
  Array        subBlocks; // of SubBlocks
} Bed;
  
typedef struct {
  unsigned int size;
  unsigned int start; // relative to Bed->start
} SubBlock;


extern void bedParser_initFromFile (char *fileName);
extern void bedParser_initFromPipe (char *command);
extern void bedParser_deInit (void);

extern Bed* bedParser_nextEntry (void);
extern Array bedParser_getAllEntries ( void ); 

extern int bedParser_sort (Bed *a, Bed *b);
extern void bedParser_freeBeds (Array beds);

extern char* bedParser_writeEntry( Bed* currBed );

#endif // DEF_BED_PARSER_H
