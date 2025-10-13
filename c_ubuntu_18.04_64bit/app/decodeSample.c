
#include <ISO14827-2.h>
#include <DSRC.h>
#include <rtxsrc/rtxMemLeakCheck.h>
#include <rtxsrc/rtxDiag.h>
#include <rtxsrc/rtxPrint.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define	HAE_TRUE				((unsigned char)1)
#define	HAE_FALSE				((unsigned char)0)

#define	HAE_OK					((int )0)
#define	HAE_ERROR				((int)-1)

#define HAE_NULL				((void *)0)

#define DSRC_PORT				60000
#define LOCAL_PORT				50000
#define LOCAL_SOURCE_PORT		55555

#define BUFF_SIZE				1024

// Message ID : 19
// unsigned char spat_sample[130] = 
// {
// 	0x00, 0x13, 0x7f, 0x00, 0x18, 0x80, 0xca, 0x00, 0xca, 0x01, 0x04, 0x00, 0x26, 0x64, 0xa8, 0xbb,
// 	0xd0, 0x76, 0x1e, 0x9d, 0x4a, 0x50, 0x64, 0xc7, 0x91, 0x50, 0x04, 0x11, 0x40, 0x04, 0x60, 0x01,
// 	0x00, 0x18, 0x39, 0x91, 0x63, 0x54, 0x02, 0x04, 0x30, 0x01, 0x27, 0x00, 0x40, 0x06, 0x1e, 0x9d,
// 	0x4a, 0x50, 0x64, 0xc7, 0x91, 0x50, 0x0c, 0x10, 0xc0, 0x09, 0x4c, 0x01, 0x00, 0x18, 0x39, 0x91,
// 	0x63, 0x54, 0x04, 0x04, 0x30, 0x02, 0x53, 0x00, 0x40, 0x06, 0x1e, 0x9d, 0x4a, 0x50, 0x64, 0xc7,
// 	0x91, 0x50, 0x14, 0x11, 0x40, 0x04, 0x60, 0x01, 0x00, 0x18, 0x39, 0x91, 0x63, 0x54, 0x06, 0x04,
// 	0x30, 0x01, 0x27, 0x00, 0x40, 0x06, 0x1e, 0x9d, 0x4a, 0x50, 0x64, 0xc7, 0x91, 0x50, 0x1c, 0x10,
// 	0xc0, 0x06, 0xcc, 0x01, 0x00, 0x18, 0x39, 0x91, 0x63, 0x54, 0x08, 0x04, 0x30, 0x01, 0xb3, 0x00,
// 	0x40, 0x00
// };

// unsigned char spat_data[240];

typedef struct{
	int Intersection_id;
	unsigned char movementName[5];
	int signalGroup;
	int eventState;
	unsigned int minEndTime;
} SIG_SPAT;

SIG_SPAT sig_SPaT[11];

struct sockaddr_in dsrc_addr, dsrx_rx_addr, local_addr;
struct sockaddr_in source_addr;

socklen_t rx_dsrc_len;
int UDPRxDSRC;

int dsrc_sock_fd;
int local_sock_fd;

unsigned char dsrc_data[BUFF_SIZE] = {0,};
unsigned char local_data[BUFF_SIZE] = {0,};

unsigned char sDecode_MessageFrame(OSCTXT *pctxt, unsigned char *pBuf, unsigned int ulLength, MessageFrame *pFrame);
unsigned char sDecode_DSRCmsg(OSCTXT *pctxt, unsigned short uiMessageId, unsigned char *pBuf, unsigned int ulLength, unsigned char *pMessage);
void parseSpat(SPAT *pSpat);

int UDP_Init(void);

