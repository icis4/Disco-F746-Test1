/*
 * dhcp.c
 *
 *  Created on: Aug 10, 2020
 *      Author: Ivaylo Ilchev iic@melexis.com
 */
#include <stdio.h>
#include <string.h>

#include "main.h"
#include "lwip.h"
#include "lwip/opt.h"
#include "lwip/dhcp.h"
#include "lwip/apps/mdns.h"
#include "app_ethernet.h"
#include "ethernetif.h"

#include "libcrc/checksum.h"

#include "app/config.h"
#include "app/debug.h"

//#include "ethernet/tcp_echoserver.h"
//#include "ethernet/tcp_terminal_server.h"
//#include "ethernet/netconn_terminal_server.h"

#define MAX_DHCP_TRIES  4
/* 60sec*/
#define DHCP_TIMEOUT_MS 60000

__IO uint8_t DHCP_state = DHCP_OFF;
extern ETH_HandleTypeDef heth;

/*Static IP ADDRESS*/
#define IP_ADDR0   192
#define IP_ADDR1   168
#define IP_ADDR2   5
#define IP_ADDR3   100

/*NETMASK*/
#define NETMASK_ADDR0   255
#define NETMASK_ADDR1   255
#define NETMASK_ADDR2   255
#define NETMASK_ADDR3   0

/*Gateway Address*/
#define GW_ADDR0   192
#define GW_ADDR1   168
#define GW_ADDR2   5
#define GW_ADDR3   100

char host_name[20]; // = "DISCO-746G";
static uint8_t MAC[6] = { 0xDE, 0x00, 0x04, 0x03, 0x02, 0x01 };

uint8_t mdns_initialized = 0;

const char* getHostName(void) {
	return host_name;
}

const uint8_t* getMACaddress(void) {
	return MAC;
}

char buff[256];
static void srv_txt(struct mdns_service *service, void *txt_userdata) {
	int res;

	sprintf(buff, "version=%s", CFG_VERSION);
	res = mdns_resp_add_service_txtitem(service, buff, strlen(buff));
	LWIP_ERROR("mdns add service txt failed(version)\n", (res == ERR_OK),
			return);
	sprintf(buff, "date=%s", __DATE__);
	res = mdns_resp_add_service_txtitem(service, buff, strlen(buff));
	LWIP_ERROR("mdns add service txt failed(date)\n", (res == ERR_OK), return);
	sprintf(buff, "tcp_raw_terminal=%d", 2001);
	res = mdns_resp_add_service_txtitem(service, buff, strlen(buff));
	LWIP_ERROR("mdns add service txt failed(date)\n", (res == ERR_OK), return);
	sprintf(buff, "tcp_netconn_terminal=%d", 2002);
	res = mdns_resp_add_service_txtitem(service, buff, strlen(buff));
	LWIP_ERROR("mdns add service txt failed(date)\n", (res == ERR_OK), return);
	sprintf(buff, "echo=%d", 2000);
	res = mdns_resp_add_service_txtitem(service, buff, strlen(buff));
	LWIP_ERROR("mdns add service txt failed(date)\n", (res == ERR_OK), return);
}

void MDNS_Init(struct netif* netif) {
	if (mdns_initialized) {
		mdns_resp_restart(netif);
		DBG_PRINT("mdns_resp_restart()\n");
		return;
	}

	DBG_PRINT("MDNS_Init\n");

	mdns_resp_init();
	mdns_resp_add_netif(netif, netif->hostname, 60);
	mdns_resp_add_service(netif, netif->hostname, "_disco", DNSSD_PROTO_TCP,
			2000, 3600, srv_txt, NULL);

	mdns_initialized = 1;
}

void MDNS_DeInit(struct netif* netif) {
	DBG_PRINT("MDNS_DeInit\n");
	mdns_resp_remove_netif(netif);
}

/**
 * @brief  DHCP Process
 * @param  argument: network interface
 * @retval None
 */
