/**
 *
 * MQTT/UDP project
 *
 * https://github.com/dzavalishin/mqtt_udp
 * Copyright (C) 2017-2018 Dmitry Zavalishin, dz@dz.ru
 *
 * @file
 * @brief Example program: Dump all incoming packets
 *
**/

#include "config.h"

#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <locale.h>
#include <fcntl.h>
#include <errno.h>

#include "../mqtt_udp.h"



int main(int argc, char *argv[])
{
    printf("Demo of MQTT/UDP passive remote configuration\n\n");

    while(1)
    {
        int rc = mqtt_udp_recv_loop( mqtt_udp_dump_any_pkt );
        if( rc ) {
            //printf("mqtt_udp_recv_loop() = %d", rc);
            //perror("error");
            mqtt_udp_global_error_handler( MQ_Err_Other, rc, "recv_loop error", 0 );
            //exit(1);
        }
    }

    return 0;
}

// not ready
#if 0


// -----------------------------------------------------------------------
//
// Use remote config item list (rconfig.client.c) as
// channel number / topic name map. Item index for
// topic is a local channel number
//
// DO NOT MOVE items with MQ_CFG_KIND_TOPIC, their position
// is local channel number.
//
// -----------------------------------------------------------------------


// Will be parameter of mqtt_udp_rconfig_client_init()
mqtt_udp_rconfig_item_t rconfig_list[] =
{
    { MQ_CFG_TYPE_STRING, MQ_CFG_KIND_TOPIC, "Switch 1 topic",	"topic/sw1", { .s = 0 } },
    { MQ_CFG_TYPE_STRING, MQ_CFG_KIND_TOPIC, "Switch 2 topic",	"topic/sw2", { .s = 0 } },
    { MQ_CFG_TYPE_STRING, MQ_CFG_KIND_TOPIC, "Switch 3 topic",	"topic/sw3", { .s = 0 } },
    { MQ_CFG_TYPE_STRING, MQ_CFG_KIND_TOPIC, "Switch 4 topic",	"topic/sw4", { .s = 0 } },
    { MQ_CFG_TYPE_STRING, MQ_CFG_KIND_TOPIC, "Di 0 topic",	"topic/di0", { .s = 0 } },
    { MQ_CFG_TYPE_STRING, MQ_CFG_KIND_TOPIC, "Di 1 topic",	"topic/di1", { .s = 0 } },

    { MQ_CFG_TYPE_STRING, MQ_CFG_KIND_OTHER, "MAC address", 	"net/mac",   { .s = 0 } },

    { MQ_CFG_TYPE_STRING, MQ_CFG_KIND_INFO, "Switch 4 topic", "info/soft",   { .s = "C RConfig Demo" } },
    { MQ_CFG_TYPE_STRING, MQ_CFG_KIND_INFO, "Switch 4 topic", "info/ver",    { .s = 0 } },
    { MQ_CFG_TYPE_STRING, MQ_CFG_KIND_INFO, "Switch 4 topic", "info/uptime", { .s = 0 } },  // DO NON MOVE OR ADD LINES ABOVE, inited by array index below

    { MQ_CFG_TYPE_STRING, MQ_CFG_KIND_OTHER, "Name", 		"node/name", { .s = 0 }, .opaque.s = ee_cfg.node_name }, // TODO R/W
    { MQ_CFG_TYPE_STRING, MQ_CFG_KIND_OTHER, "Location", 	"node/location", { .s = 0 }, .opaque.s = ee_cfg.node_location }, // TODO R/W
};


// Will be parameter of mqtt_udp_rconfig_client_init()
int rconfig_list_size = sizeof(rconfig_list) / sizeof(mqtt_udp_rconfig_item_t);


static int rconfig_rw_callback( int pos, int write );

void init_rconfig( void )
{
    char *mac_string = "020404040000";

    rconfig_list[8].value.s = "00:00:00";
    rconfig_list[9].value.s = "?";

    int rc = mqtt_udp_rconfig_client_init( mac_string, rconfig_rw_callback, rconfig_list, rconfig_list_size );
    if( rc ) printf("rconfig init failed, %d\n", rc );
}






// -----------------------------------------------------------------------
//
// Callback to connect to host code
//
// -----------------------------------------------------------------------


// Must read from local storage or write to local storage
// config item at pos
static int rconfig_rw_callback( int pos, int write )
{
    printf("asked to %s item %d\n", write ? "save" : "load", pos );

    if( (pos < 0) || (pos >= rconfig_list_size) ) return -1; // TODO error

    if( rconfig_list[pos].type != MQ_CFG_TYPE_STRING )
        return -2;

    if( rconfig_list[pos].kind == MQ_CFG_KIND_TOPIC )
    {
        if( write )
        {
            printf("new val = '%s'\n", rconfig_list[pos].value.s );
            if( pos < EEPROM_CFG_N_TOPICS )
            {
                strlcpy( ee_cfg.topics[pos], rconfig_list[pos].value.s, sizeof( ee_cfg.topics[pos] ) );
                int rc = runtime_cfg_eeprom_write(); // TODO do write from thread with timeout to combine writes
                if( rc ) printf("eeprom wr err %d\n", rc );
            }
            return 0;
        }
        else
        {

            if( pos < EEPROM_CFG_N_TOPICS )
                mqtt_udp_rconfig_set_string( pos, ee_cfg.topics[pos] );
            return 0;
        }
    }

    if( rconfig_list[pos].opaque.s != 0 )
    {
        if( write )
        {
            strlcpy( rconfig_list[pos].opaque.s, rconfig_list[pos].value.s, EEPROM_CFG_MAX_TOPIC_LEN );

            mqtt_udp_dump( rconfig_list[pos].opaque.s, EEPROM_CFG_MAX_TOPIC_LEN );

            printf("val '%s'\n", rconfig_list[pos].value.s );
            printf("node name '%s' loc '%s'\n", ee_cfg.node_name, ee_cfg.node_location );

            int rc = runtime_cfg_eeprom_write(); // TODO do write from thread with timeout to combine writes
            if( rc ) printf("eeprom wr err %d\n", rc );
        }
        else
        {
            printf("node name '%s' loc '%s'\n", ee_cfg.node_name, ee_cfg.node_location );

            mqtt_udp_dump( rconfig_list[pos].opaque.s, EEPROM_CFG_MAX_TOPIC_LEN );

            mqtt_udp_rconfig_set_string( pos, rconfig_list[pos].opaque.s );
        }
    }

}


#endif