void main(void)
{
	int ret = 0;
	int i = 0;
	unsigned char status = HAE_OK;
	int send = 0;
	// struct sig_SPaT *p = malloc(sizeof(sig_SPaT)*10);
	void *p;

	OSCTXT ctxt2;
	
	unsigned char *pEncodingData;
	unsigned int ulLength;
	MessageFrame tFrame2;

	/* Decode SPAT message */
	printf("\n[SPAT] Decode\n");
	SPAT pSpat;

	// pEncodingData = spat_sample;
	// ulLength = 130;
	ulLength = 197-16;
	if(ret = UDP_Init() < 0)
	{
		exit(1);
	}
	printf("Start\r\n");

	// printf("%d\r\n", sizeof(SIG_SPAT)*10);
	
	for(;;)
	{
		memset(&dsrc_data, 0, BUFF_SIZE);//sizeof(dsrc_data));
		rx_dsrc_len = sizeof(dsrc_addr);
	
		// UDPRxDSRC = recvfrom(dsrc_sock_fd, dsrc_data, sizeof(dsrc_data), 0, (struct sockaddr *) &dsrc_addr, &rx_dsrc_len);
		UDPRxDSRC = recvfrom(dsrc_sock_fd, dsrc_data, BUFF_SIZE, 0, (struct sockaddr *) &dsrc_addr, &rx_dsrc_len);
		printf("recv data: %d\r\n", UDPRxDSRC);
		if((dsrc_data[17] == 0x13))// && (UDPRxDSRC == 197))  // 0x13 == 19
		{	
			pEncodingData = &dsrc_data[16];

			ulLength = UDPRxDSRC - 16;

			memset(&tFrame2, 0x00, sizeof(MessageFrame));
			status = sDecode_MessageFrame(&ctxt2, pEncodingData, ulLength, (MessageFrame *)&tFrame2);
			status = sDecode_DSRCmsg(&ctxt2, tFrame2.messageId, (unsigned char *)tFrame2.value.data, (unsigned int)tFrame2.value.numocts, (unsigned char *)&pSpat);

			parseSpat((SPAT *)&pSpat);
			
			rtFreeContext (&ctxt2);
			
			p= malloc(BUFF_SIZE);// malloc(sizeof(SIG_SPAT) * 10);
			
			memset(p, 0, BUFF_SIZE);//sizeof(SIG_SPAT) * 10);
			memcpy(p, (void *)sig_SPaT, BUFF_SIZE);//sizeof(SIG_SPAT) * 10);

			//if(sig_SPaT[4].Intersection_id != 0)//== 400)
			{
			//	 if(sig_SPaT[4].signalGroup == 1)
				 {
				//	 printf("Intersection ID : %d\r\n", sig_SPaT[8].Intersection_id);
				//	 printf("signalGroup : %d\r\n", sig_SPaT[8].signalGroup);
				//	 printf("movementName : %s\r\n", sig_SPaT[8].movementName);
				//	 printf("eventState : %d\r\n", sig_SPaT[8].eventState);
				//	 printf("minEndTime : %d\r\n", sig_SPaT[8].minEndTime);
				//	 printf("\r\n");
				 }
				 //if(sig_SPaT[5].signalGroup == 5)
				 {
				 //	printf("Intersection ID : %d\r\n", sig_SPaT[5].Intersection_id);
				 //	printf("signalGroup : %d\r\n", sig_SPaT[5].signalGroup);
				 //	printf("movementName : %s\r\n", sig_SPaT[5].movementName);
				 //	printf("eventState : %d\r\n", sig_SPaT[5].eventState);
				 //	printf("minEndTime : %d\r\n", sig_SPaT[5].minEndTime);
				 }
			}
			
			// send = sendto(local_sock_fd, local_data, sizeof(local_data), 0, (struct sockaddr *) &local_addr, sizeof(local_addr));
			send = sendto(local_sock_fd, p, BUFF_SIZE, 0, (struct sockaddr *) &local_addr, sizeof(local_addr));
		
			// free(&sig_SPaT);
			free(p);

			printf("Send SPaT Message to ROS node\r\n");
			

			for(i=0;i<10;i++)
			{
				memset(&sig_SPaT[i], 0, sizeof(sig_SPaT));
			}
		}
		
	}
	
	
}

