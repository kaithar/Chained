/**
 * @file config_reading.h
 * @brief This contains prototypes for reading config files
 * Chained in Sanity
 * Config file prototypes
 */

#ifndef _INCLUDES_CONFIG_READING_H_
#define _INCLUDES_CONFIG_READING_H_

typedef struct cis_config_node cis_config_node;

struct cis_config_node {
	char *name;
	cis_config_node *parent;
	bool block;
	char *text;
	linklist_root *children;
};

extern int cis_load_config(unsigned char *filename);

#endif
