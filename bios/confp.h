
/**
 * \file confp.h
 * \author Created by David Z. Chen (david.chen@yale.edu)
 */

#ifndef CONFP_H
#define CONFP_H

#include "rbmap.h"

typedef struct {
	struct rbmap *map;
	char *conf_fname;
} config;

config *confp_open(char *file);
char *confp_get(config *conf, char *key);
int confp_close(config *conf);


#endif
