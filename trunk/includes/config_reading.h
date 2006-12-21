/**
 * @file config_reading.h
 * @brief This contains prototypes for reading config files
 * Chained in Sanity
 * Config file prototypes
 */

#ifndef _INCLUDES_CONFIG_READING_H_
#define _INCLUDES_CONFIG_READING_H_

typedef struct cis_config_node cis_config_node;
typedef struct cis_handler_node cis_handler_node;

struct cis_config_node {
	unsigned char *name;
	cis_config_node *parent;
	bool block;
	char *text;
	linklist_root *children;
};

struct cis_handler_node {
	unsigned char *name;
	cis_handler_node *parent;
	void (*handler)(cis_config_node *node);
	void (*delete_callback)(cis_handler_node *location);
	map_root *children;
};

extern int cis_load_config(unsigned char *filename);
extern cis_handler_node *cis_config_add_handler(cis_handler_node *location, unsigned char *name, void (*handler)(cis_config_node *node), void (*delete_callback)(cis_handler_node *location));
extern void cis_config_del_handler(cis_handler_node *location);

#endif
