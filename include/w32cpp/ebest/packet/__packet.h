#pragma pack(push, 1)

typedef struct _RECV_PACKET {
    int nRqID;
    int nDataLength;
    int nTotalDataBufferSize;
    int nElapsedTime;
    int nDataMode;
    char szTrCode[10 + 1];
    char cCont[1];
    char szContKey[18 + 1];
    char szUserData[30 + 1];
    char szBlockName[17];
    unsigned char* lpData;
} RECV_PACKET, *LPRECV_PACKET;

typedef struct _MSG_PACKET {
    int nRqID;
    int nIsSystemError;
    char szMsgCode[5 + 1];
    int nMsgLength;
    unsigned char* lpszMessageData;
} MSG_PACKET, *LPMSG_PACKET;

typedef struct _REAL_RECV_PACKET {
    char szTrCode[3 + 1];
    int nKeyLength;
    char szKeyData[32 + 1];
    char szRegKey[32 + 1];
    int nDataLength;
    char* pszData;
} RECV_REAL_PACKET, *LPRECV_REAL_PACKET;

typedef struct _LINKDATA_RECV_MSG {
    char sLinkName[32];
    char sLinkData[32];
    char sFiller[64];
} LINKDATA_RECV_MSG, *LPLINKDATA_RECV_MSG;

#pragma pack(pop)