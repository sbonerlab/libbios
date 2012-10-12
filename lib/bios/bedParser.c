#include "format.h"
#include "log.h"
#include "linestream.h"
#include "common.h"
#include "bedParser.h"


/** 
 *   \file bedParser.c Module to parse BED files
 *   \author Andrea Sboner (ans2077@med.cornell.edu)
 */

static LineStream ls = NULL;

/**
 * Initialize the bedParser module from file.
 * @param[in] fileName File name, use "-" to denote stdin
 */
void bedParser_initFromFile (char *fileName)
{  
  ls = ls_createFromFile (fileName);
  ls_bufferSet (ls,1);
}



/**
 * Initialize the bedParser module from pipe.
 * @param[in] command Command to be executed
 */
void bedParser_initFromPipe (char *command)
{
  ls = ls_createFromPipe (command);
  ls_bufferSet (ls,1);
}



/**
 * Deinitialize the bowtieParser module.
 */
void bedParser_deInit (void)
{
  ls_destroy (ls);
}



/**
 * Retrieve the next entry in the BED file.
 */
Bed* bedParser_nextEntry (void)
{
  char *line;
  int i;
  static Bed *currBed = NULL;
  WordIter w, wsizes, wstarts;
  line = ls_nextLine (ls);
  if ( !(ls_isEof( ls ) ) ) {	 
    if ( !strStartsWithC (line,"track") && !strStartsWithC(line, "browser") ) {
      AllocVar( currBed );
      w = wordIterCreate (line,"\t",1);
      currBed->chromosome = hlr_strdup (wordNext (w));
      currBed->start = atoi (wordNext (w));
      currBed->end = atoi (wordNext (w));
      currBed->name = hlr_strdup( wordNext(w) );
      if( currBed->name ) {
	currBed->extended = 1;
	currBed->score = atoi( wordNext( w ) );
	currBed->strand = wordNext( w )[0];
	currBed->thickStart = atoi( wordNext( w ) );
	currBed->thickEnd = atoi( wordNext( w ) );
	currBed->itemRGB = hlr_strdup( wordNext( w ) );
	currBed->blockCount = atoi( wordNext( w ) );
	currBed->subBlocks = arrayCreate( currBed->blockCount, SubBlock );
	wsizes = wordIterCreate( wordNext( w ), ",", 1);
	wstarts = wordIterCreate( wordNext( w ), ",", 1); 
	for( i=0; i < currBed->blockCount; i++) {
	  SubBlock *currSubBlock = arrayp( currBed->subBlocks, arrayMax( currBed->subBlocks ), SubBlock );
	  currSubBlock->size = atoi( wordNext( wsizes ) );
	  currSubBlock->start = atoi( wordNext( wstarts ) );
	}
	wordIterDestroy (wsizes);
	wordIterDestroy (wstarts);
      } else {
	currBed->extended = 0;
      }
      wordIterDestroy (w);
    } else {
      bedParser_nextEntry ( );
    }
  } else {
    return NULL;
  }
  return currBed;
}



/**
 * Retrieve all entries from a Bed file.
 */
Array bedParser_getAllEntries ( void ) 
{
  Array Beds;
  Bed *currBed;
  
  Beds = arrayCreate (1000000, Bed);
  int i=0;
  while (currBed = bedParser_nextEntry () ) {
    array(Beds, arrayMax (Beds),Bed) = *currBed;
    freeMem ( currBed );
    i++;
  }
  return Beds;
}
  
                          

/**
 * Sort Bed entries by chromosome, start and end.
 */
int bedParser_sort (Bed *a, Bed *b)
{
  int diff;

  diff = strcmp (a->chromosome,b->chromosome);
  if (diff != 0) {
    return diff;
  }
  diff = a->start - b->start;
  if (diff != 0) {
    return diff;
  }
  return b->end - a->end;
}



/**
 * Free an array of Bed elements.
 */
void bedParser_freeBeds (Array Beds)
{
  Bed *currBed;
  int i;
  
  for (i = 0; i < arrayMax (Beds); i++) {
    currBed = arrp (Beds,i,Bed);
    hlr_free (currBed->chromosome);
  }
  arrayDestroy (Beds);
}

char* bedParser_writeEntry( Bed* currBed ) {
  int i;
  static Stringa buffer = NULL;
  stringCreateClear (buffer,50);
  SubBlock *currSubBlock;

  if( currBed->extended ) {
    stringPrintf (buffer,"%s\t%d\t%d\t%s\t%d\t%c\t%d\t%d\t%s\t%d\t",
		  currBed->chromosome,
		  currBed->start,
		  currBed->end,
		  currBed->name,
		  currBed->score,
		  currBed->strand,
		  currBed->thickStart,
		  currBed->thickEnd,
		  currBed->itemRGB,
		  currBed->blockCount );    
    for (i = 0; i < arrayMax (currBed->subBlocks); i++) {
      currSubBlock = arrp (currBed->subBlocks,i,SubBlock);
      stringAppendf (buffer,"%d%s",currSubBlock->size, i < arrayMax (currBed->subBlocks) - 1 ? "," : "\t");
    }
    for (i = 0; i < arrayMax (currBed->subBlocks); i++) {
      currSubBlock = arrp (currBed->subBlocks,i,SubBlock);
      stringAppendf (buffer,"%d%s",currSubBlock->start, i < arrayMax (currBed->subBlocks) - 1 ? "," : "");
    }
  }
  else {
    stringPrintf( buffer, "%s\t%d\t%d", currBed->chromosome, currBed->start, currBed->end );
  }
  return string (buffer);
}

