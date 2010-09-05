/**
 * @file config_reading.h
 * @brief This contains types for reading config files
 * Chained in Sanity
 * Config file types
 */

#ifndef _INCLUDES_TYPES_CONFIG_READING_H_
#define _INCLUDES_TYPES_CONFIG_READING_H_

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
  int (*handler)(cis_config_node *node); /* Returns 0 if sucessful, anything else supresses child processing */
  void (*delete_callback)(cis_handler_node *location);
  map_root *children;
};

#endif
