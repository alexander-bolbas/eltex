#ifndef IPV4_H
#define IPV4_H

#include <stdio.h>
#include "types.h"

int ipv4_parse(const char *str, ipv4_addr_t *out);
int ipv4_is_valid_mask(ipv4_addr_t mask);
int ipv4_same_subnet(ipv4_addr_t ip, ipv4_addr_t gw, ipv4_addr_t mask);
ipv4_addr_t ipv4_random(void);
void ipv4_to_string(ipv4_addr_t ip, char buf[16]);

#endif // IPV4_H