unsigned char sDecode_MessageFrame(OSCTXT *pctxt, unsigned char *pBuf, unsigned int ulLength, MessageFrame *pFrame)
{
	unsigned char status = HAE_OK;
	
	unsigned char 	verbose = HAE_TRUE;
	unsigned char	aligned = HAE_FALSE;
	unsigned char	trace = HAE_TRUE;


	/************************************************
		1. Initialize variables
	*************************************************/

	DECLARE_MEMLEAK_DETECTOR;

	if(HAE_NULL == pBuf)
	{
		status = HAE_ERROR;
		printf( "[CENTER] ERROR : pBuf is NULL\n");
	}

	/************************************************
		2. Initializes an OSCTXT block  
	*************************************************/

	if(HAE_OK == status)
	{
		status = rtInitContext (pctxt);
		if (HAE_OK == status)
		{
			rtxSetDiag (pctxt, verbose);
			pu_setBuffer (pctxt, pBuf, ulLength, aligned);
			pu_setTrace (pctxt, trace);
		}
		else
		{
			rtxErrPrint (pctxt);
			status = HAE_ERROR;
			printf( "[CENTER] ERROR : rtInitContext() for MessageFrame\n");
		}
	}

	/************************************************
		3. Decoding MessageFrame 
	*************************************************/

	if(HAE_OK == status)
	{
		status = asn1PD_MessageFrame(pctxt, pFrame);
		
		if(HAE_OK == status)
		{
			if(HAE_TRUE == trace)
			{
				printf("[CENTER] decode of MessageFrame was successful\n");
				// asn1Print_MessageFrame("Decode MessageFrame", pFrame);
			}
		}
		else
		{
			rtxErrPrint (pctxt);
			status = HAE_ERROR;
			printf( "[CENTER] ERROR : decode of MessageFrame failed\n");
		}
	}

	return status;
}

/*************************************************************
 *
 * Function 		: sDecode_DSRCmsg
 * 
 * Description	: Decode DSRCmsg
 *
 * Parameter	: 
 * 
 * Returns		: 
 *
 * Notes		: 
 *
 *************************************************************/
unsigned char sDecode_DSRCmsg(OSCTXT *pctxt, unsigned short uiMessageId, unsigned char *pBuf, unsigned int ulLength, unsigned char *pMessage)
{
	unsigned char status = HAE_OK;
	
	unsigned char 	verbose = HAE_TRUE;
	unsigned char	aligned = HAE_FALSE;
	unsigned char	trace = HAE_TRUE;

	/************************************************
		1. Initialize variables
	*************************************************/

	DECLARE_MEMLEAK_DETECTOR;

	if(HAE_NULL == pBuf)
	{
		status = HAE_ERROR;
		printf( "[CENTER] ERROR : pBuf is NULL\n");
	}

	/************************************************
		2. Initializes an OSCTXT block  
	*************************************************/

	if(HAE_OK == status)
	{
		status = rtInitContext (pctxt);
		if (HAE_OK == status)
		{
			rtxSetDiag (pctxt, verbose);
			pu_setBuffer (pctxt, pBuf, ulLength, aligned);
			pu_setTrace (pctxt, trace);
		}
		else
		{
			rtxErrPrint (pctxt);
			status = HAE_ERROR;
			printf( "[CENTER] ERROR : rtInitContext() for DSRCmsg\n");
		}
	}

	/************************************************
		3. Decoding DSRCmsg
	*************************************************/

	if(HAE_OK == status)
	{
		switch(uiMessageId)
		{
			case ASN1V_signalPhaseAndTimingMessage:
				status = asn1PD_SPAT(pctxt, (SPAT *)pMessage);
				printf("Decode SPaT OK\r\n");
				break;
				
			case ASN1V_mapData:
				status = asn1PD_MapData(pctxt, (MapData *)pMessage);
				break;
				
				
			case ASN1V_roadSideAlert:
				status = asn1PD_RoadSideAlert(pctxt, (RoadSideAlert *)pMessage);
				break;
				
			case ASN1V_rtcmCorrections:
				status = asn1PD_RTCMcorrections(pctxt, (RTCMcorrections *)pMessage);
				break;
				
			case ASN1V_travelerInformation:	
				status = asn1PD_TravelerInformation(pctxt, (TravelerInformation *)pMessage);
				break;
				
			case ASN1V_basicSafetyMessage:
				status = asn1PD_BasicSafetyMessage(pctxt, (BasicSafetyMessage *)pMessage);
				break;
				
			case ASN1V_probeVehicleData:
			default:
				printf( "[CENTER] ERROR : DSRCmsg Invalid messageId(0x%x)\n", uiMessageId);
				break;
		}	
		
		if(HAE_OK == status)
		{
			if(HAE_TRUE == trace)
			{
				switch(uiMessageId)
				{
					case ASN1V_mapData:
						asn1Print_MapData("MapData", (MapData *)pMessage);
						break;
						
					case ASN1V_signalPhaseAndTimingMessage:
						asn1Print_SPAT("SPAT", (SPAT *)pMessage);
						
						break;
						
					case ASN1V_roadSideAlert:
						asn1Print_RoadSideAlert("RSA", (RoadSideAlert *)pMessage);
						break;
						
					case ASN1V_rtcmCorrections:
						asn1Print_RTCMcorrections("RTCM", (RTCMcorrections *)pMessage);
						break;
						
					case ASN1V_travelerInformation:
						asn1Print_TravelerInformation("TIM", (TravelerInformation *)pMessage);
						break;
						
					case ASN1V_basicSafetyMessage:
						asn1Print_BasicSafetyMessage("BSM", (BasicSafetyMessage *)pMessage);
						break;
									
					case ASN1V_probeVehicleData:						
					default :
						break;
				}
			}
		}
	}
	else
	{
		rtxErrPrint (pctxt);
		status = HAE_ERROR;
		printf( "[CENTER] ERROR : decode of DSRCmsg failed\n");
	}

	return status;
}


