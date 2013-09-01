#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>

#include "format.h"
#include "linestream.h"
#include "confp.h"

static int confp_parse_line(struct rbmap *map, char *line, int n)
{
	char *c;
	char *key = NULL, 
	     *val = NULL;
	char *start = line,
	     *mid = NULL,
	     *end = start + strlen(line) - 1;
	int len, quote = 0;

	if (c = strchr(line, '#'))
		end = c;

	/* Skip leading whitespace */
	for (c = start; *c && isspace(*c); c++)
		;
	
	if (!*c || *c == '#')
		return 0;

	if ((mid = strchr(line, '=')) == NULL) {
		fprintf(stderr, "Syntax error in config file line %d: missing '='\n", n);
		return -1;
	}

	start = c;

	if (c == mid) {
		fprintf(stderr, "Syntax error in config file line %d: missing key\n", n);
		return -1;
	}

	/* Find end of key token and copy key*/
	for ( ; *c && !isspace(*c) && c != mid; c++)
		;

	end = c;

	len = end - start;
	key = (char *) malloc((len + 1) * sizeof(char));
	strncpy(key, start, len);
	*(key + len) = '\0';

	/* Skip whitespace before value token */
	for (c = mid + 1; *c && isspace(*c); c++)
		;

	start = c;

	if (*c == '"') {
		quote = 1;
		start++;
	}

	/* Find end of value token and copy value */
	if (quote)
		for (c = start; *c && *c != '"'; c++)
			;
	else
		for (c = start; *c && !isspace(*c); c++)
			;
	end = c;

	if (quote && *c != '"') {
		fprintf(stderr, "Syntax error in config file line %d: unmatched quote\n", n);
		free(key);
		return -1;
	}

	len = end - start;
	val = (char *) malloc((len + 1) * sizeof(char));
	strncpy(val, start, len);
	*(val + len) = '\0';

	rbmap_set(map, key, val);

	free(key);
	free(val);
	return 0;
}

static int confp_read(config *conf)
{
	LineStream ls;
	char *line;
	int n = 1;

	if ((ls = ls_createFromFile(conf->conf_fname)) == NULL) {
		fprintf(stderr, "Cannot open config file %s\n", conf->conf_fname);
		return -1;
	}

	while ((line = ls_nextLine(ls)) != NULL) {
		if (confp_parse_line(conf->map, line, n) < 0) {
			fprintf(stderr, "Failed to load config file %s\n", conf->conf_fname);
			return -1;
		}
		n++;
	}

	ls_destroy(ls);

	return n;
}

static void confp_delete(config *conf)
{
	rbmap_destroy(conf->map);
	free(conf->conf_fname);
	free(conf);
}

config *confp_open(char *fname)
{
	config *new;

	if (fname == NULL)
		return NULL;

	new = (config *) malloc(sizeof(*new));

	new->map = rbmap_new();
	new->conf_fname = (char *) malloc((strlen(fname) + 1) * sizeof(char));
	strcpy(new->conf_fname, fname);

	if (confp_read(new) < 0) {
		confp_delete(new);
		return NULL;
	}

	return new;
}

char *confp_get(config *conf, char *key)
{
	return rbmap_get(conf->map, key);
}

int confp_close(config *conf)
{
	confp_delete(conf);

	return 1;
}
