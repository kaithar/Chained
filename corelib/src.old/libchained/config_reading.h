/**
 * @file config_reading.h
 * @brief This contains prototypes for reading config files
 * Chained in Sanity
 * Config file prototypes
 */

#ifndef _INCLUDES_CONFIG_READING_H_
#define _INCLUDES_CONFIG_READING_H_

extern int cis_load_config(unsigned char *filename);
extern cis_handler_node *cis_config_add_handler(cis_handler_node *location, unsigned char *name, int (*handler)(cis_config_node *node), void (*delete_callback)(cis_handler_node *location));
extern void cis_config_del_handler(cis_handler_node *location);

#endif