void parseSpat(SPAT *pSpat)
{
	OSUINT32 xx1 = 0;
	OSUINT32 xx2 = 0;
	OSUINT32 xx3 = 0;

	OSRTDListNode* pnode;
	OSRTDListNode* pnode2;
	OSRTDListNode* pnode3;
	
 	OSRTDList* plist2;
	OSRTDList* plist4;
	OSRTDList* plist6;
	 
	IntersectionState* pdata;
	MovementState* pmovement;
	MovementEvent* pmoveEvent;

	plist2 = &pSpat->intersections;

	for (pnode = plist2->head; pnode != 0 && xx1 < plist2->count; pnode = pnode->next)
	{
		pdata = ((IntersectionState*)pnode->data);

		// printf("Intersection id : %d\n", pdata->id.id);

		// Encode Localhost data frame
		if(pdata->id.id == 200)
		{
			sig_SPaT[0].Intersection_id = 200;
		}
		else if(pdata->id.id == 300)
		{
			sig_SPaT[1].Intersection_id = 300;
		}
		else if(pdata->id.id == 400)
		{
			sig_SPaT[2].Intersection_id = 400;
		}
		// else if(pdata->id.id == 500)
		// {
		// 	sig_SPaT[8].Intersection_id = 500;
		// }
		else if(pdata->id.id == 610)
		{
			sig_SPaT[3].Intersection_id = 610;
		}
		else if(pdata->id.id == 700)
		{
			sig_SPaT[4].Intersection_id = 700;
		}
		else
		{

		}
		xx2 = 0;
		plist4 = &pdata->states;

		for (pnode2 = plist4->head; pnode2 != 0 && xx2 < plist4->count; pnode2 = pnode2->next)
		{
			pmovement = ((MovementState*)pnode2->data);

			if(pmovement->m.movementNamePresent == 1)
			{

				// printf("movementName : %s\n", pmovement->movementName);
			}

			// printf("signalGroup : %d\n", pmovement->signalGroup);

			xx3 = 0;
			plist6 = &pmovement->state_time_speed;
			for (pnode3 = plist6->head; pnode3 != 0 && xx3 < plist6->count; pnode3 = pnode3->next)
			{
				pmoveEvent = (MovementEvent*)pnode3->data;

				if(pmoveEvent->eventState == 0)
				{
					// printf("eventState : unavailable(%d) -> %d (1/10s)\n", pmoveEvent->eventState, pmoveEvent->timing.minEndTime);
				}
				else if(pmoveEvent->eventState == 3)
				{
					// printf("eventState : RED stop-And-Remain(%d) -> %d (1/10s)\n", pmoveEvent->eventState, pmoveEvent->timing.minEndTime);
				}
				else if(pmoveEvent->eventState == 5)
				{
					// printf("eventState : GREEN permissive-Movemnt-Allowed(%d) -> %d (1/10s)\n", pmoveEvent->eventState, pmoveEvent->timing.minEndTime);
				}
				else if(pmoveEvent->eventState == 8)
				{
					// printf("eventState : YELLOW protected-clearance(%d) -> %d (1/10s)\n", pmoveEvent->eventState, pmoveEvent->timing.minEndTime);
				}
				else
				{
				}

				if(sig_SPaT[0].Intersection_id == 200)
				{
					if(pmovement->signalGroup == 1)
					{
						// sig_SPaT.movementName = pmovement->movementName;
						strcpy(sig_SPaT[0].movementName, pmovement->movementName);
						sig_SPaT[0].signalGroup = pmovement->signalGroup;	
						sig_SPaT[0].eventState = pmoveEvent->eventState;
						sig_SPaT[0].minEndTime = pmoveEvent->timing.minEndTime;
					}
				}

				if(sig_SPaT[1].Intersection_id == 300)
				{
					if(pmovement->signalGroup == 1)
					{
						// sig_SPaT.movementName = pmovement->movementName;
						strcpy(sig_SPaT[1].movementName, pmovement->movementName);
						sig_SPaT[1].signalGroup = pmovement->signalGroup;	
						sig_SPaT[1].eventState = pmoveEvent->eventState;
						sig_SPaT[1].minEndTime = pmoveEvent->timing.minEndTime;
					}
				}

				if(sig_SPaT[2].Intersection_id == 400)
				{
					if(pmovement->signalGroup == 2)
					{
						// sig_SPaT.movementName = pmovement->movementName;
						strcpy(sig_SPaT[2].movementName, pmovement->movementName);
						sig_SPaT[2].signalGroup = pmovement->signalGroup;	
						sig_SPaT[2].eventState = pmoveEvent->eventState;
						sig_SPaT[2].minEndTime = pmoveEvent->timing.minEndTime;
					}
				}

				if(sig_SPaT[3].Intersection_id == 610)
				{
					if(pmovement->signalGroup == 1)
					{
						// sig_SPaT.movementName = pmovement->movementName;
						strcpy(sig_SPaT[3].movementName, pmovement->movementName);
						sig_SPaT[3].signalGroup = pmovement->signalGroup;	
						sig_SPaT[3].eventState = pmoveEvent->eventState;
						sig_SPaT[3].minEndTime = pmoveEvent->timing.minEndTime;
					}
				}

				if(sig_SPaT[4].Intersection_id == 700)
				{
					if(pmovement->signalGroup == 8)
					{
						// sig_SPaT.movementName = pmovement->movementName;
						strcpy(sig_SPaT[4].movementName, pmovement->movementName);
						sig_SPaT[4].signalGroup = pmovement->signalGroup;	
						sig_SPaT[4].eventState = pmoveEvent->eventState;
						sig_SPaT[4].minEndTime = pmoveEvent->timing.minEndTime;
					}
				}
				xx3++;
			}
			
			xx2++;
		}
		
		xx1++;
	}
}