void DHCP_thread(void const *argument) {
	struct netif *netif = (struct netif*) argument;
	ip_addr_t ipaddr;
	ip_addr_t netmask;
	ip_addr_t gw;
	struct dhcp *dhcp;
	uint32_t timeout = 0;

	uint32_t phyreg = 0;

	DHCP_state = DHCP_OFF;

	for (;;) {
		switch (DHCP_state) {
		case DHCP_START :
			timeout = osKernelSysTick() + DHCP_TIMEOUT_MS;
			netif->hostname = getHostName();
			ip_addr_set_zero_ip4(&netif->ip_addr);
			ip_addr_set_zero_ip4(&netif->netmask);
			ip_addr_set_zero_ip4(&netif->gw);
			dhcp_start(netif);
			DHCP_state = DHCP_WAIT_ADDRESS;
			DBG_PRINT("  State: Looking for DHCP server ...\n")
			;
			break;

		case DHCP_WAIT_ADDRESS :
			if (dhcp_supplied_address(netif)) {
				DHCP_state = DHCP_ADDRESS_ASSIGNED;
				DBG_PRINT("IP address assigned by a DHCP server: %s\n",
						ip4addr_ntoa((const ip4_addr_t* )&netif->ip_addr));
				MDNS_Init(netif);
			} else {
				dhcp = (struct dhcp*) netif_get_client_data(netif,
						LWIP_NETIF_CLIENT_DATA_INDEX_DHCP);

				/* DHCP timeout */
				if (dhcp->tries > MAX_DHCP_TRIES || timeout < osKernelSysTick()) {
					DHCP_state = DHCP_TIMEOUT;

					/* Stop DHCP */
					dhcp_release_and_stop(netif);

					/* Static address used */
					IP_ADDR4(&ipaddr, IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3);
					IP_ADDR4(&netmask, NETMASK_ADDR0, NETMASK_ADDR1,
							NETMASK_ADDR2, NETMASK_ADDR3);
					IP_ADDR4(&gw, GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);
					netif_set_addr(netif, ip_2_ip4(&ipaddr), ip_2_ip4(&netmask),
							ip_2_ip4(&gw));
					DBG_PRINT("DHCP Timeout. Static IP address: %s\n",
							ip4addr_ntoa((const ip4_addr_t* )&netif->ip_addr));
					MDNS_Init(netif);
				}
			}
			break;

		case DHCP_OFF :
			HAL_ETH_ReadPHYRegister(&heth, PHY_BSR, &phyreg);
			if ((phyreg & PHY_LINKED_STATUS) == PHY_LINKED_STATUS) {
				DHCP_state = DHCP_START;
				netif_set_up(netif);
				DBG_PRINT("* The network cable is connected\n");
			}
			break;

		case DHCP_ADDRESS_ASSIGNED :
		case DHCP_TIMEOUT :
			HAL_ETH_ReadPHYRegister(&heth, PHY_BSR, &phyreg);
			if ((phyreg & PHY_LINKED_STATUS) != PHY_LINKED_STATUS) {
				DBG_PRINT("* The network cable is not connected\n");
				dhcp_release_and_stop(netif);
				DHCP_state = DHCP_OFF;
				netif_set_down(netif);
			}
			break;
		default:
			break;
		}
		/* wait 250 ms */
		osDelay(250);
	}
}

/**
 * @brief  Initializes the lwIP stack
 * @param  None
 * @retval None
 */
static void Netif_Config(struct netif* netif) {
	ip_addr_t ipaddr;
	ip_addr_t netmask;
	ip_addr_t gw;

	IP_ADDR4(&ipaddr, IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3);
	IP_ADDR4(&netmask, NETMASK_ADDR0, NETMASK_ADDR1, NETMASK_ADDR2,
			NETMASK_ADDR3);
	IP_ADDR4(&gw, GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);

	netif_add(netif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init,
			&tcpip_input);

	/* Enable multicast for mDNS */
	(heth.Instance)->MACFFR |= ETH_MULTICASTFRAMESFILTER_NONE;
	netif->flags |= NETIF_FLAG_IGMP;

	/*  Registers the default network interface. */
	netif_set_default(netif);

	if (netif_is_link_up(netif)) {
		/* When the netif is fully configured this function must be called.*/
		netif_set_up(netif);
	} else {
		/* When the netif link is down this function must be called */
		netif_set_down(netif);
	}

	/* Set the link callback function, this function is called on change of link status*/
	netif_set_link_callback(netif, ethernetif_update_config);
}

/**
 * @brief  Start Thread
 * @param  argument not used
 * @retval None
 */
static void StartThread(void const *argument) {
	extern struct netif gnetif; /* network interface structure */

	/* Create tcp_ip stack thread */
	tcpip_init(NULL, NULL);

	/* Initialize the LwIP stack */
	Netif_Config(&gnetif);

	/* Initialize webserver demo */
	// http_server_socket_init();

	/* Initialize tcp echo server */
//	tcp_echoserver_init();

	/* Initialize tcp terminal server */
//	tcp_terminal_server_init();

	/* Initialize netconn terminal server */
//	nc_terminal_init();

	/* Start DHCPClient */
	osThreadDef(DHCP, DHCP_thread, osPriorityNormal, 0,
			configMINIMAL_STACK_SIZE * 2);
	osThreadCreate(osThread(DHCP), &gnetif);

	for (;;) {
		/* Delete the Init Thread */
		osThreadTerminate(NULL);
	}
}

/* Don't work. Bug in Cube? */
#if 0
/**
 * @brief  This function notify user about link status changement.
 * @param  netif: the network interface
 * @retval None
 */
void ethernetif_notify_conn_changed(struct netif *netif) {
	if (netif_is_up(netif)) {
		/* Update DHCP state machine */
		// DHCP_state = DHCP_START;
		DBG_PRINT("=== The network cable is connected\n");
	} else {
		/* Update DHCP state machine */
		// DHCP_state = DHCP_LINK_DOWN;
		DBG_PRINT("=x= The network cable is not connected\n");
	}
}
#endif

void StartEthernet(void) {
	uint32_t id[3];
	id[0] = HAL_GetUIDw0();
	id[1] = HAL_GetUIDw1();
	id[2] = HAL_GetUIDw2();

	uint32_t id_hash = crc_32((void*) id, 12);
	sprintf(host_name, "disco-%08lx", id_hash);
	MAC[0] = 0x1E;
	MAC[1] = 0x00;
	MAC[2] = (id_hash >> 24) & 0xFF;
	MAC[3] = (id_hash >> 16) & 0xFF;
	MAC[4] = (id_hash >> 8) & 0xFF;
	MAC[5] = id_hash & 0xFF;

	DBG_PRINT("\nHostname:%s\n", host_name);
	DBG_PRINT("MAC:%02X-%02X-%02X-%02X-%02X-%02X\n", MAC[0], MAC[1], MAC[2],
			MAC[3], MAC[4], MAC[5]);

	osThreadDef(Start, StartThread, osPriorityNormal, 0,
			configMINIMAL_STACK_SIZE * 2);
	osThreadCreate(osThread(Start), NULL);
}