int UDP_Init(void)
{
	int optVal = 1;
	int optLen = sizeof(optVal);

	printf("UDP DSRC Socket has been created.\n");

	// Open UDP Port for DSRC
	if ((dsrc_sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("socket ");
		return -1;
	}

	memset(&dsrc_addr, 0x00, sizeof(dsrc_addr));
	dsrc_addr.sin_family = AF_INET;
	// dsrc_addr.sin_addr.s_addr = inet_addr("192.168.10.101"); // FOR Lab Test
	dsrc_addr.sin_addr.s_addr = inet_addr("192.168.1.3"); //htonl(INADDR_ANY);
	dsrc_addr.sin_port = htons(DSRC_PORT);
	
	setsockopt(dsrc_sock_fd, SOL_SOCKET, SO_RCVTIMEO, (char *)&optVal, optLen);
	
	if (bind(dsrc_sock_fd, (struct sockaddr *) &dsrc_addr, sizeof(dsrc_addr))
			< 0) {
		perror("bind DSRC");
		return 1;
	}
	printf("UDP DSRC Socket has been created.\n");

	// Open UDP Port for Local
	if ((local_sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("socket ");
		return -1;
	}
    


	memset(&local_addr, 0x00, sizeof(local_addr));
	local_addr.sin_family = AF_INET;
	local_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //htonl(INADDR_ANY);
	local_addr.sin_port = htons(LOCAL_PORT);

	memset(&source_addr, 0, sizeof(source_addr));
	source_addr.sin_family = AF_INET;
	source_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	source_addr.sin_port = htons(LOCAL_SOURCE_PORT);
	if (bind(local_sock_fd, (struct sockaddr *) &source_addr, sizeof(source_addr))
			< 0) {
		perror("bind Local");
		return 1;
	}
	printf("UDP Local Socket has been created.\n");

}